#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <ParabolaCore/Strings.h>
#include <ParabolaCore/NetworkServer.h>
#include <ParabolaCore/Clock.h>
#include <ParabolaCore/Vectors.h>
using namespace pE;
#include <vector>
#include <map>
#include "Hero.h"
#include "HeroController.h"

namespace Client{
	enum ClientGeneratedPacket{
		AUTH_REQUEST = 0
	};
}

class GameSession{
public:
	/// Startup
	GameSession();

	/// Wait for connections
	void startListening();

	/// Callback when a new client connects
	void clientConnect(NetworkServerPeer* peer);

	/// Callback when data is received
	void clientData(NetworkServerPeer* peer, NetworkPacket* packet);

	/// Starts the game
	void start();

	/// Updates the game state
	void update();

	/// All updates that ocurr every 1 second
	void updateSecondly();

	void addHeroTeam1(Hero hero);
	void addHeroTeam2(Hero hero);

	/// Players ingame
	int playerCount();

	/// Kills the player if it is found
	void killPlayer(String nick);

	/// Finds a player by nickname, in both teams
	Hero* findPlayerByNickname(String nick);

	void logPlayer(String name);

	bool onGame;

	Clock m_gameTime;
	float lastUpdate;
	float gatheredTime;
	float updateStep;
	Clock m_test;
	Clock m_oneSecondTimer;

	std::vector<Hero> m_team1;
	std::vector<Hero> m_team2;

	// Networking
	NetworkServer m_server;

	/// Just connected peers, that are nothing yet
	std::vector<NetworkServerPeer*> m_neutralPeers;

	/// Each Hero must have a controller, AI or Network, otherwise it is just a dummy
	std::map<Hero*, HeroController*> m_heroControllers;
};

#endif