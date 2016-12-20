#include "GameWindow.h"

/////////////////////////////////////////////////////////////////

GameWindow::GameWindow()
{
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;

	window.create(sf::VideoMode(WIDTH, HEIGHT, 32), title, sf::Style::Titlebar | sf::Style::Close, setting);
	window.setFramerateLimit(60);
}


GameWindow::~GameWindow()
{
}

/////////////////////////////////////////////////////////////////

void GameWindow::clearWindow()
{
	window.clear();
}

void GameWindow::display()
{
	window.display();
}

void GameWindow::draw(sf::Drawable & obj)
{
	window.draw(obj);
}

sf::RenderWindow & GameWindow::getRenderWindow()
{
	return window;
}

void GameWindow::close()
{
	window.close();
}
