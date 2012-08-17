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

			Packet p;
			p << (Uint32)Client::TEST;
			p << "Client pressed A";
			m_client.send(p);

		}
	}
	else if(event.type == Event::KeyReleased){
		if(event.key.code == Keyboard::D){

			Packet p;
			p << (Uint32)Client::TEST;
			p << "Client released D";
			m_client.send(p);

			if(m_myHero){
				m_myHero->direction.x = 0;

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

	View v;
	v.setRect(0,0,2000,1700);
	m_renderer->setView(v);

	for(unsigned int i = 0; i < m_heroList.size(); i++){
		if(m_heroList[i] == m_myHero)
			m_renderer->drawDebugCircle(m_heroList[i]->position, 20, Vec2f(), Color(155,0,0,255));
		else
			m_renderer->drawDebugCircle(m_heroList[i]->position, 20, Vec2f(), Color(155,155,0,255));

		Text t;
		t.setPosition(m_heroList[i]->position);
		t.setString(m_heroList[i]->nick);
		m_renderer->draw(t);
	}

	m_renderer->display();
};

/// Updating the game
void ClientApp::onUpdate(Time time){
	m_client.update(10);

	if(m_myHero){
		m_myHero->position += m_myHero->direction * m_myHero->movementSpeed * time.asSeconds();
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
		case Server::HERO_INFO:
			{
				String nick;
				Int16 id;
				Vec2f pos;

				pck >> id >> pos >> nick;

				Hero *hero = new Hero();
				hero->id = id;
				hero->position = pos;
				hero->nick = nick;			

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
