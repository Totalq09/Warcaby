#include "Board.h"
#include <iostream>

sf::Color Board::PLAYERCOLOR = sf::Color::White;
sf::Color Board::ENEMYCOLOR = sf::Color::Red;

Board::Board(int size, int squareSize) : BOARDSIZE(size), SQUARENUMBER(size)
{
	std::cout << BOARDSIZE << " " << SQUARENUMBER << std::endl;
	squares = new BoardElement*[BOARDSIZE];
	for (int i = 0; i < BOARDSIZE; i++)
	{
		squares[i] = new BoardElement[BOARDSIZE];
	}

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int k = 0; k < BOARDSIZE; k++)
		{
			if ((i + k) % 2 == 0)
			{
				squares[i][k].setColor(sf::Color(210, 210, 210)); //GREY
				squares[i][k].setStatus(Status::Error);
			}
			else
			{
				squares[i][k].setColor(sf::Color::Black); 
				squares[i][k].setStatus(Status::None);
			}
			squares[i][k].setPosition(sf::Vector2f(k*squareSize, i*squareSize));
			squares[i][k].setSize(squareSize);
		}
	}
}

Board::~Board()
{
}

void Board::draw(sf::RenderWindow & win)
{
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int k = 0; k < BOARDSIZE; k++)
		{
			squares[i][k].draw(win);
		}
	}
}

void Board::clear()
{
	for (int i = 0; i < BOARDSIZE; i++)
	{
		delete[] squares[i];
	}

	delete[] squares;
}

void Board::setBoardState(BoardState state)
{
	currentState = state;
}

void Board::setElementStatus(int x, int y, Status st)
{
	squares[x][y].setStatus(st);
}

void Board::setElementStatus(sf::Vector2i pos, Status st)
{
	squares[pos.x][pos.y].setStatus(st);
}

Status Board::getElementStatus(int x, int y)
{
	return squares[x][y].getStatus();
}

Status Board::getElementStatus(sf::Vector2i pos)
{
	return squares[pos.x][pos.y].getStatus();
}

void Board::setElementSelected(int x, int y, bool set)
{
	squares[x][y].setSelected(set);

	selected = sf::Vector2i(x, y);
}

void Board::setElementSelected(sf::Vector2i pos, bool set)
{
	squares[pos.x][pos.y].setSelected(set);

	selected = pos;
}

bool Board::isElementSelected(int x, int y)
{
	return squares[x][y].isSelected();
}

bool Board::isElementSelected(sf::Vector2i pos)
{
	return squares[pos.x][pos.y].isSelected();
}

sf::Vector2i Board::getSelectedElementPosition()
{
	return selected;
}

Status Board::getSelectedElementStatus()
{
	return squares[selected.x][selected.y].getStatus();
}

bool Board::setPawns()
{
	int offset;
	const int numberOfRows = SQUARENUMBER - (SQUARENUMBER + 2) / 2;
	
	//Player
	for (int i = 0; i < numberOfRows; i++) //trzy rzedy
	{
		if (i % 2 == 0)
			offset = 1;
		else
			offset = 0;

		for (int k = 0; k < SQUARENUMBER / 2; k++)
		{
			setElementStatus(i, 2 * k + offset, Status::Player); // tu jest odwrotnie, bo w macierzy trzymam to tak, ze x to rzedy, a y to kolumny, w SFMLu jest odwrotnie
		}
	}

	//"Enemy"
	for (int i = SQUARENUMBER - numberOfRows; i < SQUARENUMBER; i++) //trzy rzedy
	{
		if (i % 2 == 0)
			offset = 1;
		else
			offset = 0;

		for (int k = 0; k < SQUARENUMBER / 2; k++)
		{
			setElementStatus(i, 2 * k + offset, Status::Enemy);
		}
	}

	int i;

	i = rand() % 2;
	return true;
	/*if(i == 0)
		return true;
	else
		return false;*/
}

void Board::movePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
		squares[selectedPawn.x][selectedPawn.y].setSelected(false);
		Status newStatus = squares[selectedPawn.x][selectedPawn.y].getStatus();
		squares[newPlace.x][newPlace.y].setStatus(newStatus);
		squares[selectedPawn.x][selectedPawn.y].setStatus(Status::None);
}

