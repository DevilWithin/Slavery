#ifndef Hero_h__
#define Hero_h__

#include <ParabolaCore/Vectors.h>
#include <ParabolaCore/Strings.h>
using namespace pE;

class Hero{
public:
	/// Startup the hero
	Hero();

	/// Calculates the normalized direction
	void calculateAngleDirection();

	/// The unique identifier of the hero being controlled
	Int16 id;
	Int16 teamid;

	/// Health values
	float health;
	float maxHealth;
	bool dead;
	Int16 respawnTime;
	float respawnTimeLeft;

	/// Current position of the hero
	Vec2f position;
	/// Position we re moving to
	Vec2f targetPosition;
	bool autoMoving;

	/// Nickname of the hero
	String nick;

	/// Number of kills
	int kills;
	int deaths;

	/// Gold amount
	int gold;

	Vec2f direction;
	Vec2f angleDirection;
	float movementSpeed;
};

#endif // Hero_h__
