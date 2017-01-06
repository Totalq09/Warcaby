#include "Player.h"

void Player::createKillTree()
{
	if (updateKillTree == false)
		return;
	else
		updateKillTree = false;

	selectedKillTree.clear();
	killTrees.clear();
		
	KillTree temp;
	temp.setPlayer(status);

	std::cout << "BUDUJE DRZEWO" << std::endl;
	killingDeep = 0;

	for (int i = 0; i < board->getSize(); i++)
	{
		for (int j = 0; j < board->getSize(); j++)
		{
			if (board->getElementStatus(i, j) == status || board->getElementStatus(i, j) == statusKing)
			{
				temp.create(*board, i, j);
			}			

			if (temp.getLength() == 0)
			{
				//nothing
			}

			else if (killTrees.empty() == true)
				killTrees.push_back(temp);

			
			else if (temp.getLength() > killTrees.front().getLength())
			{
				clearKillTree();

				killTrees.push_back(temp);
			}

			else if (temp.getLength() == killTrees.front().getLength())
			{
				killTrees.push_back(temp);
			}

			temp.clear();
		}	
	}

	if (killTrees.empty() == true)
	{
		killingDeep = 0;
	}
	else
		killingDeep = killTrees.begin()->getLength();

	for (auto iter = killTrees.begin(); iter != killTrees.end(); iter++)
	{
		std::cout << "*******************************************" << std::endl << std::endl;
		std::cout << iter->getPaths() << std::endl;
		std::cout << iter->getLength() << std::endl;
		std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl << std::endl;
		for (int i = 1; i < iter->getPaths()+1; i++)
		{
			std::cout << "***************"<< i <<"******************" << std::endl << std::endl;
			iter->gotoRoot();
			iter->setPath(i);
			{
				for (int i = 0; i < iter->getLength()+1; i++)
				{
					std::cout << iter->getCoordinates().x << " " << iter->getCoordinates().y << std::endl;
				
					iter->next();
				}
			}	
		}

		iter->gotoRoot();
		iter->print();
		iter->gotoRoot();
	}

	std::cout << "*******************************************" << std::endl << std::endl;

	updateKillTree = false;
}

void Player::clearKillTree()
{
	if(killTrees.empty() == false)
		for (auto iter = killTrees.begin(); iter != killTrees.end(); iter++)
		{
			iter->clear();
		}
	killTrees.clear();
}

Player::Player(Status status, Board & board)
{
	this->status = status;

	if (status == Status::Player)
		statusKing = Status::PlayerKing;
	else
		statusKing = Status::EnemyKing;

	if (status == Status::Player)
		direction = 1;
	else
		direction = -1;

	this->board = &board;
}

