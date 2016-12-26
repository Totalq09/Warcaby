#include "Engine.h"
#include <iostream>
////////////////////////////////////////////////////////////////


////////////////////////////////////
Engine::Engine(GameWindow & win) : board(SQUARENUMBER,SQUARESIZE)
{
	window = &win;

	srand(time(NULL));
	isEnd = false;
	buttonPressed = false;
}

Engine::~Engine()
{
	if (player != nullptr)
		delete player;
	if (enemy != nullptr)
		delete enemy;
}

void Engine::runEngine()
{
	bool isPlayerTurn = board.setPawns();

	player = new Player(Status::Player, board);
	enemy = new Player(Status::Enemy, board);

	while (true)
	{
		bool switchTurn;
		if (isPlayerTurn)
		{
			switchTurn = handleInput(true);
		}
		else
		{
			switchTurn = handleInput(false);
		}

		handleEvent();

		if (isEnd)
			break;

		if (switchTurn == true)
			isPlayerTurn = !isPlayerTurn;

		draw();
	}

	window->close();
}

/////////////////////////////////////////////////////////////////

void Engine::draw()
{
	window->clearWindow();
	board.draw(window->getRenderWindow()); // dlatego ze musimy podac okno na ktorym bedziemy wyswietlac,	
										   //window to jest obiekt klasy opakowujacej sf::RenderWindow
	window->display();
}

void Engine::handleEvent()
{
	sf::Event event;

	while (window->getRenderWindow().pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			clear();
			break;
		}

		if (event.type == sf::Event::GainedFocus || event.type == sf::Event::LostFocus)
		{
			break;
		}
	}
}

bool Engine::handleInput(bool isPlayerTurn)
{
	Mouse mouse;
	Player *currentPlayer;

	if (isPlayerTurn)
		currentPlayer = player;
	else
		currentPlayer = enemy;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		buttonPressed = false;

	if (buttonPressed == true)
		return false;
		
	//////////////////////////////////////////////////////////

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouse = getMousePosition();
		if (mouse.inBounds == false)
			return false;

		buttonPressed = true;

		if (currentPlayer->handleInput(mouse.pos) == true)
			return true;

		//if (currentState == EngineState::None)
		//{
		//	if ((isPlayerTurn == true && (board.getElementStatus(mouse.pos) == Status::Player || board.getElementStatus(mouse.pos) == Status::PlayerKing)) || (isPlayerTurn == false && (board.getElementStatus(mouse.pos) == Status::Enemy || board.getElementStatus(mouse.pos) == Status::EnemyKing)))
		//	{
		//		currentState = EngineState::PawnPicked;
		//		board.setElementSelected(mouse.pos, true);
		//	}
		//	return false;
		//}

		//else if (currentState == EngineState::PawnPicked)
		//{
		//	if (board.isElementSelected(mouse.pos) == true)
		//	{
		//		currentState = EngineState::None;
		//		board.setElementSelected(mouse.pos, false);
		//		return false;
		//	}
		//	
		//	if (board.getElementStatus(mouse.pos) == Status::None)
		//	{
		//		if (board.movePawn(board.getSelectedElementPosition(), mouse.pos, board.getSelectedElementStatus()) == false)
		//			return false;

		//		currentState = EngineState::None;
		//		return true;
		//	}
		//}
	}

	return false;
}

Mouse Engine::getMousePosition() //zbieramy pozycje i sprawdzamy czy w ogole kursor byl w oknie
{
	Mouse mouse;
	sf::Vector2i pos = sf::Mouse::getPosition(window->getRenderWindow());

	int x = pos.x / SQUARESIZE;
	int y = pos.y / SQUARESIZE;

	if (x >= SQUARENUMBER || y >= SQUARENUMBER || pos.x < 0 || pos.y < 0)
		mouse.inBounds = false;
	else
		mouse.inBounds = true;

	mouse.pos = sf::Vector2i(pos.y / SQUARESIZE, pos.x / SQUARESIZE);

	return mouse;
}

void Engine::clear()
{
	board.clear();

	window->clearWindow();
	isEnd = true;
}


