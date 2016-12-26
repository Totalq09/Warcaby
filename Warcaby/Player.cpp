#include "Player.h"

void Player::calculateCaptures()
{
	if (captures.empty() == false)
		captures.clear();

	for (int i = 0; i < board->getSize(); i++)
	{
		for (int j = 0; j < board->getSize(); j++)
		{
			if ((status == Status::Player &&
				(board->getElementStatus(i, j) == status || board->getElementStatus(i, j) == Status::PlayerKing))
				|| (status == Status::Enemy &&
				(board->getElementStatus(i, j) == status || board->getElementStatus(i, j) == Status::EnemyKing)))
			{
				std::cout << "To idzie do KillTree " << i << " " << j << std::endl;
				KillTree temp(status);
				temp.createKillTree(*board, i, j);
				temp.gotoRoot();
				temp.print();

				if (temp.getLength() == 0)
					;//NOTHING
				else if (captures.empty() == true)
				{
					captures.push_back(temp);
					std::cout << "Dodano do pustej" << std::endl;
				}
					
				else if (captures.empty() != false)
					for (auto iter = captures.begin(); iter != captures.end(); iter++)
					{
						if (temp.getLength() == iter->getLength())
							captures.push_back(temp);
						else if (temp.getLength() > iter->getLength())
						{
							captures.clear();
							captures.push_back(temp);
						}
						else
							; //nothing
					}
			}
		}
		std::cout << "Licze" << std::endl;
	}

	for (auto iter = captures.begin(); iter != captures.end(); iter++)
	{
		std::cout << "Prosze o wydruk" << std::endl;
		iter->print();
	}

	capturesCalculationNeeded = false;
}

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

	if (capturesCalculationNeeded == true)
		calculateCaptures();

	if (status == Status::Player)
	{
		if (state == PlayerState::None)
		{
			if (selectedSquareStatus == Status::Player || selectedSquareStatus == Status::PlayerKing)
			{
				if (captures.empty() == true)
				{
					std::cout << "Brak bic" << std::endl;
					state = PlayerState::PawnPicked;
					board->setElementSelected(selectedSquare, true);
				}
				else
				{
					for (auto iter = captures.begin(); iter != captures.end(); iter++)
					{
						if (selectedSquare == iter->getCoordinates())
						{
							std::cout << "Musisz wybrac pionka bijacego" << std::endl;
							state = PlayerState::PawnPicked;
							board->setElementSelected(selectedSquare, true);
						}
					}
				}
			}
		}

		else if (state == PlayerState::PawnPicked)
		{
			if (board->isElementSelected(selectedSquare) == true)
			{
				std::cout << "Odznaczam" << std::endl;
				state = PlayerState::None;
				board->setElementSelected(selectedSquare, false);
			}

			else if (board->getElementStatus(selectedSquare) == Status::None)
			{
				if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				{
					capturesCalculationNeeded = true;
					//if(koniec linii sekwencji bicia) == true)
					if (board->shouldBeKing(selectedSquare) == true)
						board->setKing(selectedSquare);

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
					//if(koniec linii sekwencji bicia) == true)
					if (board->shouldBeKing(selectedSquare) == true)
						board->setKing(selectedSquare);

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
	if (checkIfCaptureMovementPossible(selectedPawn, newPlace) == true)
	{
		capturePawn(selectedPawn, newPlace);
		return true;
	}

	if (checkIfDirectMovementPossible(selectedPawn, newPlace) == true)
	{
		simplyMovePawn(selectedPawn, newPlace);
		return true;
	}
	
	return false;
}

bool Player::checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if (board->isKing(selectedPawn) == false)
	{
		if (newPlace.x == selectedPawn.x + direction) // zwykly ruch
		{
			if (newPlace.y + 1 == selectedPawn.y || newPlace.y - 1 == selectedPawn.y)
				return true;
		}
	}

	else // damka
	{
		int diffX = std::abs(newPlace.x - selectedPawn.x); 
		int diffY = std::abs(newPlace.y - selectedPawn.y);

		if (diffX == diffY)
			return true; //korzystam z tego ze roznica we wspolrzednych x i y dla duchu damki musi byc taka sama
						 // jest taka sama, a ponadto, najpierw sprawdzam bicia takze, moge zalozyc, ze nie "przeskocze" zadnych pionow		
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
