#pragma once
#include <SFML\Graphics.hpp>
#include <string>


class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	void clearWindow();
	void display();
	void draw(sf::Drawable &obj);
	sf::RenderWindow & getRenderWindow();
	void close();

	const int SIZE = 10 * 64;

private:
	const int WIDTH = SIZE;
	const int HEIGHT = SIZE;
	const std::string title = "Warcaby";

	sf::RenderWindow window;
};

