#include "Board.h"
#include <iostream>

sf::Color Board::PLAYERCOLOR = sf::Color::White;
sf::Color Board::ENEMYCOLOR = sf::Color::Red;

Board::Board(int size, int squareSize, sf::Texture * crown) : BOARDSIZE(size), SQUARENUMBER(size)
{
	this->crown = crown;

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
			squares[i][k].setTexture(crown);
		}
	}	

	
}

Board::Board(const Board & boardCopy) : BOARDSIZE(boardCopy.BOARDSIZE) , SQUARENUMBER(BOARDSIZE)
{
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NIEBEZPIECZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//linijka ponize powinna byc zalaczona, ale ani drzewo ani artificial jej nie potrzebuja
	//OPTYMALIZACJA
	//this->crown = boardCopy.crown;

	squares = new BoardElement*[BOARDSIZE];
	for (int i = 0; i < BOARDSIZE; i++)
	{
		squares[i] = new BoardElement[BOARDSIZE];
	}

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int k = 0; k < BOARDSIZE; k++)
		{
			squares[i][k].setTexture(boardCopy.crown);

			//if ((i + k) % 2 == 0)
			//{
			//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NIEBEZPIECZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//	//linijka ponize powinna byc zalaczona, ale ani drzewo ani artificial jej nie potrzebuja
			//	//OPTYMALIZACJA
			//	//squares[i][k].setColor(sf::Color(210, 210, 210)); //GREY
			//	squares[i][k].setStatus(Status::Error);
			//}
			//else
			//{
			//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NIEBEZPIECZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//	//linijka ponize powinna byc zalaczona, ale ani drzewo ani artificial jej nie potrzebuja
			//	//OPTYMALIZACJA
			//	//squares[i][k].setColor(sf::Color::Black);
			//	//squares[i][k].setStatus(Status::None);
			//	squares[i][k].setStatus(boardCopy.squares[i][k].getStatus());
			//}

			if ((i + k) % 2 != 0)
			{
				squares[i][k].setStatus(boardCopy.squares[i][k].getStatus());
			}

			//squares[i][k].setColor(boardCopy.squares[i][k].getColor());
			//squares[i][k].setStatus(boardCopy.squares[i][k].getStatus());

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NIEBEZPIECZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//linijka ponize powinna byc zalaczona, ale ani drzewo ani artificial jej nie potrzebuja
			//OPTYMALIZACJA
			//squares[i][k].setPosition(sf::Vector2f(k * 64, i * 64));
			//squares[i][k].setPosition(sf::Vector2f(squares[i][k].getPosition().x, squares[i][k].getPosition().y));
			
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NIEBEZPIECZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//linijka ponize powinna byc zalaczona, ale ani drzewo ani artificial jej nie potrzebuja
			//OPTYMALIZACJA
			//	squares[i][k].setSize(64);
			//squares[i][k].setSize(squares[i][k].getSize());
		}
	}
}



Board::~Board()
{
	clear();
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
	//	if(squares[i] != nullptr)
		delete[] squares[i];
		squares[i] = nullptr;
	}

	//if (squares != nullptr)
	delete[] squares;
	squares = nullptr;
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

		//Pionek, kt�ry dojdzie do ostatniego rz�du planszy, staje si� damk�, przy czym je�li znajdzie si� tam w wyniku bicia i b�dzie m�g� wykona� kolejne bicie(do ty�u),
		//to b�dzie musia� je wykona� i nie staje si� wtedy damk�(pozostaje pionkiem).
		//takze tutaj do przerobienia
		//po prostu jesli sciezka bicia sie nie skonczyla, to nie moze zostac damka
}

bool Board::shouldBeKing(sf::Vector2i newPlace)
{
	Status status = squares[newPlace.x][newPlace.y].getStatus();

	if (status == Status::PlayerKing || status == Status::EnemyKing) // jesli to juz damka, to nie zmieniamy
		return false;

	else if (status == Status::Player && newPlace.x == SQUARENUMBER - 1)
		return true;
	else if (status == Status::Enemy && newPlace.x == 0)
		return true;
	else
		return false;
}

void Board::setKing(sf::Vector2i pos)
{
	squares[pos.x][pos.y].setKing();

	if (squares[pos.x][pos.y].getStatus() == Status::Player)
		squares[pos.x][pos.y].setStatus(Status::PlayerKing);
	else
		squares[pos.x][pos.y].setStatus(Status::EnemyKing);

}

void Board::setKing(int x, int y)
{

	squares[x][y].setKing();

	if (squares[x][y].getStatus() == Status::Player)
		squares[x][y].setStatus(Status::PlayerKing);
	else
		squares[x][y].setStatus(Status::EnemyKing);
}