//bool Board::checkIfDirectMovementPossible(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st)
//{
//	//x oznacza numer RZEDU!
//	if (st == Status::Player)
//	{
//		if (newPlace.x == selectedPawn.x + 1) // zwykly ruch
//		{
//			if (newPlace.y + 1 == selectedPawn.y || newPlace.y - 1 == selectedPawn.y)
//				return true;
//		}
//		return false;
//	}
//
//	if (st == Status::Enemy) // zwykly ruch
//	{
//		if (newPlace.x == selectedPawn.x - 1)
//		{
//			if (newPlace.y + 1 == selectedPawn.y || newPlace.y - 1 == selectedPawn.y)
//				return true;
//		}
//		return false;
//	}
//}
//
//bool Board::checkIfPossibleByCapture(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st)
//{
//	if (std::abs(newPlace.x - selectedPawn.x) == 2 && std::abs(newPlace.y - selectedPawn.y) == 2)
//	{
//		int x = (newPlace.x + selectedPawn.x) / 2;
//		int y = (newPlace.y + selectedPawn.y) / 2;
//
//		sf::Vector2i pos(x, y);
//
//		if(st == Status::Player || st == Status::PlayerKing)
//			if (squares[x][y].getStatus() == Status::Enemy || squares[x][y].getStatus() == Status::EnemyKing)
//			{
//				squares[x][y].setStatus(Status::None);
//				return true;
//			}
//
//		if (st == Status::Enemy || st == Status::EnemyKing)
//			if (squares[x][y].getStatus() == Status::Player || squares[x][y].getStatus() == Status::PlayerKing)
//			{
//				squares[x][y].setStatus(Status::None); 
//				return true;
//			}
//	}
//
//	return false;
//}
//
//bool Board::checkIfPossibleByCaptureNoChange(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Status st)
//{
//	if (std::abs(newPlace.x - selectedPawn.x) == 2 && std::abs(newPlace.y - selectedPawn.y) == 2)
//	{
//		int x = (newPlace.x + selectedPawn.x) / 2;
//		int y = (newPlace.y + selectedPawn.y) / 2;
//
//		sf::Vector2i pos(x, y);
//
//		if (st == Status::Player || st == Status::PlayerKing)
//			if (squares[x][y].getStatus() == Status::Enemy || squares[x][y].getStatus() == Status::EnemyKing)
//			{
//				if(squares[newPlace.x][newPlace.y].getStatus() == Status::None)
//				return true;
//			}
//
//		if (st == Status::Enemy || st == Status::EnemyKing)
//			if (squares[x][y].getStatus() == Status::Player || squares[x][y].getStatus() == Status::PlayerKing)
//			{
//				if (squares[newPlace.x][newPlace.y].getStatus() == Status::None)
//				return true;
//			}
//	}
//
//	return false;
//}

