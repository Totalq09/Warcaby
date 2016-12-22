#pragma once
#include <SFML/Graphics.hpp>
#include "Status.h"
#include "Pawn.h"
#include <iostream>


class BoardElement
{
private:
	sf::RectangleShape shape;
	sf::Color mainColor;

	//co znajduje sie na kaflu
	Status status = Status::None;
	
	//rozmiar kafla
	static int SQUARESIZE;
	static sf::Color PLAYERCOLOR;
	static sf::Color ENEMYCOLOR;

	Pawn * pawn = nullptr;

public:
	
	BoardElement();
	BoardElement(sf::Vector2f pos, sf::Color col, sf::Vector2f s, int squareSize, Status st = Status::Error);
	BoardElement(sf::Vector2f pos, sf::Color col, float s, int squareSize, Status st = Status::Error);
	BoardElement(const BoardElement & boardElement);
	~BoardElement();

	/////////////////////////////

	static void setSquareSize(int s);
	static void setPlayerColor(sf::Color col);
	static void setEnemyColor(sf::Color col);

	void setColor(sf::Color col);
	sf::Color getColor() const;

	//tutaj po prostu ustalamy miejsce na planszy do wyswietlenia, czyli podajemy konkretne miejsce w floacie
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition() const;

	//tutaj poslugujemy sie numerami komorek (np A1)
	sf::Vector2i getOnBoardPosition();
	void setOnBoardPosition(sf::Vector2i pos);

	void setSize(float s);
	void setSize(sf::Vector2f s);
	sf::Vector2f getSize();

	void draw(sf::RenderWindow &win);

	//////////////////////////////

	void setStatus(Status s, bool val = false);
	Status getStatus() const { return status; }

	void setSelected(bool set);
	bool isSelected() const;
};

