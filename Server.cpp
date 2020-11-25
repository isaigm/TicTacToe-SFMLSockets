#include "Server.h"
#include "Commons.h"
Server::Server()
{
	listener.listen(PORT, IP_ADDRESS);
	selector.add(listener);
}

void Server::run()
{
	if (selector.wait())
	{
		if (selector.isReady(listener))
		{
			auto client = new sf::TcpSocket;
			if (listener.accept(*client) == sf::TcpSocket::Done)
			{
				if (clients.size() < 2)
				{
					std::cout << "new client\n";
					selector.add(*client);
					clients.push_back(client);
					sf::Packet packet;
					packet << sf::Uint64((clients.size() % 2) + 1);
					client->send(packet);
				}
				else
				{
					delete client;
				}
			}
			else
			{
				delete client;
			}
		}
		else
		{
			for (size_t i = 0; i < clients.size(); i++)
			{
				if (selector.isReady(*clients[i]))
				{
					sf::Packet packet;
					if (clients[i]->receive(packet) == sf::TcpSocket::Done)
					{
						for (size_t j = 0; j < clients.size(); j++)
						{
							if (i != j)
								clients[j]->send(packet);
						}
					}
				}
			}
		}
	}
}
