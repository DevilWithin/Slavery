#ifndef HeroController_h__
#define HeroController_h__

#include "Hero.h"

enum ControllerTypes{
	NETWORK_CONTROLLER = 0,
	AI_CONTROLLER
};

class GameSession;

class HeroController{
public:
	HeroController(Hero* h, GameSession* s){
		this->hero = h;
		this->session = s;
	}

	/// Acknowledges a direction update
	void informDirectionUpdate();

	virtual void onThink(){}

	Hero* hero;
	GameSession* session;

	int m_type;
};

#endif // HeroController_h__
