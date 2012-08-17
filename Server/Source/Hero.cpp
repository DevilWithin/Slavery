#include "Hero.h"

Hero::Hero(){
	health = 1;
	maxHealth = 100;
	healthRegen = 5;
	dead = false;
	gold = 450;
	level = 1;
	moveDirection = Vec2f(0,0);
	movementSpeed = 30.f;
	model = "Default";
	spawnPoint = Vec2f(300,300);
	respawnTime = 5.f;
	timeSinceDeath = 0.f;
};