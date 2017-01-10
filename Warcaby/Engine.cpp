#include "Engine.h"
#include <iostream>
#include "ArtificialPlayer.hpp"
#include <ctime>
//#include <io.h>
////////////////////////////////////////////////////////////////


////////////////////////////////////
Engine::Engine(GameWindow & win, sf::Texture *crown) : board(SQUARENUMBER,SQUARESIZE, crown)
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

	clear();
}

int Engine::runMulti()
{
	bool isPlayerTurn = board.setPawns();

	player = new Player(Status::Player, board);
	enemy = new Player(Status::Enemy, board);

	while (true)
	{

		if (player->checkIfAnyMovementPossible() == false)
		{
			return 0;
		}

		if (enemy->checkIfAnyMovementPossible() == false)
		{
			return 1;
		}

		bool switchTurn;

		if (isPlayerTurn)
		{
			player->createKillTree();
			switchTurn = handleInput(true);
		}
		else
		{
			enemy->createKillTree();
			switchTurn = handleInput(false);
		}

		handleEvent();

		if (isEnd)
			return 3;

		if (switchTurn == true)
			isPlayerTurn = !isPlayerTurn;

		draw();
	}
}

int Engine::runSingle()
{
    bool isPlayerTurn = board.setPawns();

	//player = new Player(Status::Player, board);
	std::vector<sf::Vector2i> moves;
	sf::Vector2i selectedSquare = sf::Vector2i(0,0);
	ArtificialPlayer* artPlayer = new ArtificialPlayer(3, true);    //biale
	enemy = new Player(Status::Enemy, board);

    draw();
	while (true)
	{
		if (enemy->checkIfAnyMovementPossible() == false)
		{
			return 1;
		}

		bool switchTurn;

		if (isPlayerTurn)
		{
            board.setElementSelected(selectedSquare, false);
            artPlayer->movePawn(board, moves);

            if(moves.size() == 0)
                return 0;

			for(int i=1; i<moves.size(); ++i){
                if(std::abs(moves[i-1].x-moves[i].x) > 1)
                    capturePawn(moves[i-1], moves[i]);
                else
                    board.movePawn(moves[i-1], moves[i]);
            }
            if(board.shouldBeKing(moves[moves.size()-1]))
                board.setKing(moves[moves.size()-1]);

            selectedSquare = moves[moves.size()-1];
            board.setElementSelected(selectedSquare, true);
            moves.clear();
            switchTurn = true;

        }
		else
		{
			enemy->createKillTree();
			switchTurn = handleInput(false);
		}

		handleEvent();

		if (isEnd)
			return 3;

		if (switchTurn == true)
			isPlayerTurn = !isPlayerTurn;

		draw();
	}
}

int Engine::runBots()
{
    bool isPlayerTurn = board.setPawns();

	ArtificialPlayer* artPlayer = new ArtificialPlayer(3, true);    //biale
	ArtificialPlayer* artEnemy = new ArtificialPlayer(3, false);    //czerwone
	std::vector<sf::Vector2i> moves;
	sf::Vector2i selectedSquare = sf::Vector2i(0,0);

    draw();

    time_t actTime = time(0);
	while (true)
	{
        artPlayer->movePawn(board, moves);
        if (moves.size() == 0)
        {
            return 0;   //wygral gracz czerwony
        }

        board.setElementSelected(selectedSquare, false);

        for(int i=1; i<moves.size(); ++i){
            if(std::abs(moves[i-1].x-moves[i].x) > 1)
                capturePawn(moves[i-1], moves[i]);
            else
                board.movePawn(moves[i-1], moves[i]);
        }
        if(board.shouldBeKing(moves[moves.size()-1]))
            board.setKing(moves[moves.size()-1]);

        selectedSquare = moves[moves.size()-1];
        board.setElementSelected(selectedSquare, true);
        draw();
        moves.clear();
        handleEvent();

        while(time(0)-actTime < 3)
            continue;
        actTime = time(0);

        artEnemy->movePawn(board, moves);
        if (moves.size() == 0)
        {
            return 1;   //wygral gracz bialy
        }
        board.setElementSelected(selectedSquare, false);

        for(int i=1; i<moves.size(); ++i){
            if(std::abs(moves[i-1].x-moves[i].x) > 1)
                capturePawn(moves[i-1], moves[i]);
            else
                board.movePawn(moves[i-1], moves[i]);
        }
        if(board.shouldBeKing(moves[moves.size()-1]))
            board.setKing(moves[moves.size()-1]);

        selectedSquare = moves[moves.size()-1];
        board.setElementSelected(selectedSquare, true);
        draw();
        moves.clear();
        handleEvent();

        while(time(0)-actTime < 3)
            continue;
        actTime = time(0);

        if (isEnd)
            return 3;
    }
}


void Engine::capturePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace)
{
    Status tmp = board.getElementStatus(selectedPawn);

	if (tmp == Status::Player || tmp == Status::Enemy)
	{
		int x = (newPlace.x + selectedPawn.x) / 2;
		int y = (newPlace.y + selectedPawn.y) / 2;

		sf::Vector2i pos(x, y);

		board.movePawn(selectedPawn, newPlace);
		board.setElementStatus(x, y, Status::None);
	}

	//damka
	else if (tmp == Status::PlayerKing || tmp == Status::EnemyKing)
	{
		//jesli po drodze bedzie pionek, zbijam go

		int x;
		int y;

		if (newPlace.x - selectedPawn.x > 0)
			x = 1;
		else
			x = -1;

		if (newPlace.y - selectedPawn.y > 0)
			y = 1;
		else
			y = -1;

		for (int i = 1; i < std::abs(newPlace.x - selectedPawn.x); i++)
		{
			if (board.getElementStatus(selectedPawn.x + (i * x), selectedPawn.y + (i * y)) != Status::None)
			{
				board.movePawn(selectedPawn, newPlace);
				board.setElementStatus(selectedPawn.x + (i * x), selectedPawn.y + (i * y), Status::None);
			}
		}
	}
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
	//Kocham c++ i podwojna dealokacje pamieci...
	//Ku potomnym
	//board.clear();

	window->clearWindow();
	isEnd = true;
}


