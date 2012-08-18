#include "GameSession.h"
#include "HeroNetworkController.h"

#include <iostream>
using namespace std;

GameSession::GameSession(){
	onGame = false;
	m_networkedControllers = 0;
	m_idCounter = 0;
	m_server.onClientConnect.connect(MAKE_SLOT_LOCAL(GameSession, clientConnect));
	m_server.onDataReceived.connect(MAKE_SLOT_LOCAL(GameSession, clientData));
};

/// Wait for connections
void GameSession::startListening(){
	m_server.start(8002);
};

/// Callback when a new client connects
void GameSession::clientConnect(NetworkServerPeer* peer){
	m_neutralPeers.push_back(peer);	
};

/// Callback when data is received
void GameSession::clientData(NetworkServerPeer* peer, NetworkPacket* packet){
	Packet p = packet->getData();
	Uint32 packet_id;
	p >> packet_id;

	switch(packet_id){
	case Client::AUTH_REQUEST:
		{
			String s;
			p >> s;
			cout<<"Client auth accepted: "<<s<<endl;

			Packet pck;
			pck << (Uint32)Server::AUTH_SUCESSFULL;
			peer->send(pck);
			

			Hero* hero = findPlayerByNickname(s);
			if(!hero)return;

			HeroController *controller = new HeroNetworkController(peer, hero);
			peer->setUserData(controller);
			m_networkedControllers++;
			m_networkControllers.push_back((HeroNetworkController*)controller);
			m_heroControllers[hero] = controller;

			// inform about online players
			for(int i = 0; i < playerCount(); i++){
				Hero *h = getHero(i);
				Packet info_pck;
				info_pck << (Uint32)Server::HERO_INFO;
				info_pck << (Int16)h->id;
				info_pck << h->position;
				info_pck << h->nick;
				info_pck << h->movementSpeed;
				info_pck << h->health;
				info_pck << h->maxHealth;
				peer->send(info_pck);

				if(hero == h){
					Packet self_pck;
					self_pck << (Uint32)Server::HERO_IDENTITY;
					self_pck << (Int16)h->id;			
					peer->send(self_pck);
				}
			}

		}break;
	case Client::TEST:
		{
			String message;
			p >> message;

			cout<<"Test Packet: "<<message<<endl;

		}break;
	case Client::CHAT_MESSAGE:
		{
			String message;
			p >> message;

			Packet pck;
			pck << (Uint32)Server::GLOBAL_CHAT_MESSAGE;
			pck << (Int16)((HeroController*)peer->getUserData())->hero->id;
			pck << message;
			m_server.send(pck);

			cout<< ((HeroController*)peer->getUserData())->hero->nick <<" said: "<<message<<endl;

		}break;
	case Client::DROP_BOMB:
		{
			Vec2f hit;
			p >> hit;

			for(unsigned int i = 0; i < playerCount(); i++){
				if(Math::distance(getHero(i)->position, hit) < 20 && !getHero(i)->dead){
					Int16 taken = getHero(i)->takeHealth(20);
					Int16 sourceId = ((HeroController*)peer->getUserData())->hero->id;

					Packet pck;
					pck << (Uint32)Server::HERO_DAMAGE;
					pck << (Int16)getHero(i)->id;
					pck << (Int16)taken;
					m_server.send(pck);

					if(getHero(i)->health == 0){
						getHero(i)->dead = true;
						getHero(i)->timeSinceDeath = 0.f;

						Packet pck2;
						pck2 << (Uint32)Server::HERO_DEATH;
						pck2 << (Int16)getHero(i)->id; // Who died
						pck2 << (Int16)getHero(i)->respawnTime; //How long it will take
						pck2 << (Int16)sourceId; // Who killed
						m_server.send(pck2);
					}
				}
			}

		}break;
	case Client::HERO_DIRECTION_REQUEST:
		{
			Int16 id;
			p >> id;
			Hero* hero = findPlayerById(id);
			if(hero){
				// update
				p >> hero->moveDirection;

				//tell the others
				Packet pck2;
				pck2 << (Uint32)Server::HERO_DIRECTION_UPDATE;
				pck2 << (Int16)hero->id;
				pck2 << hero->moveDirection;

				for(int i = 0; i < m_networkControllers.size(); i++){
					HeroNetworkController* controller = m_networkControllers[i];
					controller->peer->send(pck2);
				}
			}

		}break;

	}

	//cout<<"DATA: "<<p.getDataSize()<<endl;
};

/// Starts the game
void GameSession::start(){
	onGame = true;
	m_gameTime.reset();
	lastUpdate = 0.f;
	gatheredTime = 0.f;
	updateStep = 1.f / 60.f;
};

