#ifndef HeroNetworkController_h__
#define HeroNetworkController_h__

#include <ParabolaCore/NetworkServer.h>
#include "HeroController.h"

using namespace pE;

class Hero;

/// Gives commands to an hero from the network
class HeroNetworkController: public HeroController{
public:
	/// Startup from a server peer
	HeroNetworkController(NetworkServerPeer* peer, Hero* hero);

	/// Called when the peer has data
	void handleMessage(NetworkPacket* packet);

	NetworkServerPeer* peer;
};

#endif // HeroNetworkController_h__
