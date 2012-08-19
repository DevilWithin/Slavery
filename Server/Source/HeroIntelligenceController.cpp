#include "HeroIntelligenceController.h"
#include "GameSession.h"

#include <iostream>
using namespace std;

/// Called to update AI
void HeroIntelligenceController::onThink(){

	const char* strs[] = {"Nothing happens around here..", "Im bored..", "Im so sick of you.", "Why don't you stop bothering me?", "Hello, let me sing you the song of my people", "Have you accepted Jesus Christ as your lord and savior?"};

	if(c.getElapsedTime().asSeconds() > hero->nick.size()){
		say(strs[Math::randomInt(0,5)]);
		c.reset();
	}
	
	Hero* h;
	if(hero->teamid == 1)
		h = session->findPlayerByNickname("Grimshaw");
	else
		h = session->findPlayerByNickname("Liryea");
	if(h){
		if(Math::distance(h->position, hero->position) < 100){
			if(hero->moveDirection != Vec2f(0,0))
				say("Caught you");

			hero->moveDirection = Vec2f(0,0);
			
		}
		else{
			if(h->position.x > hero->position.x){
				hero->moveDirection.x = 1;
			}
			if(h->position.x < hero->position.x){
				hero->moveDirection.x = -1;
			}
			if(h->position.y > hero->position.y){
				hero->moveDirection.y = 1;
			}
			if(h->position.y < hero->position.y){
				hero->moveDirection.y = -1;
			}
		}		
	}

	informDirectionUpdate();

	

};