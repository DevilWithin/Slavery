#include "ClientApp.h"

#include <iostream>
using namespace std;

/// Startup
ClientApp::ClientApp(){
	setWindowTitle("Moba Client");

	m_client.onDataReceived.connect(MAKE_SLOT_LOCAL(ClientApp, onClientData));
};

/// When game is created
void ClientApp::onCreate(){
	m_renderer = Renderer::createAutomaticRenderer(&getWindow());

	m_client.connect("127.0.0.1", 8002, 100);
};

/// Game events
void ClientApp::onEvent(Event &event){
	if(event.type == Event::KeyPressed){
		if(event.key.code == Keyboard::D){
			m_client.send(String("dd")); //hack

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
	}

	m_renderer->drawDebugCircle(Vec2f(300,300), 50, Vec2f(), Color(0, 0, 255));

	m_renderer->display();
};

/// Updating the game
void ClientApp::onUpdate(Time time){
	m_client.update(10);
};


/// Called when there is new data to read
void ClientApp::onClientData(NetworkClient* , NetworkPacket* packet){
	String s = packet->toString();
	cout<<"Message: "<<s<<endl;

	StringList pck;
	s.split(' ', 0, pck);

	if(pck[0] == "01"){
		// welcome packet
		Hero* h = new Hero();
		h->id = pck[1].toInt();

		Vec2f position;
		StringList vecs;
		pck[2].split(',', 1, vecs);
		position.x = vecs[0].toFloat();
		position.y = vecs[1].toFloat();
		
		m_heroList.push_back(h);
		h->position = position;

		m_myHero = h;

		cout<<"I am hero "<<h->id<<endl;
	}
};
