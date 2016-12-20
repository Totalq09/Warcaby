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

private:
	const int WIDTH = 512;
	const int HEIGHT = 512;
	const std::string title = "Warcaby";

	sf::RenderWindow window;
};

