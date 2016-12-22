#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::Pawn(sf::Vector2f pos, sf::Color col, float s, sf::Texture * crown)
{
	shape.setRadius(s-10);
	shape.setFillColor(col);
	shape.setPosition(pos + sf::Vector2f(10, 10));
	this->crown = crown;
}

Pawn::Pawn(const Pawn & pawn)
{
	shape.setRadius(pawn.shape.getRadius());
	shape.setFillColor(pawn.shape.getFillColor());
	shape.setPosition(pawn.shape.getPosition());

	//specjalnie nie alokuje pamieci na crown, bo wszystkie pionki wspoldziela te sama teksture
	crown = pawn.crown;

	if (pawn.isKing() == true)
		setKing(true);
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
		king->setTexture(*crown);
		king->setOrigin(king->getGlobalBounds().width / 2, king->getGlobalBounds().height / 2);
		king->setPosition(shape.getPosition().x + shape.getRadius(), shape.getPosition().y + shape.getRadius());
		king->setScale(0.5f, 0.5f);
	}

	else //w zasadzie to chyba jest to blad
	{
		if (king == nullptr)
			delete king;
	}
}

bool Pawn::isKing() const
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
