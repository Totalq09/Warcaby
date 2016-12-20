#pragma once
#include <SFML/Graphics.hpp>
#include"Board.h"

class Board;

class ArtificialPlayer
{
    public:
        ArtificialPlayer(int, bool, Board*);
        std::vector<sf::Vector2i> movePawn();
        void updateBoard(std::vector<sf::Vector2i>&);
        ~ArtificialPlayer();
    private:
        int searchingDepth;
        bool pawnColor;
        int alphaBeta(Board, int, int, int);
        int scorePawnsPosition(Board&);
        bool isEndOfGame(Board*);
        int scorePawnsPosition(Board*);
        Board* board;

};

