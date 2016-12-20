#pragma once
#include "Entity.h"

class PawnEntity : public Entity
{
public:
	PawnEntity();
	PawnEntity(int squareSize);
	~PawnEntity();

	virtual bool isInBounds(sf::Vector2f pos) = 0;

	//sprawdz gdzie wolno sie ruszyc
};

