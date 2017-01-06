#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "GameWindow.h"

//klasa odpowiedzialna bedzie za uruchomienie gry i obs³uge menu (dodamy pozniej)

enum GameState { MENU, SINGLE, MULTI, BOTS, GAME_OVER, END };

class Game
{

public:
	Game();
	~Game();

	void menu();

	void runGame();
	void multiPlayer();

	//void single();
	//void com vs com();
	//void multi();

private:
	GameWindow gameWindow;
	Engine *engine;

	sf::Texture crown;

	GameState state;

	sf::Font font;
	sf::Text text[4];
};

