#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
class Server
{
public:
	Server();
	void run();
private:
	std::vector<sf::TcpSocket *> clients;
	sf::SocketSelector selector;
	sf::TcpListener listener;
};

