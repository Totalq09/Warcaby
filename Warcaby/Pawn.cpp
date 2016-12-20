#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::Pawn(sf::Vector2f pos, sf::Color col, float s)
{
	shape.setRadius(s-10);
	shape.setFillColor(col);
	shape.setPosition(pos + sf::Vector2f(10, 10));
}

Pawn::~Pawn()
{
	if (king != nullptr)
		delete king;
}


void Pawn::setPosition(sf::Vector2f pos)
{
	shape.setPosition(pos);
}

void Pawn::setColor(sf::Color col)
{
	shape.setFillColor(col);
}

sf::Color Pawn::getColor()
{
	return shape.getFillColor();
}

void Pawn::setKing(bool val)
{
	if (val == true)
	{
		king = new sf::Sprite;
		//inicjalizacja
	}

	else //w zasadzie to chyba jest to blad
	{
		if (king == nullptr)
			delete king;
	}
}

bool Pawn::isKing()
{
	if (king != nullptr)
		return true;
	else
		return false;
}

void Pawn::draw(sf::RenderWindow & win)
{
	win.draw(shape);

	if (king != nullptr)
		win.draw(*king);
}
