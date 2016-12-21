#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "GameWindow.h"
#include "Board.h"
#include "PawnEntity.h"
#include "Pawn.h"

enum class EngineState { None, PawnPicked };

//niestety nie moge sobie od tak sprawdzac pozycji myszy, najpierw musze sprawdzic
//czy w ogole kursor znajduje sie w oknie gry
//dlatego bede potrzebowal takiej prostej struktury
//nieco utrudni to sprawdzanie inputu, ale suma sumarum nie bede musial sie iterowac przez wszystkie
//elementy planszy, by sprawdzic, czy na ktorys nie kliknelismy
//bede mogl zalozyc ze ktorys pole zostalo na pewno wybrane

struct Mouse
{
	sf::Vector2i pos;
	bool inBounds;
};

///////////////////////////////////////////

class Engine
{
public:
	Engine(GameWindow & win);
	~Engine();

	void runEngine();

private:
	static const int SQUARESIZE = 64;
	static const int SQUARENUMBER = 10;
	static const int PAWNNUMBER = SQUARENUMBER;

	Board board;
	GameWindow* window;

	bool buttonPressed;

	bool isEnd;
	EngineState currentState;

	///////////////////////////////////////////

	void draw();

	void handleEvent();
	bool handleInput(bool isPlayerTurn);

	Mouse getMousePosition();

	void clear();
};

