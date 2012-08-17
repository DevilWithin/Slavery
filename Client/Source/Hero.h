#ifndef Hero_h__
#define Hero_h__

#include <ParabolaCore/Vectors.h>
using namespace pE;

class Hero{
public:

	/// The unique identifier of the hero being controlled
	int id;

	/// Current position of the hero
	Vec2f position;
};

#endif // Hero_h__