/// Updates the game state
void GameSession::update(){
	String cmd = "TITLE " + String::number(m_gameTime.getElapsedTime().asSeconds());
	//system(cmd.c_str());

	float currentTime = m_gameTime.getElapsedTime().asSeconds();
	float elapsedTime = currentTime - lastUpdate;

	m_server.update(1);

	if(onGame){
		// Secondly updates
		if(m_oneSecondTimer.getElapsedTime().asSeconds() > 1){
			updateSecondly();
			m_oneSecondTimer.reset();
		}

		gatheredTime += elapsedTime;
		while(gatheredTime >= updateStep){

			// Movement updates
			for(unsigned int i = 0; i < m_team1.size(); i++){
				m_team1[i].position += m_team1[i].moveDirection * updateStep * m_team1[i].movementSpeed;
			}
			for(unsigned int i = 0; i < m_team2.size(); i++){
				m_team2[i].position += m_team2[i].moveDirection * updateStep * m_team2[i].movementSpeed;
			}

			gatheredTime -= updateStep;
		}
	}


	if(m_test.getElapsedTime().asSeconds() > 1){
		logPlayer("Grimshaw");
		logPlayer("Liryea");
		cout<<endl;
		m_test.reset();
	}

	lastUpdate = currentTime;
};

/// Kills the player if it is found
void GameSession::killPlayer(String nick){
	Hero* hero = findPlayerByNickname(nick);
	if(hero){
		hero->health = 0.f;
		hero->dead = true;
		hero->timeSinceDeath = 0.f;
	}
};

/// Number of network controlled heros
int GameSession::getNetworkPlayerCount(){
	return m_networkedControllers;
};

/// Get a network controller by index
HeroNetworkController* GameSession::getNetworkPlayer(int index){
	int j = 0;
	for(std::map<Hero*, HeroController*>::iterator it = m_heroControllers.begin(); it != m_heroControllers.end(); it++){
		if((*it).second->m_type == NETWORK_CONTROLLER){			
			if(j == index){
				return (HeroNetworkController*)(*it).second;
			}
			j++;
		}
	}
	return NULL;
};


/// Players ingame
int GameSession::playerCount(){
	return m_team1.size() + m_team2.size();
};

/// Get hero by index
Hero* GameSession::getHero(int index){
	if(index < m_team1.size()){
		return &m_team1[index];
	}
	else{
		return &m_team2[index-m_team1.size()];
	}
};

/// Finds a player by its id
Hero* GameSession::findPlayerById(Int16 id){	
	for(unsigned int i = 0; i < m_team1.size(); i++){
		if(m_team1[i].id == id){
			return &m_team1[i];
		}
	}
	for(unsigned int i = 0; i < m_team2.size(); i++){
		if(m_team2[i].id == id){
			return &m_team2[i];
		}
	}

	return NULL;
};

/// Finds a player by nickname, in both teams
Hero* GameSession::findPlayerByNickname(String nick){
	for(unsigned int i = 0; i < m_team1.size(); i++){
		if(m_team1[i].nick == nick){
			return &m_team1[i];
		}
	}
	for(unsigned int i = 0; i < m_team2.size(); i++){
		if(m_team2[i].nick == nick){
			return &m_team2[i];
		}
	}

	return NULL;
};

void GameSession::logPlayer(String name){
	Hero* hero = findPlayerByNickname(name);
	if(hero){
		String death;
		if(hero->dead)death = ":(";
		else death = ":)";
		cout<< death <<" "<< name<<"["<< hero->level <<"] ("<<String::number((float)hero->position.x)<<","<<String::number(hero->position.y)<<") hp: "<< hero->health << endl;
	}	
};

/// All updates that ocurr every 1 second
void GameSession::updateSecondly(){
	for(unsigned int i = 0; i < playerCount(); i++){
		Hero* hero = getHero(i);

		if(!hero->dead){
			Int16 regen = hero->healthRegen;

			hero->health += hero->healthRegen;
			if(hero->health >= hero->maxHealth){
				regen = regen - (hero->health - hero->maxHealth);
				hero->health = hero->maxHealth;
			}

			Packet pck;
			pck << (Uint32)Server::HERO_RECOVER;
			pck << (Int16)hero->id;
			pck << regen;

			m_server.send(pck);

		}else{
			hero->timeSinceDeath += 1.f;
			if(hero->timeSinceDeath >= hero->respawnTime){
				hero->health = hero->maxHealth;
				hero->dead = false;
				hero->position = hero->spawnPoint;
				cout<<"Respawn: "<<hero->nick<<endl;


				Packet pck;
				pck << (Uint32)Server::HERO_RESPAWN;
				pck << (Int16)hero->id;
				pck << hero->position;
				pck << hero->health;
				m_server.send(pck);
			}
		}
	}
};

void GameSession::addHeroTeam1(Hero hero){
	hero.id = ++m_idCounter;
	hero.position = Vec2f(200,600);
	hero.spawnPoint = Vec2f(100 + m_idCounter*100,100 + m_idCounter*100);
	m_team1.push_back(hero);	
};

void GameSession::addHeroTeam2(Hero hero){
	hero.id = ++m_idCounter;
	hero.position = Vec2f(1000 + m_idCounter*100, 500);
	hero.spawnPoint = Vec2f(1000 + m_idCounter*100,1000 + m_idCounter*50);
	m_team2.push_back(hero);	
};


