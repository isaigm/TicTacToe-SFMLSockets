#include "Client.h"
#include <iostream>
#include <thread>
#include "Commons.h"
Client::Client() : window(sf::VideoMode(800, 600), "Tic Tac Toe multiplayer", sf::Style::Titlebar | sf::Style::Close)
{
	window.setVerticalSyncEnabled(60);
	if (!font.loadFromFile("C:\\Users\\isaig\\Desktop\\arial.ttf"))
	{
		std::cout << "Cannot load font\n";
		exit(1);
	}
	sf::Packet packet;
	socket.connect(IP_ADDRESS, PORT);
	if (socket.receive(packet) != sf::TcpSocket::Done)
	{
		std::cout << "cannot get player id\n";
		exit(1);
	}
	packet >> playerID;
	if (playerID == O_PLAYER)
	{
		canMove = false;
	}
	else if (playerID == X_PLAYER)
	{
		canMove = true;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = EMPTY;
			sf::RectangleShape r;
			sf::Vector2f size(S, S);
			r.setSize(size);
			r.setPosition(INIT_X + S * j, INIT_Y + S * i);
			r.setFillColor(sf::Color(192, 192, 192));
			r.setOutlineColor(sf::Color::Black);
			r.setOutlineThickness(2);
			rects.push_back(std::move(r));
		}
	}
	window.setVerticalSyncEnabled(true);
}

void Client::run()
{
	//warning: this code is unsafe because produces memory inconsistencies
	std::thread th([this]() {
		while (true)
		{
			int x = -1;
			int y = -1;
			int splayer = 0;
			int whoWon = 0;
			sf::Packet packet;
			if (socket.receive(packet) == sf::TcpSocket::Done)
			{
				packet >> x;
				packet >> y;
				packet >> splayer;
				packet >> whoWon;
				if (playerID != splayer)
				{
					makeMove((playerID % 2) + 1, x, y);
					if (whoWon != 0)
					{
						finishedGame = true;
					}
					canMove = true;
				}
			}
		}
	});
	th.detach();
	gameLoop();
}

void Client::render()
{
	window.clear(sf::Color::White);
	for (const auto &r : rects)
	{
		window.draw(r);
	}
	for (const auto &m : moves)
	{
		window.draw(m);
	}
	window.display();
}

void Client::handleInput()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canMove && !finishedGame)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				bool flag = chooseCell(playerID, mousePos);
				if (flag)
				{
					canMove = false;
					if (wWon != 0)
					{
						std::cout << "player: " << wWon << " has won\n";
						finishedGame = true;
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void Client::gameLoop()
{
	while (window.isOpen())
	{
		handleInput();
		render();
	}
}

bool Client::makeMove(int player, int x, int y)
{
	if (board[y][x] == EMPTY && x >= 0 && x <= 2 && y >= 0 && y <= 2)
	{
		board[y][x] = player;
		sf::Text text;
		text.setCharacterSize(70);
		text.setFillColor(sf::Color::Black);
		text.setFont(font);
		text.setStyle(sf::Text::Regular);
		if (player == X_PLAYER)
		{
			text.setString("X");
		}
		else if (player == O_PLAYER)
		{
			text.setString("O");
		}
		if (won(player, x, y))
		{
			wWon = player;
		}
		auto bounds = text.getLocalBounds();
		auto center = sf::Vector2f(INIT_X + S * x + S / 2, INIT_Y + S * y + S / 2);
		text.setPosition(center.x - bounds.width / 2, center.y - bounds.height / 2 - 20);
		moves.push_back(std::move(text));
		sf::Packet packet;
		packet << x;
		packet << y;
		packet << player;
		packet << wWon;
		socket.send(packet);
		return true;
	}
	return false;
}

bool Client::won(int player, int x, int y)
{
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (board[i][x] == player)
			count++;
	}
	if (count == 3)
		return true;
	count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (board[y][i] == player)
			count++;
	}
	if (count == 3)
		return true;
	if (x == y)
	{
		count = 0;
		for (int i = 0; i < 3; i++)
		{
			if (board[i][i] == player)
				count++;
		}
		if (count == 3)
			return true;
	}
	if ((x == 1 && y == 1) || (x == 2 && y == 0) || (x == 0 && y == 2))
	{
		count = 0;
		for (int i = 2; i >= 0; i--)
		{
			if (board[2 - i][i] == player)
				count++;
		}
		if (count == 3)
			return true;
	}
	return false;
}

bool Client::chooseCell(int player, const sf::Vector2i &mousePos)
{
	for (const auto &r : rects)
	{
		if (r.getGlobalBounds().contains(mousePos.x, mousePos.y))
		{
			int x = (r.getPosition().x - INIT_X) / S;
			int y = (r.getPosition().y - INIT_Y) / S;
			return makeMove(player, x, y);
		}
	}
	return false;
}
