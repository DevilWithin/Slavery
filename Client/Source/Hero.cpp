#include "Hero.h"

/// Startup the hero
Hero::Hero(){
	direction = Vec2f(0.f,0.f);
	movementSpeed = 30.f;
	kills = 0;
	deaths = 0;
};