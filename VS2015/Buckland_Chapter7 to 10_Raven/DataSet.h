#ifndef DATA_SET_H
#define DATA_SET_H

#include <memory>
#include <iostream>
#include <vector>

enum class Weapon : unsigned char
{
	Blaster,
	Shotgun,
	RocketLauncher,
	RailGun
};

struct Input
{
	Input() 
		: _distance(0.0f)
		, _ammo(0.0f)
		, _ennemies(0.0f)
		, _allies(0.0f)
		, _health(0.0f)
		, _weaponBlaster(0.0f)
		, _weaponShotgun(0.0f)
		, _weaponRocketLauncher(0.0f)
		, _weaponRailGun(0.0f)
	{
	}

	Input(float distance, float ammo, float ennemies, float allies, float health, Weapon weapon) 
		: Input()
	{
		this->_distance = distance;
		this->_ammo = ammo;
		this->_ennemies = ennemies;
		this->_allies = allies;
		this->_health = health;

		switch (weapon)
		{
		case Weapon::Shotgun:
			this->_weaponShotgun = 1.0f;
			break;

		case Weapon::RocketLauncher:
			this->_weaponRocketLauncher = 1.0f;
			break;

		case Weapon::RailGun:
			this->_weaponRailGun = 1.0f;
			break;

		default:
			this->_weaponBlaster = 1.0f;
			break;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Input& input);
	friend std::istream& operator>>(std::istream& is, Input& input);

	union
	{
		float values[9];
		struct
		{
			float _distance;
			float _ammo;
			float _ennemies;
			float _allies;
			float _health;
			float _weaponBlaster;
			float _weaponShotgun;
			float _weaponRocketLauncher;
			float _weaponRailGun;
		};
	};
};

struct Output
{
	friend struct DataSet;

	Output()
		: _shouldFire(0.0f)
	{
	}

	Output(float shouldFire)
		: _shouldFire(shouldFire)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Output& output);
	friend std::istream& operator>>(std::istream& is, Output& output);

private:
	float _shouldFire;
};

struct DataSet
{
	void Add(const Input& input, const Output& output);

	const std::vector<float>& Inputs();
	const std::vector<float>& Outputs();

	size_t Size() const { return this->_inputs.size(); }

	friend std::ostream& operator<<(std::ostream& os, DataSet& dataSet);
	friend std::istream& operator>>(std::istream& is, DataSet& dataSet);

private:
	bool _dirty = false;
	std::vector<float> _floatInputs;
	std::vector<float> _floatOutputs;

	std::vector<Input> _inputs;
	std::vector<Output> _outputs;
};

#endif