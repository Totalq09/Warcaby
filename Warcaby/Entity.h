#pragma once
#include <SFML/Graphics.hpp>

//Prawdopodobnie ze wzglêdu na ubogoœæ mechaniczn¹ Warcab klasa ta nie bêdzie potrzebna, 
//ale skoro jest to moze sie przyda, na pewno nie zaszkodzi
//Mozliwe, ze getPosition() sie przyda

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void draw(sf::RenderWindow &win) = 0;

	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual void setOnBoardPosition(sf::Vector2i pos) = 0;
	virtual sf::Vector2i getOnBoardPosition() = 0;

	virtual sf::Color getColor() { return sf::Color::Transparent; };
	virtual void setColor(sf::Color col) {};
};

