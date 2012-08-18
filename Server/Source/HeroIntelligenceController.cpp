#include "HeroIntelligenceController.h"
#include "GameSession.h"

#include <iostream>
using namespace std;

/// Called to update AI
void HeroIntelligenceController::onThink(){
	cout<< hero->nick << " is thinking."<<endl;

	/*if(hero->position.x < 300){
		hero->moveDirection.x = 1;
	}
	if(hero->position.x > 1000){
		hero->moveDirection.x = -1;
	}*/
	
	Hero* h;
	if(hero->teamid == 1)
		h = session->findPlayerByNickname("Grimshaw");
	else
		h = session->findPlayerByNickname("Liryea");
	if(h){
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

	informDirectionUpdate();

	

};