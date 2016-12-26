#include "BoardElement.h"

int BoardElement::SQUARESIZE = 64;
sf::Color BoardElement::PLAYERCOLOR = sf::Color::White;
sf::Color BoardElement::ENEMYCOLOR = sf::Color::Red;

//////////////////////////////

BoardElement::BoardElement()
{
}

BoardElement::BoardElement(sf::Vector2f pos, sf::Color col, sf::Vector2f s, int squareSize, sf::Texture *crown, Status st)
{
	shape.setPosition(pos);
	shape.setFillColor(col);
	shape.setSize(s);

	mainColor = col;
	status = st;

	this->crown = crown;
}

BoardElement::BoardElement(sf::Vector2f pos, sf::Color col, float s, int squareSize, sf::Texture *crown, Status st)
{
	shape.setPosition(pos);
	shape.setFillColor(col);
	shape.setSize(sf::Vector2f(s,s));

	mainColor = col;
	status = st;
	this->crown = crown;
}

BoardElement::BoardElement(const BoardElement & boardElement)
{
	shape = boardElement.shape;
	mainColor = boardElement.mainColor;
	status = boardElement.status;

	if (boardElement.pawn != nullptr)
	{
		pawn = new Pawn(*boardElement.pawn);
	}

	this->crown = boardElement.crown;
}

///////////////////////////////////////////////////////

BoardElement::~BoardElement()
{
	if (pawn != nullptr)
	{
		delete pawn;
		pawn = nullptr;
	}
}

void BoardElement::setTexture(sf::Texture * crown)
{
	this->crown = crown;
}

void BoardElement::setSquareSize(int s)
{
	SQUARESIZE = s;
}

void BoardElement::setPlayerColor(sf::Color col)
{
	PLAYERCOLOR = col;
}

void BoardElement::setEnemyColor(sf::Color col)
{
	ENEMYCOLOR = col;
}

void BoardElement::setColor(sf::Color col)
{
	shape.setFillColor(col);
	mainColor = col;
}

sf::Color BoardElement::getColor() const
{
	return shape.getFillColor();
}

void BoardElement::setPosition(sf::Vector2f pos)
{
	shape.setPosition(pos);
}

sf::Vector2f BoardElement::getPosition() const
{
	return shape.getPosition();
}

//te setSize'y sluza do skalowania piona
void BoardElement::setSize(float s)
{
	shape.setSize(sf::Vector2f(s,s));
}

void BoardElement::setSize(sf::Vector2f s)
{
	shape.setSize(s);
}

sf::Vector2f BoardElement::getSize()
{
	return shape.getSize(); 
}

void BoardElement::draw(sf::RenderWindow & win)
{
	win.draw(shape);

	if (pawn != nullptr)
	{
		pawn->draw(win);
	}
}

sf::Vector2i BoardElement::getOnBoardPosition()
{
	int x = getPosition().x /*- sf::Vector2f(10, 10)).x */ / SQUARESIZE;
	int y = getPosition().y /*- sf::Vector2f(10, 10)).y*/ / SQUARESIZE;

	return sf::Vector2i(x, y); //!!!!
}

void BoardElement::setOnBoardPosition(sf::Vector2i pos)
{
	float x = pos.x * SQUARESIZE;
	float y = pos.y * SQUARESIZE;

	shape.setPosition(sf::Vector2f(x, y));
}


/////////////////////////////////////////////////////

void BoardElement::setStatus(Status s, bool val) // val - czy to jest damka
{
	try
	{
		switch (s)
		{
			case Status::Captured:
				if (status != Status::Player && status != Status::PlayerKing 
					&& status != Status::Enemy && status != Status::EnemyKing)
					throw "Illegal status change - cannot capture non-pawn field";

			case Status::None:
				if (status == Status::Error)
					throw "Illegal status change - cannot change to none";
				else if (status == Status::None)
					;
				else if (status == Status::Player || status == Status::Enemy || status == Status::PlayerKing || status == Status::EnemyKing)
				{
					if (pawn == nullptr)
						throw "Pawn did not exist but should";
					else
					{
						delete pawn;
						pawn = nullptr;
					}		
				}
				else
					throw "Status does not exist";
				break;
			
			case Status::Error:
				if(status != Status::None && status != Status::Error)
					throw "Illegal status change - cannot change to error";
				break;

			case Status::Player:
				if(status != Status::None && status != Status::Captured)
					throw "Illegal status change - field was occupied or error";
				else
				{
					pawn = new Pawn(getPosition(), PLAYERCOLOR, SQUARESIZE/2, crown);
				}
				break;
			case Status::Enemy:
				if (status != Status::None && status != Status::Captured)
					throw "Illegal status change - field was occupied or error";
				else
				{
					pawn = new Pawn(getPosition(), ENEMYCOLOR, SQUARESIZE/2, crown);
				}
				break;
			case Status::PlayerKing:
				if (status != Status::None && status != Status::Player && status != Status::Captured)
					throw "Illegal status change - field was occupied or error";
				else
				{
					pawn = new Pawn(getPosition(), PLAYERCOLOR, SQUARESIZE/2, crown);
					pawn->setKing(true);
				}
				break;
			case Status::EnemyKing:
				if (status != Status::None && status != Status::Enemy && status != Status::Captured)
					throw "Illegal status change - field was occupied or error";
				else
				{
					pawn = new Pawn(getPosition(), ENEMYCOLOR, SQUARESIZE/2, crown);
					pawn->setKing(true);
				}
				break;
		}
	}
	catch (const char *msg)
	{
		std::cout << msg << std::endl;
		while (1);
		exit(101);
	}

	status = s;	
}

void BoardElement::setSelected(bool set)
{
	if (set == true)
		shape.setFillColor(sf::Color::Green);
	else
		shape.setFillColor(mainColor);
}

bool BoardElement::isSelected() const
{
	if (shape.getFillColor() == sf::Color::Green)
		return true;
	else
		return false;
}

