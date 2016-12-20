#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "GameWindow.h"

//klasa odpowiedzialna bedzie za uruchomienie gry i obs³uge menu (dodamy pozniej)

class Game
{

public:
	Game();
	~Game();

	void runGame();

private:
	GameWindow gameWindow;
	Engine engine;
};

