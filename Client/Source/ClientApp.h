#ifndef ClientApp_h__
#define ClientApp_h__

#include <ParabolaCore/GameCore.h>
#include <ParabolaCore/Sprite.h>
#include <ParabolaCore/NetworkClient.h>
#include <ParabolaCore/ScopedFile.h>
#include <ParabolaCore/TextStream.h>

using namespace pE;

#include <vector>
#include "Hero.h"

class ClientApp : public GameCore{
public:
	/// Startup
	ClientApp();

	/// When game is created
	void onCreate();

	/// Updating the game
	void onUpdate(Time time);

	/// Game events
	void onEvent(Event &event);

	/// Rendering
	void onRender();

	/// Called when there is new data to read
	void onClientData(NetworkClient* , NetworkPacket* packet);

	/// Called when the client connected
	void onClientConnect(NetworkClient* client);

	/// Rendering
	Renderer *m_renderer;

	/// Networking
	NetworkClient m_client;

	/// Heros in the game
	std::vector<Hero*> m_heroList;
	Hero* m_myHero;
};

#endif // ClientApp_h__
