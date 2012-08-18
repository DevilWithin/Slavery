#ifndef ClientApp_h__
#define ClientApp_h__

#include <ParabolaCore/GameCore.h>
#include <ParabolaCore/Sprite.h>
#include <ParabolaCore/NetworkClient.h>
#include <ParabolaCore/ScopedFile.h>
#include <ParabolaCore/TextStream.h>
#include <ParabolaCore/Text.h>
#include <ParabolaCore/RocketContext.h>
#include <Rocket/Controls.h>

using namespace pE;

#include <vector>
#include "Hero.h"

namespace Client{
	enum ClientGeneratedPacket{
		AUTH_REQUEST = 0,
		TEST,
		DROP_BOMB,
		HERO_DIRECTION_REQUEST,
		CHAT_MESSAGE
	};
}
namespace Server{
	enum ServerGeneratedPacket{
		AUTH_SUCESSFULL = 0,
		GLOBAL_CHAT_MESSAGE,
		HERO_INFO,
		HERO_IDENTITY,
		HERO_DIRECTION_UPDATE,
		HERO_DAMAGE,
		HERO_RECOVER,
		HERO_RESPAWN,
		HERO_DEATH
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

	/// Called when rocket sends events
	void rocketEvent(String event);

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
	RocketDocument* m_chat;
	View m_view;
};

#endif // ClientApp_h__
