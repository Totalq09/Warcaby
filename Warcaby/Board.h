#pragma once
#include "Entity.h"
#include "BoardElement.h"
#include "Pawn.h"

class Board
{
private: 
	const int BOARDSIZE;
	const int SQUARENUMBER;

	BoardElement **squares;

	sf::Vector2i selected;

	static sf::Color PLAYERCOLOR;
	static sf::Color ENEMYCOLOR;

public:
	Board(int size, int squareSize);
	~Board();

	void draw(sf::RenderWindow &win);

	void clear();

	/////////////////////////////////////

	void setElementStatus(int x, int y, Status st);
	void setElementStatus(sf::Vector2i pos, Status st);
	Status getElementStatus(int x, int y);
	Status getElementStatus(sf::Vector2i pos);

	void setElementSelected(int x, int y, bool set);
	void setElementSelected(sf::Vector2i pos, bool set);
	bool isElementSelected(int x, int y);
	bool isElementSelected(sf::Vector2i pos);

	sf::Vector2i getSelectedElementPosition();
	Status getSelectedElementStatus();

	bool setPawns();

	bool movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);

	bool checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);
	bool checkIfPossibleByCapture(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);
	bool checkIfPossibleByCaptureNoChange(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);

	std::vector<sf::Vector2i> findLongestStrike(Status st);
	std::vector<sf::Vector2i> goForwardPath(sf::Vector2i pos, Status st, std::vector<sf::Vector2i> path);
};

