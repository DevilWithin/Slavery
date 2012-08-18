#ifndef HeroIntelligenceController_h__
#define HeroIntelligenceController_h__

#include "HeroController.h"

class HeroIntelligenceController : public HeroController{
public:
	HeroIntelligenceController(Hero* hero, GameSession* session) : HeroController(hero, session){

	}

	/// Called to update AI
	void onThink();
};


#endif // HeroIntelligenceController_h__
