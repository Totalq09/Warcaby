#pragma once
#include <SFML/Graphics.hpp>
#include"Board.h"

class Board;

class ArtificialPlayer
{
    public:
        ArtificialPlayer(int, bool);
        void movePawn(Board&, std::vector<sf::Vector2i>&);
        ~ArtificialPlayer();
    private:
        int searchingDepth;
        bool pawnColor;
        bool beatFlag;

        int alphaBetaMin(Board, std::vector<char>&, int, int, int);
        int alphaBetaMax(Board, std::vector<char>&, int, int, int);
        int scorePawnsPosition(Board&);
        bool isEndOfGame(std::vector<std::vector<char>* >&);
        int s(void);
        void getAllPossibleMoves(bool, Board&, std::vector<std::vector<char>* >&);
        void getAllPossiblePlayerMoves(Board& board, std::vector<std::vector<char>* >&);
        void getAllPossibleEnemyMoves(Board& board, std::vector<std::vector<char>* >&);
        void setPossiblePawnMoves(Board&, int, int, Status, std::vector<std::vector<char>* >&);
        void setPossibleKingMoves(Board&, int, int, Status, std::vector<std::vector<char>* >&);
        void setNormalMovesOfPawn(Board&, std::vector<std::vector<char>* >&, Status, int, int);
        void setNormalMovesOfKing(Board&, std::vector<std::vector<char>* >&, Status, int, int);
        void setBeatingMove(Board&, std::vector<std::vector<char>* >&, Status, int, int);
        sf::Vector2i getVector(char);
        char getCharTypePos(int, int);
        char getCharTypePos(sf::Vector2i);
        void capturePawn(sf::Vector2i, sf::Vector2i, Board&);

};
