#include "Game.h"

Game::Game() : engine(gameWindow)
{
	state = MENU;

	if (!font.loadFromFile("font.otf"))
	{
		exit(111);
	}
}

Game::~Game()
{
}

void Game::runGame()
{
	//tutaj beda stany gry (menu, single itd.), na razie po prostu odpalamy singla

	while (state != END)
	{
		switch (state)
		{
		case GameState::MENU:
			menu();
			break;
		case GameState::MULTI:
			multiPlayer();
			state = GameState::MENU;
			break;
		default:
			menu();
			break;
		}
	}
}

void Game::multiPlayer()
{
	engine.runEngine();
}

void Game::menu()
{
	sf::Text title("Draughts", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition(640 / 2 - title.getGlobalBounds().width / 2, 20);

	const int ile = 4;

	sf::Text tekst[ile];

	std::string str[] = { "SINGLE", "MULTI","BOTS", "EXIT" };

	for (int i = 0; i<ile; i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(65);

		tekst[i].setString(str[i]);

		tekst[i].setPosition(gameWindow.getRenderWindow().getSize().x / 2 - tekst[i].getGlobalBounds().width / 2, 150 + i * 60);
	}

	int active = 0;

	tekst[active].setFillColor(sf::Color::Cyan);

	while (state == MENU)
	{
		sf::Event event;

		while (gameWindow.getRenderWindow().pollEvent(event))
		{
			//Wci?ni?cie ESC lub przycisk X
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
				state = END;

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Down && ( active == 0 || active == 1 || active == 2))
				{
					tekst[active].setFillColor(sf::Color::White);
					active++;
				}

				else if (event.key.code == sf::Keyboard::Up && ( active == 1 || active == 2 || active == 3 ))
				{
					tekst[active].setFillColor(sf::Color::White);
					active--;
				}
			}

			tekst[active].setFillColor(sf::Color::Cyan);

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && active == 1)
			{
				state = MULTI;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && active == 0)
			{
				//state = SINGLE;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && active == 2)
			{
				//state = BOTS;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && active == 3)
			{
				state = END;
			}
		}

		gameWindow.getRenderWindow().clear();

		gameWindow.draw(title);
		for (int i = 0; i < ile; i++)
		{
			gameWindow.draw(tekst[i]);
		}

		gameWindow.display();
	}
}