#ifndef ML_BOT_H
#define ML_BOT_H

#include "Raven_Bot.h"
#include "Raven_WeaponSystem.h"

#include <mlpack/methods/perceptron/perceptron.hpp>


class MLBot : public Raven_Bot
{
	mlpack::perceptron::Perceptron<mlpack::perceptron::SimpleWeightUpdate, mlpack::perceptron::RandomInitialization>* _firingBehaviour;

protected:
	void TakeAimAndShoot() const override 
	{
		this->GetWeaponSys()->TakeAimAndShoot(this->_firingBehaviour);
	}

public:
	MLBot(Raven_Game* world, Vector2D pos, mlpack::perceptron::Perceptron<mlpack::perceptron::SimpleWeightUpdate, mlpack::perceptron::RandomInitialization>* firingBehavior)
		: Raven_Bot(world, pos)
		, _firingBehaviour{ firingBehavior }
	{
		if (!firingBehavior)
		{
			throw std::logic_error{ "The given firing behavior cannot be null." };
		}
	}
};

#endif