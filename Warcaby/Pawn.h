#pragma once
#include <SFML/Graphics.hpp>

class Pawn
{
private:
	sf::CircleShape shape;
	//jesli to bedzie damka, to pojawi sie sprite nakladany na shape
	//king
	sf::Texture *crown;
	sf::Sprite *king = nullptr;

public:
	Pawn();
	Pawn(sf::Vector2f pos, sf::Color col, float s, sf::Texture * crown);
	Pawn(const Pawn & pawn);
	~Pawn();

	void setPosition(sf::Vector2f pos);

	void setColor(sf::Color col);
	sf::Color getColor();

	void setKing(bool val);
	bool isKing() const;

	void draw(sf::RenderWindow &win);
};

