#include "Raven_WeaponSystem.h"

#include "Raven_Game.h"
#include "armory/Raven_Weapon.h"
#include "Debug/DebugConsole.h"
#include <armadillo/armadillo.h>

void Raven_WeaponSystem::TakeAimAndShoot(mlpack::perceptron::Perceptron<mlpack::perceptron::SimpleWeightUpdate, mlpack::perceptron::RandomInitialization>* firingBehavior) const
{
	//aim the weapon only if the current target is shootable or if it has only
	//very recently gone out of view (this latter condition is to ensure the 
	//weapon is aimed at the target even if it temporarily dodges behind a wall
	//or other cover)
	if (m_pOwner->GetTargetBot() && (firingBehavior || m_pOwner->GetTargetSys()->isTargetShootable() || (m_pOwner->GetTargetSys()->GetTimeTargetHasBeenOutOfView() < m_dAimPersistance)))
	{
		//the position the weapon will be aimed at
		Vector2D AimingPos = m_pOwner->GetTargetBot()->Pos();

		//if the current weapon is not an instant hit type gun the target position
		//must be adjusted to take into account the predicted movement of the 
		//target
		if (GetCurrentWeapon()->GetType() == type_rocket_launcher ||
			GetCurrentWeapon()->GetType() == type_blaster)
		{
			AimingPos = PredictFuturePositionOfTarget();
		}

		AddNoiseToAim(AimingPos);

		if (m_pOwner->RotateFacingTowardPosition(AimingPos))
		{
			if (firingBehavior)
			{
				try
				{
					Input snapshot = this->m_pOwner->GetWorld()->Snapshot(this->m_pOwner);

					arma::mat input{ arma::SizeMat{ 9, 1 } };
					for (int i = 0; i < 9; ++i)
					{
						input(i, 0) = snapshot.values[i];
					}

					arma::u64_rowvec predictedLabel{ arma::SizeMat{ 1, 1 } };
					firingBehavior->Classify(input, predictedLabel);

					if (predictedLabel(0, 0))
					{
						GetCurrentWeapon()->ShootAt(AimingPos);
					}
				}
				catch (const std::exception&)
				{
				}
			}
			else if (m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible() > m_dReactionTime &&
				((GetCurrentWeapon()->GetType() != type_rocket_launcher && GetCurrentWeapon()->GetType() != type_blaster) || m_pOwner->hasLOSto(AimingPos)))
			{
				GetCurrentWeapon()->ShootAt(AimingPos);
			}
		}
	}

	//no target to shoot at so rotate facing to be parallel with the bot's
	//heading direction
	else
	{
		m_pOwner->RotateFacingTowardPosition(m_pOwner->Pos() + m_pOwner->Heading());
	}
}