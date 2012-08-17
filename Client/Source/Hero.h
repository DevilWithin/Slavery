#ifndef Hero_h__
#define Hero_h__

#include <ParabolaCore/Vectors.h>
#include <ParabolaCore/Strings.h>
using namespace pE;

class Hero{
public:
	/// Startup the hero
	Hero();

	/// The unique identifier of the hero being controlled
	Int16 id;

	/// Current position of the hero
	Vec2f position;

	/// Nickname of the hero
	String nick;

	Vec2f direction;
	float movementSpeed;
};

#endif // Hero_h__
