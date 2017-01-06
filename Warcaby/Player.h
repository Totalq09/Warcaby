#pragma once
#include "Status.h"
#include "Board.h"
#include "KillTree.h"
#include <iostream>

enum class PlayerState { None, PawnPicked, DuringCapture};

class Player
{
private:
	Status status;
	Status statusKing;
	PlayerState state = PlayerState::None;

	//dla gracza +1 dla przeciwnika -1
	int direction;

	//jak gleboko jestesmy w sekwencji bicia, 0 to znaczy ze nie ma bicia
	//4 oznacza ze jestesmy w sekwencji i trzeba jeszcze zbic 4 pionki
	int killingDeep = 0;

	//jesli false, to znaczy ze jestesmy w trakcie bicia i nie mozemy odznaczyc pionka -
	//(highlighted)
	bool changeable = true;

	Board * board;

	std::vector<KillTree> killTrees;

	//gracz wybral pionka, ktory bedzie bil, jest to jedno z drzew wygenerowanych w killTrees
	KillTree selectedKillTree;

	void clearKillTree();

	//czy potrzeba tworzyc drzewo na nowo
	bool updateKillTree = true;

public:
	
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

	void createKillTree();

	bool checkIfAnyMovementPossible();
};

