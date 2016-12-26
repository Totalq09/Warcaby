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

	sf::Texture crown;

	BoardElement **squares;

	BoardState currentState = BoardState::None;

	sf::Vector2i selected;

	static sf::Color PLAYERCOLOR;
	static sf::Color ENEMYCOLOR;

	void setBoardState(BoardState state);
	void initiate(int squareSize);

public:
	Board(int size, int squareSize);
	Board(const Board & board);
	~Board();

	void draw(sf::RenderWindow &win);

	void clear();

	/////////////////////////////////////

	void setElementStatus(int x, int y, Status st);
	void setElementStatus(sf::Vector2i pos, Status st);
	Status getElementStatus(int x, int y) const;
	Status getElementStatus(sf::Vector2i pos) const;

	void setElementSelected(int x, int y, bool set);
	void setElementSelected(sf::Vector2i pos, bool set);

	bool isElementSelected(int x, int y) const;
	bool isElementSelected(sf::Vector2i pos) const;

	sf::Vector2i getSelectedElementPosition() const;
	Status getSelectedElementStatus();

	bool setPawns();

	void movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace);

	bool isKing(int x, int y) { return squares[x][y].isKing(); };
	bool isKing(sf::Vector2i pos) { return squares[pos.x][pos.y].isKing(); };

	bool shouldBeKing(sf::Vector2i newPlace);

	void setKing(sf::Vector2i pos);
	void setKing(int x, int y);

	int getSize() { return SQUARENUMBER; };
};

