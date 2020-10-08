#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
class Client
{
public:
	Client();
	void run();

private:
	void render();
	void handleInput();
	void gameLoop();
	bool makeMove(int player, int x, int y);
	bool won(int player, int x, int y);
	bool chooseCell(int player, const sf::Vector2i &mousePos);
	sf::Uint64 playerID = 0;
	int wWon = 0;
	bool canMove = false;
	bool finishedGame = false;
	sf::TcpSocket socket;
	std::vector<sf::RectangleShape> rects;
	std::vector<sf::Text> moves;
	sf::Text whoWon;
	sf::Font font;
	sf::RenderWindow window;
	uint8_t board[3][3];
};
