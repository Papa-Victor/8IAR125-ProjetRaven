#include "Raven_Game.h"

#include "Raven_WeaponSystem.h"
#include "armory/Raven_Weapon.h"
#include "Raven_ObjectEnumerations.h"
#include "Raven_SensoryMemory.h"
#include "misc/WindowUtils.h"
#include "Raven_Map.h"
#include "2D/WallIntersectionTests.h"
#include <fstream>
#include <limits>
#include <cmath>

void Raven_Game::ToggleRecording()
{
	this->_recording = !this->_recording;
	if (!this->_recording)
	{
		this->SaveRecording();
	}
}

void Raven_Game::SaveRecordingTask()
{
	int set = (this->_currentSet + 1) % 2;
	std::ofstream file{ _fileName, std::ios::app };

	file << this->_dataSets[set];
}

void Raven_Game::Record(bool shot)
{
	auto possessedBot = this->PossessedBot();
	if (possessedBot && possessedBot->isAlive())
	{
		Vector2D pos = possessedBot->Pos();
		Raven_Weapon* currentWeapon = possessedBot->GetWeaponSys()->GetCurrentWeapon();
		Vector2D cursorPos = GetClientCursorPosition();

		int numEnnemies = 0,
			numAllies = 0;

		double closest = Diagonal();
		Raven_Bot* closestBot = nullptr;

		double closestWall;
		if (!FindClosestPointOfIntersectionWithWalls(pos, cursorPos, closestWall, Vector2D{}, this->m_pMap->GetWalls()))
		{
			closestWall = Diagonal();
		}

		for (auto bot : possessedBot->GetSensoryMem()->GetListOfRecentlySensedOpponents())
		{
			if (possessedBot->GetSensoryMem()->isOpponentWithinFOV(bot))
			{
				if (!possessedBot->GetTeam() || possessedBot->GetTeam() != bot->GetTeam())
				{
					numEnnemies++;
				}
				else
				{
					numAllies++;
				}

				double distance = DistToLineSegment(pos, cursorPos, bot->Pos());
				if (distance < closest)
				{
					closest = distance;
					closestBot = bot;
				}
			}
		}

		Weapon weapon = Weapon::Blaster;
		switch (currentWeapon->GetType())
		{
		case type_rail_gun:
			weapon = Weapon::RailGun;
			break;

		case type_rocket_launcher:
			weapon = Weapon::RocketLauncher;
			break;

		case type_shotgun:
			weapon = Weapon::Shotgun;
			break;
		}

		float distance = min(pos.Distance(cursorPos), closestWall);
		if (closestBot)
		{
			distance = min(distance, pos.Distance(closestBot->Pos()));
		}
		distance /= Diagonal();

		float ammo = static_cast<float>(currentWeapon->NumRoundsRemaining()) / static_cast<float>(currentWeapon->MaxRounds() ? currentWeapon->MaxRounds() : 1.0f),
			ennemies = static_cast<float>(numEnnemies) / static_cast<float>(this->m_Bots.size() - 1),
			allies = static_cast<float>(numAllies) / static_cast<float>(this->m_Bots.size() - 1),
			health = static_cast<float>(possessedBot->Health()) / static_cast<float>(possessedBot->MaxHealth());

		Input input{ distance, ammo, ennemies, allies, health, weapon };
		Output output{ shot ? 1.0f : 0.0f };

		this->_dataSets[this->_currentSet].Add(input, output);
		this->_framesSinceRecord = 0;

		if (this->_dataSets[this->_currentSet].Size() == _entriesPerWrite)
		{
			this->SaveRecording();
		}
	}
}