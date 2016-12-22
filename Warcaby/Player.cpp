#include "Player.h"

Player::Player(Status status, Board & board)
{
	this->status = status;

	if (status == Status::Player)
		direction = 1;
	else
		direction = -1;

	this->board = &board;
}

Player::~Player()
{
}

bool Player::isPlayer() const
{
	if (status == Status::Player)
		return true; //true if white
	else
		return false;
}

bool Player::handleInput(sf::Vector2i selectedSquare)
{
	Status selectedSquareStatus = board->getElementStatus(selectedSquare);

	if (status == Status::Player)
	{
		if (state == PlayerState::None)
		{
			if (selectedSquareStatus == Status::Player || selectedSquareStatus == Status::PlayerKing)
			{
				state = PlayerState::PawnPicked;
				board->setElementSelected(selectedSquare, true);
			}
		}

		else if (state == PlayerState::PawnPicked)
		{
			if (board->isElementSelected(selectedSquare) == true)
			{
				state = PlayerState::None;
				board->setElementSelected(selectedSquare, false);
			}

			else if (board->getElementStatus(selectedSquare) == Status::None)
			{
				if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				{
					state = PlayerState::None;
					return true;
				}
			}
		}
	}

	else if (status == Status::Enemy)
	{
		if (state == PlayerState::None)
		{
			if (selectedSquareStatus == Status::Enemy || selectedSquareStatus == Status::EnemyKing)
			{
				state = PlayerState::PawnPicked;
				board->setElementSelected(selectedSquare, true);
			}
		}

		else if (state == PlayerState::PawnPicked)
		{
			if (board->isElementSelected(selectedSquare) == true)
			{
				state = PlayerState::None;
				board->setElementSelected(selectedSquare, false);
			}

			else if (board->getElementStatus(selectedSquare) == Status::None)
			{
				if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				{
					state = PlayerState::None;
					return true;
				}
			}
		}
	}
		
	return false;
}

/////////////////////////////////////////////////////////

void Player::simplyMovePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	board->movePawn(selectedPawn, newPlace);
}

void Player::capturePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	int x = (newPlace.x + selectedPawn.x) / 2;
	int y = (newPlace.y + selectedPawn.y) / 2;

	sf::Vector2i pos(x, y);

	board->movePawn(selectedPawn, newPlace);
	board->setElementStatus(x, y, Status::None);
}

///////////////////////////////////////////////////////

bool Player::movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if (checkIfDirectMovementPossible(selectedPawn, newPlace) == true)
	{
		simplyMovePawn(selectedPawn, newPlace);
		return true;
	}

	if (checkIfCaptureMovementPossible(selectedPawn, newPlace) == true)
	{
		capturePawn(selectedPawn, newPlace);
		return true;
	}

	return false;
}

bool Player::checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if (newPlace.x == selectedPawn.x + direction) // zwykly ruch
	{
		if (newPlace.y + 1 == selectedPawn.y || newPlace.y - 1 == selectedPawn.y)
			return true;
	}
	return false;
}

bool Player::checkIfCaptureMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if (std::abs(newPlace.x - selectedPawn.x) == 2 && std::abs(newPlace.y - selectedPawn.y) == 2)
	{
		int x = (newPlace.x + selectedPawn.x) / 2;
		int y = (newPlace.y + selectedPawn.y) / 2;

		sf::Vector2i pos(x, y);

		if (status == Status::Player)
			if (board->getElementStatus(x,y) == Status::Enemy || board->getElementStatus(x, y) == Status::EnemyKing)
			{
				if (board->getElementStatus(newPlace.x, newPlace.y) == Status::None)
					return true;
			}

		if (status == Status::Enemy)
			if (board->getElementStatus(x, y) == Status::Player || board->getElementStatus(x, y) == Status::PlayerKing)
			{
				if (board->getElementStatus(newPlace.x, newPlace.y) == Status::None)
					return true;
			}
	}

	return false;
}
