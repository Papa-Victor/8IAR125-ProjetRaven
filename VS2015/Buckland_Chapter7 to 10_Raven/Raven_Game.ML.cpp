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
#include "Debug/DebugConsole.h"

#include "MLBot.h"
#include <armadillo/armadillo.h>
#include <mlpack/core/data/load.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
using namespace std;

void Raven_Game::Train()
{
	if (!this->_perceptron)
	{
		this->_perceptron = new mlpack::perceptron::Perceptron<mlpack::perceptron::SimpleWeightUpdate, mlpack::perceptron::RandomInitialization>;
	}
	this->_perceptron->MaxIterations() = IterationsPerLog;

	// TODO LOOK INTO THIS : There seems to be a problem between the armadillo/mlpack heaps and ours... Need to preallocate everything.
	ifstream f{ TrainingFileName };
	std::string line_string;
	int rows = 0;
	while (getline(f, line_string) && line_string.size())
	{
		++rows;
	}

	arma::mat data{ arma::SizeMat{ static_cast<arma::uword>(rows), 10 } };
	mlpack::data::Load(TrainingFileName, data, false, false);
	data = arma::trans(data);
	data = arma::shuffle(data, 1);

	int trainingSize = data.n_cols * 0.9;
	auto subTrainingData = data.submat(0, 0, 8, trainingSize);
	auto subTrainingLabels = data.submat(9, 0, 9, trainingSize);
	auto subTestingData = data.submat(0, trainingSize + 1, 8, data.n_cols - 1);
	auto subTestingLabels = data.submat(9, trainingSize + 1, 9, data.n_cols - 1);

	arma::mat
		trainingData{ arma::SizeMat{subTrainingData.n_rows, subTrainingData.n_cols} },
		trainingLabels{ arma::SizeMat{subTrainingLabels.n_rows, subTrainingLabels.n_cols} },
		testingData{ arma::SizeMat{subTestingData.n_rows, subTestingData.n_cols} },
		testingLabels{ arma::SizeMat{subTestingLabels.n_rows, subTestingLabels.n_cols} };

	arma::subview<double>::extract(trainingData, subTrainingData);
	arma::subview<double>::extract(trainingLabels, subTrainingLabels);
	arma::subview<double>::extract(testingData, subTestingData);
	arma::subview<double>::extract(testingLabels, subTestingLabels);

	float successRate = 0.0f;
	for (int i = 0; i < MaxIterations && successRate < MinimumSuccessRate; i += IterationsPerLog)
	{
		this->_perceptron->Train(trainingData, arma::conv_to<arma::Row<size_t>>::from(trainingLabels.row(0)), 2);

		arma::u64_rowvec predictedLabels{ arma::SizeMat{ 1, testingData.n_cols } };
		this->_perceptron->Classify(testingData, predictedLabels);

		int equalOccurences = 0;
		for (int i = 0; i < predictedLabels.n_cols; ++i)
		{
			if (predictedLabels(0, i) == testingLabels(0, i))
			{
				equalOccurences++;
			}
		}

		successRate = static_cast<float>(equalOccurences) / static_cast<float>(predictedLabels.n_cols);
		debug_con << "(" << i << ") " << (successRate * 100) << "% success rate.\n";
		debug_con.flush();
	}

	ofstream perceptronFile{ PerceptronFileName, ios::trunc };
	boost::archive::text_oarchive toa{ perceptronFile };
	toa << this->_perceptron;
}

void Raven_Game::AddMLBot()
{
	if (this->_perceptron)
	{
		MLBot* botToAdd = new MLBot(this, Vector2D{}, this->_perceptron);
		this->_mlBots.push(botToAdd);
		this->AddBot(botToAdd);
	}
}

void Raven_Game::RemoveMLBot()
{
	if (this->_mlBots.size())
	{
		MLBot* latestBot = this->_mlBots.top();
		this->_mlBots.pop();
		this->_botToRemove = latestBot;
	}
}

void Raven_Game::ToggleRecording()
{
	this->_recording = !this->_recording;
	if (!this->_recording)
	{
		this->SaveRecording();
	}
}

Input Raven_Game::Snapshot(Raven_Bot* targetBot) const
{
	if (!targetBot->isAlive())
	{
		throw std::exception{ "There is no valid bot from which to get a snapshot." };
	}

	Vector2D pos = targetBot->Pos();
	Raven_Weapon* currentWeapon = targetBot->GetWeaponSys()->GetCurrentWeapon();
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

	for (auto bot : targetBot->GetSensoryMem()->GetListOfRecentlySensedOpponents())
	{
		if (targetBot->GetSensoryMem()->isOpponentWithinFOV(bot))
		{
			if (!targetBot->GetTeam() || targetBot->GetTeam() != bot->GetTeam())
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

	double distance = std::min(pos.Distance(cursorPos), closestWall);
	if (closestBot)
	{
		distance = std::min(distance, pos.Distance(closestBot->Pos()));
	}
	distance /= Diagonal();

	float ammo = static_cast<float>(currentWeapon->NumRoundsRemaining()) / static_cast<float>(currentWeapon->MaxRounds() ? currentWeapon->MaxRounds() : 1.0f),
		ennemies = static_cast<float>(numEnnemies) / static_cast<float>(this->m_Bots.size() - 1),
		allies = static_cast<float>(numAllies) / static_cast<float>(this->m_Bots.size() - 1),
		health = static_cast<float>(targetBot->Health()) / static_cast<float>(targetBot->MaxHealth());

	return Input{ static_cast<float>(distance), ammo, ennemies, allies, health, weapon };
}

void Raven_Game::SaveRecordingTask()
{
	int set = (this->_currentSet + 1) % 2;
	std::ofstream file{ TrainingFileName, std::ios::app };

	file << this->_dataSets[set];
}

void Raven_Game::Record(bool shot)
{
	if (this->PossessedBot())
	{
		try
		{
			Input snapshot = this->Snapshot(this->PossessedBot());
			Output output{ shot ? 1.0f : 0.0f };

			this->_dataSets[this->_currentSet].Add(snapshot, output);
			this->_framesSinceRecord = 0;

			if (this->_dataSets[this->_currentSet].Size() == _entriesPerWrite)
			{
				this->SaveRecording();
			}

		}
		catch (const std::exception&)
		{
		}
	}
}