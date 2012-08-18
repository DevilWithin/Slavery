#ifndef ClientApp_h__
#define ClientApp_h__

#include <ParabolaCore/GameCore.h>
#include <ParabolaCore/Sprite.h>
#include <ParabolaCore/NetworkClient.h>
#include <ParabolaCore/ScopedFile.h>
#include <ParabolaCore/TextStream.h>
#include <ParabolaCore/Text.h>
#include <ParabolaCore/RocketContext.h>

using namespace pE;

#include <vector>
#include "Hero.h"

namespace Client{
	enum ClientGeneratedPacket{
		AUTH_REQUEST = 0,
		TEST,
		DROP_BOMB,
		HERO_DIRECTION_REQUEST
	};
}
namespace Server{
	enum ServerGeneratedPacket{
		AUTH_SUCESSFULL = 0,
		HERO_INFO,
		HERO_IDENTITY,
		HERO_DIRECTION_UPDATE,
		HERO_DAMAGE,
		HERO_RECOVER
	};
};



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

	/// Find a hero by its id
	Hero* getHeroById(int id);

	/// Rendering
	Renderer *m_renderer;

	/// Networking
	NetworkClient m_client;

	/// Heros in the game
	std::vector<Hero*> m_heroList;
	Hero* m_myHero;

	/// Rendering
	RocketContext* m_ui;
	View m_view;
};

#endif // ClientApp_h__
