#include <iostream>
#include <ParabolaCore/NetworkServer.h>
using namespace pE;

#include "GameSession.h"
#include "Hero.h"

int main(int argc, char **argv){

	system("TITLE ServerApp");
	enet_initialize();

	GameSession game;

	game.startListening();

	Hero grimshaw;
	grimshaw.nick = "Grimshaw";
	grimshaw.maxHealth = 2000;
	grimshaw.healthRegen = 50;
	grimshaw.movementSpeed = 500;
	game.addHeroTeam1(grimshaw);

	Hero toto;
	toto.nick = "Mr. Chuckles";
	game.addHeroTeam1(toto);
	

	Hero liryea;
	liryea.nick = "Liryea";	
	liryea.maxHealth = 200;
	liryea.healthRegen = 3;
	liryea.respawnTime = 20;
	game.addHeroTeam2(liryea);

	Hero toto2;
	toto2.nick = "Liryea's Slave";
	game.addHeroTeam2(toto2);

	//bind ai
	Hero* h = game.findPlayerByNickname("Mr. Chuckles");
	if(h)
		game.m_heroControllers[h] = new HeroIntelligenceController(h, &game);

	Hero* h2 = game.findPlayerByNickname("Liryea's Slave");
	if(h2)
		game.m_heroControllers[h2] = new HeroIntelligenceController(h2, &game);

	game.start();

	
	while(1 == 1){
		game.update();
	}

	return 0;
}