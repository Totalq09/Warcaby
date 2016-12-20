#include "Game.h"


Game::Game() : engine(gameWindow)
{

}


Game::~Game()
{
}

void Game::runGame()
{
	//tutaj beda stany gry (menu, single itd.), na razie po prostu odpalamy singla
	engine.runEngine();
}
