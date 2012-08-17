#include <ParabolaCore/ParabolaMain.h>
#include <ParabolaCore/Engine.h>
#include <ParabolaCore/Network.h>
#include "ClientApp.h"
using namespace pE;

Engine *sdk;
void applicationStartup(){
	enet_initialize();
	sdk = new Engine();
	sdk->create();
	sdk->getGameManager().addGameForExecution(new ClientApp());
}

void applicationUpdate(){
	sdk->update();
}

void applicationCleanup(){

}