// Warcaby.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <SFML/Graphics.hpp>
#include "Game.h"

#include "KillTree.h"

int main()
{
	Game game;
	game.runGame();




	/*
	int plansza[10][10] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};
	std::cout << "Creating Array\n";
	Status **stat;
	stat = new Status*[10];
	for (int i = 0; i < 10; i++){
		stat[i] = new Status[10];
	}
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++){
			switch (plansza[i][j]){
			case 0:	stat[i][j] = Status::None; break;
			case 1: stat[i][j] = Status::Enemy; break;
			case 2: stat[i][j] = Status::Player; break;
			case 3: stat[i][j] = Status::PlayerKing; break;
			}
		}
	
	KillTree kt;
	std::cout << "Creating Tree\n";
	kt.create(stat, 4, 4);
	std::cout << "Printing Tree\n";
	kt.print();

	int x, y;
	int k = 0;
	kt.gotoRoot();
	std::cout << "Amount of max path found: " << kt.getPaths() << std::endl;
	kt.setPath(9);
	std::cout << "Printing path 9: " <<std::endl;
	while (!kt.isLeaf()){
		kt.getCoordinates(x, y);
		std::cout << "Step " << k++ << " - Coords read: [" << x << "][" << y << "]\n";
		kt.next();
	}
	kt.getCoordinates(x, y);
	std::cout << "Step " << k++ << " - Coords read: [" << x << "][" << y << "]\n";

	for (int i = 0; i < 10; i++){
		delete stat[i];
	}
	delete stat;
	*/
}