#include "HeroNetworkController.h"
#include "Hero.h"

#include <iostream>
using namespace std;

/// Startup from a server peer
HeroNetworkController::HeroNetworkController(NetworkServerPeer* peer, Hero* hero){
	this->hero = hero;
	this->peer = peer;

	peer->onDataReceived.connect(MAKE_SLOT_LOCAL(HeroNetworkController, handleMessage));
};

/// Called when the peer has data
void HeroNetworkController::handleMessage(NetworkPacket* packet){

};