//std::vector<sf::Vector2i> Board::findLongestStrike(Status st)
//{
//	//wybierz kolejny pionek (jesli jest pierwszy ustaw go jako pionek z najdluzszym bieciem
//	//zapusc cos w rodzaju DFSa, wyszukaj najdluzsza sciezke, zapisz ja 
//	//przeiteruj przez wszystkie pionki
//
//	std::vector<sf::Vector2i> theLongestPath;
//	std::vector<sf::Vector2i> temp;
//
//	for (int i = 0; i < SQUARENUMBER; i++)
//	{
//		for (int k = 0; k < SQUARENUMBER; k++)
//		{
//			if (squares[i][k].getStatus() == st)
//			{
//				temp.clear();
//				temp.push_back(sf::Vector2i(i, k));
//
//				if (theLongestPath.empty() == true)
//				{
//					theLongestPath.push_back(sf::Vector2i(i, k));
//					theLongestPath = goForwardPath(sf::Vector2i(i, k), st, temp);
//				}
//				else
//				{
//					temp = goForwardPath(sf::Vector2i(i, k), st, temp);
//
//					if (temp.size() > theLongestPath.size())
//						theLongestPath = temp;
//				}
//			}
//		}
//	}
//
////	std::cout << theLongestPath.size();
//	return theLongestPath;
//}
//
//std::vector<sf::Vector2i> Board::goForwardPath(sf::Vector2i pos, Status st, std::vector<sf::Vector2i> path)
//{
//	/*if (pos.x < 0 || pos.y < 0 || pos.x >= SQUARENUMBER || pos.y >= SQUARENUMBER)
//		return path;*/
//
//	std::vector<sf::Vector2i> theLongestPath = path;
//	std::vector<sf::Vector2i> temp;
//
//	std::vector<sf::Vector2i> tempPath;
//
//	sf::Vector2i nextPosition;
//	nextPosition = pos + sf::Vector2i(2,2);
//
//	bool go = true;
//
//	tempPath = path;
//
//	for (auto ptr = path.begin(); ptr != path.end(); ptr++)
//	{
//		if (*ptr == nextPosition)
//			go = false;
//	}
//	if (nextPosition.x >= 8 || nextPosition.y >= 8)
//		go = false;
//	else
//	{
//		if (go == true && checkIfPossibleByCaptureNoChange(pos, nextPosition, st) == true)
//		{
//			if (checkIfPossibleByCaptureNoChange(pos, nextPosition, st) == true)
//			{
//				tempPath.push_back(sf::Vector2i(nextPosition));
//				theLongestPath = goForwardPath(nextPosition, st, tempPath);
//
//				for (auto p = tempPath.begin(); p < tempPath.end(); p++)
//				{
//				//	std::cout << p->x << " " << p->y << std::endl;
//						//sf::sleep(sf::seconds(1));
//				}
//			}
//		}	
//	}
//
//	////////////////////////////////////////////////////
//
//	nextPosition = pos;
//	nextPosition.x -= 2;
//	nextPosition.y -= 2;
//
//	go = true;
//
//	tempPath = path;
//
//	for (auto ptr = path.begin(); ptr != path.end(); ptr++)
//	{
//		if (*ptr == nextPosition)
//			go = false;
//	}
//	if (nextPosition.x < 0  || nextPosition.y < 0)
//		go = false;
//	else
//	{
//		if (go == true && checkIfPossibleByCaptureNoChange(pos, nextPosition, st) == true)
//		{
//			tempPath.push_back(sf::Vector2i(nextPosition));
//			temp = goForwardPath(nextPosition, st, tempPath);
//			if (temp.size() > theLongestPath.size())
//			{
//				//std::cout << "BLAD" << std::endl;
//				theLongestPath = temp;
//			}
//		}
//	}
//
//	///////////////////////////////////////////
//
//	nextPosition = pos;
//	nextPosition.x += 2;
//	nextPosition.y -= 2;
//
//	go = true;
//
//	tempPath = path;
//
//	for (auto ptr = path.begin(); ptr != path.end(); ptr++)
//	{
//		if (*ptr == nextPosition)
//			go = false;
//	}
//	if (nextPosition.x >= 8 || nextPosition.y < 0)
//		go = false;
//	else
//	{
//		if (go == true && checkIfPossibleByCaptureNoChange(pos, nextPosition, st) == true)
//		{
//			tempPath.push_back(sf::Vector2i(nextPosition));
//			temp = goForwardPath(nextPosition, st, tempPath);
//			if (temp.size() > theLongestPath.size())
//			{
//				//std::cout << "BLAD" << std::endl;
//				theLongestPath = temp;
//			}
//		}
//	}
//	
//	////////////////////////////////////
//
//	nextPosition = pos;
//	nextPosition.x -= 2;
//	nextPosition.y += 2;
//
//	go = true;
//
//	tempPath = path;
//
//	for (auto ptr = path.begin(); ptr != path.end(); ptr++)
//	{
//		if (*ptr == nextPosition)
//			go = false;
//	}
//	if (nextPosition.x < 0 || nextPosition.y >= 8)
//		go = false;
//	else
//	{
//		if (go == true && checkIfPossibleByCaptureNoChange(pos, nextPosition, st) == true)
//		{
//			tempPath.push_back(sf::Vector2i(nextPosition));
//			temp = goForwardPath(nextPosition, st, tempPath);
//			if (temp.size() > theLongestPath.size())
//			{
//				//std::cout << "BLAD" << std::endl;
//				theLongestPath = temp;
//			}
//				
//		}
//	}
//
//	//std::cout << theLongestPath.size() << std::endl;
//	return theLongestPath;
//}
//
//
//
//
//
