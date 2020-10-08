#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>
#include "Server.h"
#include "Client.h"
int main()
{
	int option;
	std::cin >> option;
	if (option == 1)
	{
		Server server;
		while (true)
		{
			server.run();
		}
	}
	else
	{
		Client client;
		client.run();
	}
	return 0;
}