#include "HeroController.h"
#include "ParabolaCore/NetworkPacket.h"
#include "GameSession.h"

#include <iostream>
using namespace std;

/// Acknowledges a direction update
void HeroController::informDirectionUpdate(){
	//tell the others 
	Packet pck2;
	pck2 << (Uint32)Server::HERO_DIRECTION_UPDATE;
	pck2 << (Int16)hero->id;
	pck2 << hero->moveDirection;

	for(int i = 0; i < session->m_networkControllers.size(); i++){
		HeroNetworkController* controller = session->m_networkControllers[i];
		session->m_server.send(pck2);
	}
};

/// Says something in the global chat
void HeroController::say(String message){
	Packet pck;
	pck << (Uint32)Server::GLOBAL_CHAT_MESSAGE;
	pck << (Int16)hero->id;
	pck << message;
	session->m_server.send(pck);

	cout<< hero->nick <<" said: "<<message<<endl;
};