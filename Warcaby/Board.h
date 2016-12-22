#pragma once
#include "Status.h"
#include "BoardElement.h"
#include "Pawn.h"

enum class BoardState { None, PawnPicked };

class Board
{
private: 
	const int BOARDSIZE;
	const int SQUARENUMBER;

	BoardElement **squares;

	BoardState currentState = BoardState::None;

	sf::Vector2i selected;

	static sf::Color PLAYERCOLOR;
	static sf::Color ENEMYCOLOR;

	void setBoardState(BoardState state);

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

	void movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace);

	//bool checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);
	//bool checkIfPossibleByCapture(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);
	//bool checkIfPossibleByCaptureNoChange(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st);
};

