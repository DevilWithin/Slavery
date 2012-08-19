#ifndef HeroIntelligenceController_h__
#define HeroIntelligenceController_h__

#include "HeroController.h"

#include <ParabolaCore/Clock.h>
using namespace pE;

class HeroIntelligenceController : public HeroController{
public:
	HeroIntelligenceController(Hero* hero, GameSession* session) : HeroController(hero, session){

	}

	/// Called to update AI
	void onThink();

	Clock c;

};


#endif // HeroIntelligenceController_h__
