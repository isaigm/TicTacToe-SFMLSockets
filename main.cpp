#include <iostream>
#include "Server.h"
#include "Client.h"
int main()
{
	int option;
	std::cout << "1 for server, 0 for client\n";
	std::cin >> option;
	if (option == 1)
	{
		Server server;
		std::cout << "server listening...\n";
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