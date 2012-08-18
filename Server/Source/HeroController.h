#ifndef HeroController_h__
#define HeroController_h__

#include "Hero.h"

enum ControllerTypes{
	NETWORK_CONTROLLER = 0,
	AI_CONTROLLER
};

class HeroController{
public:

	Hero* hero;

	int m_type;
};

#endif // HeroController_h__
