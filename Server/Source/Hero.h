#ifndef Hero_h__
#define Hero_h__

#include <ParabolaCore/Strings.h>
#include <ParabolaCore/Vectors.h>
using namespace pE;

class Hero{
public:
	Hero();

	/// Damage the hero
	Int16 takeHealth(int amount);

	/// Health
	float health;
	float maxHealth;
	float healthRegen;

	/// Gold
	int gold;

	/// Experience level
	int level;

	/// Dead or Alive
	bool dead;
	float respawnTime;
	float timeSinceDeath;

	/// Network id of the hero
	int id;
	int teamid;

	/// Nickname of the player
	String nick;

	/// The model of the player
	String model;

	/// Where it is
	Vec2f spawnPoint;
	Vec2f position;
	Vec2f moveDirection;
	float movementSpeed;

};

#endif // Hero_h__
