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

	//void single();
	//void com vs com();
	//void multi();

private:
	GameWindow gameWindow;
	Engine *engine;

	sf::RectangleShape summaryBackground;

	sf::Texture crown;
	sf::Texture summaryBackgroundTex;

	GameState state;

	sf::Font font;
	sf::Text text[4];

	int winner;

	void summary(int win);
	int multiPlayer();

	void clear();
};

