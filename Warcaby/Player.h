#pragma once
#include "Status.h"
#include "Board.h"
#include "KillTree.h"

enum class PlayerState { None, PawnPicked, DuringCapture};

class Player
{
private:
	Status status;
	PlayerState state = PlayerState::None;

	//dla gracza +1 dla przeciwnika -1
	int direction;
	bool capturesCalculationNeeded = true;

	Board * board;

	std::vector<KillTree> captures;

	void calculateCaptures();

public:
	
	////////////////////////////////

	Player(Status status, Board & board);
	~Player();

	bool isPlayer() const;

	bool movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace);

	bool checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace);
	bool checkIfCaptureMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace);

	void simplyMovePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace);
	void capturePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace);

	//jesli ma nastapic zamiana tur (drugi gracz) to return true
	bool handleInput(sf::Vector2i selectedSquare);


};

