#include "GameSession.h"
#include "HeroNetworkController.h"

#include <iostream>
using namespace std;

GameSession::GameSession(){
	onGame = false;

	m_server.onClientConnect.connect(MAKE_SLOT_LOCAL(GameSession, clientConnect));
};

/// Wait for connections
void GameSession::startListening(){
	m_server.start(8002);
};

/// Callback when a new client connects
void GameSession::clientConnect(NetworkServerPeer* peer){
	if(onGame){
		// Already on game, rejecting client
	}
	else{

	}

	// DIRTY fix - first connection is Grimshaw, then Liryea
	String playerToSend;
	if(m_heroControllers.size() == 0){
		playerToSend = "Grimshaw"; 
	}
	else playerToSend = "Liryea";

	//
	Hero* hero = findPlayerByNickname(playerToSend);
	HeroNetworkController *heroController = new HeroNetworkController(peer, hero);
	m_heroControllers[hero] = heroController;

	// entry packet
	String pck = "01 ";
	pck += String::number(hero->id) + " ";
	pck += String::number(hero->position.x);
	pck += ",";
	pck += String::number(hero->position.y);



	peer->send(pck);
	
	
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
				m_team2[i].position += m_team2[i].moveDirection * updateStep * m_team1[i].movementSpeed;
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

/// Players ingame
int GameSession::playerCount(){
	return m_team1.size() + m_team2.size();
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
	for(unsigned int i = 0; i < m_team1.size(); i++){
		if(!m_team1[i].dead){
			m_team1[i].health += m_team1[i].healthRegen;
			if(m_team1[i].health >= m_team1[i].maxHealth) m_team1[i].health = m_team1[i].maxHealth;
		}else{
			m_team1[i].timeSinceDeath += 1.f;
			if(m_team1[i].timeSinceDeath >= m_team1[i].respawnTime){
				m_team1[i].health = m_team1[i].maxHealth;
				m_team1[i].dead = false;
				m_team1[i].position = m_team1[i].spawnPoint;
				cout<<"Respawn: "<<m_team1[i].nick<<endl;
			}
		}
	}

	for(unsigned int i = 0; i < m_team2.size(); i++){
		if(!m_team2[i].dead){
			m_team2[i].health += m_team2[i].healthRegen;
			if(m_team2[i].health >= m_team2[i].maxHealth) m_team2[i].health = m_team2[i].maxHealth;
		}
		else{
			m_team2[i].timeSinceDeath += 1.f;
			if(m_team2[i].timeSinceDeath >= m_team2[i].respawnTime){
				m_team2[i].health = m_team2[i].maxHealth;
				m_team2[i].dead = false;
				m_team2[i].position = m_team2[i].spawnPoint;
				cout<<"Respawn: "<<m_team2[i].nick<<endl;
			}
		}
	}
};

void GameSession::addHeroTeam1(Hero hero){
	hero.id = 1;
	hero.position = Vec2f(0,0);
	m_team1.push_back(hero);	
};

void GameSession::addHeroTeam2(Hero hero){
	hero.id = 2;
	hero.position = Vec2f(1000, 0);
	m_team2.push_back(hero);	
};


