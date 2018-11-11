#ifndef TRIGGER_TEAM_WEAPON_GIVER_H
#define TRIGGER_TEAM_WEAPON_GIVER_H

#include "Triggers/Trigger.h"
#include "Raven_Bot.h"
#include "Team.h"

class Trigger_TeamWeaponGiver : public Trigger<Raven_Bot>
{
private:
	const double m_radius = 7;

	Team* m_Team;

	//vrtex buffers for rocket shape
	std::vector<Vector2D>         m_vecRLVB;
	std::vector<Vector2D>         m_vecRLVBTrans;

public:
	Trigger_TeamWeaponGiver(Vector2D pos, Raven_Bot* pBot);
	~Trigger_TeamWeaponGiver();

	//if triggered, this trigger will call the PickupWeapon method of the
	//bot. PickupWeapon will instantiate a weapon of the appropriate type.
	void Try(Raven_Bot* pBot);

	void Update(){}

	//draws a symbol representing the weapon type at the trigger's location
	void Render();
};
#endif // !TRIGGER_TEAM_WEAPON_GIVER_H