#pragma once
#include <SFML/Graphics.hpp>
#include "PawnEntity.h"

class Pawn
{
private:
	sf::CircleShape shape;
	sf::Sprite *king = nullptr;

public:
	Pawn();
	Pawn(sf::Vector2f pos, sf::Color col, float s);
	~Pawn();

	void setPosition(sf::Vector2f pos);

	void setColor(sf::Color col);
	sf::Color getColor();

	void setKing(bool val);
	bool isKing();

	void draw(sf::RenderWindow &win);
};

