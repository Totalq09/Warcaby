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
		//std::cout << "*******************************************" << std::endl;
		//std::cout << iter->getPaths() << std::endl;
		//std::cout << iter->getLength() << std::endl;
		//std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
		//for (int i = 1; i < iter->getPaths()+1; i++)
		//{
		//	std::cout << "***************" << i << "******************" << std::endl;
		//	iter->gotoRoot();
		//	iter->setPath(i);
		//	{
		//		for (int i = 0; i < iter->getLength()+1; i++)
		//		{
		//			std::cout << iter->getCoordinates().x << " " << iter->getCoordinates().y << std::endl;
		//		
		//			iter->next();
		//		}
		//	}	
		//}
		
		iter->gotoRoot();
		iter->setPath(1);
	}

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
	Status tmp = board->getElementStatus(selectedPawn);

	if (tmp == Status::Player || tmp == Status::Enemy)
	{
		int x = (newPlace.x + selectedPawn.x) / 2;
		int y = (newPlace.y + selectedPawn.y) / 2;

		sf::Vector2i pos(x, y);

		board->movePawn(selectedPawn, newPlace);
		board->setElementStatus(x, y, Status::None);
	}

	//damka
	else if (tmp == Status::PlayerKing || tmp == Status::EnemyKing)
	{
		//jesli po drodze bedzie pionek, zbijam go

		int x;
		int y;

		if (newPlace.x - selectedPawn.x > 0)
			x = 1;
		else
			x = -1;

		if (newPlace.y - selectedPawn.y > 0)
			y = 1;
		else
			y = -1;

		for (int i = 1; i < std::abs(newPlace.x - selectedPawn.x); i++)
		{
			if (board->getElementStatus(selectedPawn.x + (i * x), selectedPawn.y + (i * y)) != Status::None)
			{
				board->movePawn(selectedPawn, newPlace);
				board->setElementStatus(selectedPawn.x + (i * x), selectedPawn.y + (i * y), Status::None);			
			}
		}
	}
	

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

bool Player::checkIfAnyMovementPossible()
{
	if (killingDeep != 0)
		return true;

	for (int i = 0; i < board->getSize(); i++)
	{
		for (int j = 0; j < board->getSize(); j++)
		{
			//1.sprawdze czy moge ruszyc sie pionem
			//2.to samo dla damki, jesli bedzie sie mogla ruszyc o 1 kafelek, to juz wystarczy

			if(status == Status::Player && (board->getElementStatus(i, j) == Status::Player || board->getElementStatus(i, j) == Status::PlayerKing)
				|| status == Status::Enemy && (board->getElementStatus(i, j) == Status::Enemy || board->getElementStatus(i, j) == Status::EnemyKing))
			{
				int x = 1;
				int y = 1;
				for (int o = 0; o < 2; o++)
				{
					for (int p = 0; p < 2; p++)
					{
						y = -y;

						if (checkIfDirectMovementPossible(sf::Vector2i(i, j), sf::Vector2i(i + x, j + y)) == true)
						{
							return true;
						}
							
					}
					x = -x;
				}
			}
		}
	}
	//brak mozliwosci ruchu ruchu lub brak pionkow
	return false;
}

bool Player::checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
	if (newPlace.x > 9 || newPlace.x < 0 || newPlace.y > 9 || newPlace.x < 0)
		return false;

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
		int diffX = (newPlace.x - selectedPawn.x); 
		int diffY = (newPlace.y - selectedPawn.y);

		int x;
		int y;

		if (diffX > 0)
			x = 1;
		else
			x = -1;
		if (diffY > 0)
			y = 1;
		else
			y = -1;

		if (std::abs(diffX) == std::abs(diffY))
		{
			for (int off = 1; off <= std::abs(diffX); off++)
			{
				//jesli poza plansza:
				if ((selectedPawn + sf::Vector2i(x*off, y*off)).x < 0 || (selectedPawn + sf::Vector2i(x*off, y*off)).x > 9 ||
					(selectedPawn + sf::Vector2i(x*off, y*off)).y < 0 || (selectedPawn + sf::Vector2i(x*off, y*off)).y > 9)
					continue;

				//sprawdzam czy po drodze cokolwiek stoi
				if (board->getElementStatus(selectedPawn + sf::Vector2i(x*off, y*off)) != Status::None)
					return false;
			}
			return true; //korzystam z tego ze roznica we wspolrzednych x i y dla duchu damki musi byc taka sama
						 // jest taka sama, a ponadto, najpierw sprawdzam bicia takze, moge zalozyc, ze nie "przeskocze" zadnych pionow		
		}
			
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

	int killingPathTemp = 1;

	//schodzimy jeden poziom nizej
	int killingDeepTemp = killingDeep;
	killingDeepTemp--;

	//NOC
	selectedKillTree.gotoRoot();
	selectedKillTree.setPath(killingPathTemp);

	for (int i = selectedKillTree.getLength() - killingDeepTemp; i > 0; i--)
	{
		selectedKillTree.next();
	}
	//

	//idziemy wzdluz sciezek ( czyli po sasiadach )
	//for (; killingPathTemp <= selectedKillTree.getLength() ;)
	for (; killingPathTemp <= selectedKillTree.getPaths();)
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
