#include "ClientApp.h"

#include <iostream>
using namespace std;

/// Startup
ClientApp::ClientApp(){
	setWindowTitle("Moba Client");

	m_client.onDataReceived.connect(MAKE_SLOT_LOCAL(ClientApp, onClientData));
	m_client.onConnected.connect(MAKE_SLOT_LOCAL(ClientApp, onClientConnect));

	m_myHero = NULL;
};

/// When game is created
void ClientApp::onCreate(){
	m_renderer = Renderer::createAutomaticRenderer(&getWindow());

	m_ui = RocketContext::create("ui", Vec2i(1024,768));
	m_ui->loadFont("DroidSansFallback.ttf");

	String account_name = "127.0.0.1";
	ScopedFile fp("../../ip.txt", IODevice::TextRead);
	if(fp.canRead()){
		TextStream ts(&fp);
		account_name = ts.getLine();
	}

	m_client.connect(account_name, 8002, 100);
};

/// Game events
void ClientApp::onEvent(Event &event){

	if(event.type == Event::MouseButtonPressed){
		if(event.mouseButton.button == Mouse::Right){
			

			Vec2f mouse = getWindow().convertCoords(Vec2i(event.mouseButton.x, event.mouseButton.y) , m_view);

			Packet pck;
			pck << (Uint32)Client::DROP_BOMB;
			pck << mouse;
			m_client.send(pck);
		}
	}


	if(event.type == Event::KeyPressed){
		if(event.key.code == Keyboard::D){
			if(m_myHero){
				m_myHero->direction.x = 1;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}

		}
		if(event.key.code == Keyboard::A){
			if(m_myHero){
				m_myHero->direction.x = -1;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}

		}
		if(event.key.code == Keyboard::W){
			if(m_myHero){
				m_myHero->direction.y = -1;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}

		}
		if(event.key.code == Keyboard::S){
			if(m_myHero){
				m_myHero->direction.y = 1;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}

		}
	}
	else if(event.type == Event::KeyReleased){
		if(event.key.code == Keyboard::D){			
			if(m_myHero){
				m_myHero->direction.x = 0;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}
		}
		if(event.key.code == Keyboard::A){			
			if(m_myHero){
				m_myHero->direction.x = 0;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}
		}
		if(event.key.code == Keyboard::S){			
			if(m_myHero){
				m_myHero->direction.y = 0;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}
		}
		if(event.key.code == Keyboard::W){			
			if(m_myHero){
				m_myHero->direction.y = 0;

				Packet p;
				p << (Uint32)Client::HERO_DIRECTION_REQUEST;
				p << m_myHero->id;
				p << m_myHero->direction;
				m_client.send(p);
			}
		}
	}
};

/// Rendering
void ClientApp::onRender(){
	m_renderer->clearBuffers();

	m_view.setRect(0,0,2000,1700);
	m_renderer->setView(m_view);

	for(unsigned int i = 0; i < m_heroList.size(); i++){
		if(m_heroList[i] == m_myHero)
			m_renderer->drawDebugCircle(m_heroList[i]->position, 20, Vec2f(), Color(155,0,0,255));
		else
			m_renderer->drawDebugCircle(m_heroList[i]->position, 20, Vec2f(), Color(155,155,0,255));

		Text t;
		t.setPosition(m_heroList[i]->position);
		String content = m_heroList[i]->nick  + "[" + String::number(m_heroList[i]->health / m_heroList[i]->maxHealth * 100) + "]\nKills: " + String::number(m_heroList[i]->kills) + "   Deaths: " + String::number(m_heroList[i]->deaths);
		if(m_heroList[i]->dead){
			content.append("\nRespawn: " + String::number(m_heroList[i]->respawnTimeLeft));
		}
		t.setString(content);
		m_renderer->draw(t);
	}

	m_renderer->display();
};

/// Updating the game
void ClientApp::onUpdate(Time time){
	m_client.update(10);

	for(unsigned int i = 0; i < m_heroList.size(); i++){
		m_heroList[i]->position += m_heroList[i]->direction * m_heroList[i]->movementSpeed * time.asSeconds();

		if(m_heroList[i]->dead)
			m_heroList[i]->respawnTimeLeft -= time.asSeconds();
	}
};


/// Called when the client connected
void ClientApp::onClientConnect(NetworkClient* client){
	String account_name = "Grimshaw";
	ScopedFile fp("../../account.txt", IODevice::TextRead);
	if(fp.canRead()){
		TextStream ts(&fp);
		account_name = ts.getLine();
	}

	cout<<"Account: "<<account_name<<endl;

	Packet p;
	p << (Uint32)Client::AUTH_REQUEST;
	p << account_name;	
	client->send(p);
};

/// Called when there is new data to read
void ClientApp::onClientData(NetworkClient* , NetworkPacket* packet){
	Packet pck = packet->getData();
	Uint32 packet_id;
	pck >> packet_id;

	switch(packet_id){
		case Server::AUTH_SUCESSFULL:
		{
			cout<<"Server authorized login."<<endl;
		}break;
		case Server::HERO_INFO:
			{
				String nick;
				Int16 id;
				Vec2f pos;
				float movement;
				float hp, maxHp;

				pck >> id >> pos >> nick >> movement >> hp >> maxHp;

				Hero *hero = new Hero();
				hero->id = id;
				hero->position = pos;
				hero->nick = nick;
				hero->movementSpeed = movement;
				hero->health = hp;
				hero->maxHealth = maxHp;

				m_heroList.push_back(hero);

			}break;

		case Server::HERO_IDENTITY:
			{
				Int16 id;
				pck >> id;

				for(unsigned int i = 0; i < m_heroList.size(); i++){
					if(m_heroList[i]->id == id){
						//found our hero
						m_myHero = m_heroList[i];
					}
				}
			}break;
		case Server::HERO_DAMAGE:
			{
				Int16 id, damage;
				pck >> id >> damage;

				Hero* hero = getHeroById(id);
				if(hero){
					hero->health -= damage;
				}
			}break;
		case Server::HERO_RESPAWN:
			{
				Int16 id;
				pck >> id;

				Hero* hero = getHeroById(id);
				if(hero){
					// respawn him
					pck >> hero->position;
					pck >> hero->health;

					hero->dead = false;
				}
			}break;
		case Server::HERO_DEATH:
			{
				Int16 id, source, respawn;
				pck >> id >> respawn >> source;

				Hero* hero = getHeroById(id);
				if(hero){
					hero->dead = true;
					hero->respawnTime = respawn;
					hero->respawnTimeLeft = respawn;
					hero->deaths ++;

					Hero* killer = getHeroById(source);
					if(killer){
						killer->kills ++;
						cout<<killer->nick << " has killed "<<hero->nick<<"!"<<endl;
					}
				}
			}break;
		case Server::HERO_RECOVER:
			{
				Int16 id, damage;
				pck >> id >> damage;

				Hero* hero = getHeroById(id);
				if(hero){
					hero->health += damage;
				}
			}break;
		case Server::HERO_DIRECTION_UPDATE:
			{
				Int16 id;
				pck >> id;

				for(unsigned int i = 0; i < m_heroList.size(); i++){
					if(m_heroList[i]->id == id){
						//found our hero
						pck >> m_heroList[i]->direction;
					}
				}

				cout<<"Update"<<endl;
			}break;
	}
};

/// Find a hero by its id
Hero* ClientApp::getHeroById(int id){
	for(unsigned int i = 0; i < m_heroList.size(); i++){
		if(m_heroList[i]->id == id)return m_heroList[i];
	}
	return NULL;
};