Player::~Player()
{
	clearKillTree();
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
				//Brak bic dowolny moze zostac wybrany
				if (killTrees.empty() == true)
				{
					state = PlayerState::PawnPicked;
					board->setElementSelected(selectedSquare, true);
				}

				//sprawdzamy, ktory pionek musi bic
				for (auto iter = killTrees.begin(); iter != killTrees.end(); iter++)
				{
					if (iter->getCoordinates() == selectedSquare)
					{
						selectedKillTree = *iter;
						state = PlayerState::PawnPicked;
						board->setElementSelected(selectedSquare, true);
					}
				}	
			}
		}

		//pionek wybrany
		else if (state == PlayerState::PawnPicked)
		{
			//gracz "odkliknal" pionek
			if (changeable == true && board->isElementSelected(selectedSquare) == true)
			{
				state = PlayerState::None;
				board->setElementSelected(selectedSquare, false);
			}

			//gracz chce sie ruszyc i wybral puste pole
			else if (board->getElementStatus(selectedSquare) == Status::None)
			{
				if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				{
					//koniec sekwencji bicia
					if (killingDeep == 0)
					{
						if (board->shouldBeKing(selectedSquare) == true)
							board->setKing(selectedSquare);

						state = PlayerState::None;

						//trzeba zrobic update drzewa przy nastepnym ruch
						updateKillTree = true;
						changeable = true;

						return true;
					}
					//jeszcze nie koniec bicia
					else
					{
						changeable = false;
						board->setElementSelected(selectedSquare, true);
						return false;
					}
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
				//Brak bic, dowolny moze zostac wybrany
				if (killTrees.empty() == true)
				{
					state = PlayerState::PawnPicked;
					board->setElementSelected(selectedSquare, true);
				}

				//sprawdzamy, ktory pionek musi bic
				for (auto iter = killTrees.begin(); iter != killTrees.end(); iter++)
				{
					if (iter->getCoordinates() == selectedSquare)
					{
						selectedKillTree = *iter;
						state = PlayerState::PawnPicked;
						board->setElementSelected(selectedSquare, true);
					}
				}
			}
		}

		//pionek wybrany
		else if (state == PlayerState::PawnPicked)
		{
			//gracz "odkliknal" pionek
			if (changeable == true && board->isElementSelected(selectedSquare) == true)
			{
				state = PlayerState::None;
				board->setElementSelected(selectedSquare, false);
			}

			//gracz chce sie ruszyc i wybral puste pole
			else if (board->getElementStatus(selectedSquare) == Status::None)
			{
				if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				{
					if (killingDeep == 0)
					{
						if (board->shouldBeKing(selectedSquare) == true)
							board->setKing(selectedSquare);

						state = PlayerState::None;

						//trzeba zrobic update drzewa przy nastepnym ruchu
						updateKillTree = true;
						changeable = true;

						return true;
					}
					//jeszcze nie koniec bicia
					else
					{
						changeable = false;
						board->setElementSelected(selectedSquare, true);
						return false;
					}
				}
				//if (movePawn(board->getSelectedElementPosition(), selectedSquare) == true)
				//{
				//	//if(koniec linii sekwencji bicia) == true)
				//	if (board->shouldBeKing(selectedSquare) == true)
				//		board->setKing(selectedSquare);

				//	state = PlayerState::None;
				//	return true;
				//}
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

	killingDeep--;
}

///////////////////////////////////////////////////////

bool Player::movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if(killingDeep != 0 && checkIfCaptureMovementPossible(selectedPawn, newPlace) == true)
	{
		capturePawn(selectedPawn, newPlace);
		return true;
	}

	//jesli jest jeszcze bicie, to nie mozemy sie po prostu poruszyc
	if (killingDeep == 0 && checkIfDirectMovementPossible(selectedPawn, newPlace) == true)
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
	//if (std::abs(newPlace.x - selectedPawn.x) == 2 && std::abs(newPlace.y - selectedPawn.y) == 2)
	//{
	//	int x = (newPlace.x + selectedPawn.x) / 2;
	//	int y = (newPlace.y + selectedPawn.y) / 2;

	//	sf::Vector2i pos(x, y);

	//	if (status == Status::Player)
	//		if (board->getElementStatus(x,y) == Status::Enemy || board->getElementStatus(x, y) == Status::EnemyKing)
	//		{
	//			if (board->getElementStatus(newPlace.x, newPlace.y) == Status::None)
	//				return true;
	//		}

	//	if (status == Status::Enemy)
	//		if (board->getElementStatus(x, y) == Status::Player || board->getElementStatus(x, y) == Status::PlayerKing)
	//		{
	//			if (board->getElementStatus(newPlace.x, newPlace.y) == Status::None)
	//				return true;
	//		}
	//}

	//return false;

	selectedKillTree.gotoRoot();

	int killingPathTemp = 0;

	//schodzimy jeden poziom nizej
	int killingDeepTemp = killingDeep;
	killingDeepTemp--;

	selectedKillTree.next();


	//idziemy wzdluz sciezek ( czyli po sasiadach )
	for (; killingPathTemp <= selectedKillTree.getLength() ;)
	{
		sf::Vector2i pos = selectedKillTree.getCoordinates();
		
		if (pos == newPlace)
		{
			return true;
		}

		else
		{
			killingPathTemp++;
			selectedKillTree.gotoRoot();
			selectedKillTree.setPath(killingPathTemp);

			for (int i = selectedKillTree.getLength() - killingDeepTemp; i > 0; i--)
			{
				selectedKillTree.next();
			}
		}
	}

	return false;
}
