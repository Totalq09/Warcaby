#pragma once
#include <SFML/Graphics.hpp>
#include"Board.h"

class Board;

class ArtificialPlayer
{
    public:
        ArtificialPlayer(int, bool);
        std::vector<sf::Vector2i>* movePawn(Board&);
        ~ArtificialPlayer();
   // private:
        int searchingDepth;
        bool pawnColor;

        int alphaBetaMin(Board, std::vector<sf::Vector2i>&, int, int, int);
        int alphaBetaMax(Board, std::vector<sf::Vector2i>&, int, int, int);
        bool isEndOfGame(std::vector<std::vector<sf::Vector2i>* >&);
        int scorePawnsPosition(Board&);
        int s(void);
        std::vector<std::vector<sf::Vector2i>* > getAllPossibleMoves(bool, Board&);
        std::vector<std::vector<sf::Vector2i>* > getAllPossiblePlayerMoves(Board& board);
        std::vector<std::vector<sf::Vector2i>* > getAllPossibleEnemyMoves(Board& board);
        void setPossiblePawnMoves(Board&, int, int, Status, std::vector<std::vector<sf::Vector2i>* >&, std::vector<std::vector<sf::Vector2i>* >&);
        void setPossibleKingMoves(Board&, int, int, Status, std::vector<std::vector<sf::Vector2i>* >&, std::vector<std::vector<sf::Vector2i>* >&);
        void setNormalMovesOfPawn(Board&, std::vector<std::vector<sf::Vector2i>* >&, Status, int, int);
        void setBeatingMovesOfPawn(Board&, std::vector<std::vector<sf::Vector2i>* >&, Status, int, int);

};

