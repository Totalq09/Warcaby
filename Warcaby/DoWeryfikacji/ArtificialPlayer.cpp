#include "ArtificialPlayer.hpp"
#include <limits.h>

ArtificialPlayer::ArtificialPlayer(int depth, bool colorOfPawns, Board* board)
{
    this->searchingDepth = depth;
    this->pawnColor = colorOfPawns;
    this->board = board;
}

//zmieniony algorytm obcinania AlphaBeta
//-> zamiast zwracać wartość alfa lub beta, zwraca współrzędne piona do przesunięcia
std::vector<sf::Vector2i> ArtificialPlayer::movePawn()
{
    int alpha = INT_MIN;
    int temp;
    std::vector<sf::Vector2i> path;

    //najpierw sprawdzić, czy nie ma bicia jakiegoś!!!

    if(isEndOfGame(board) || searchingDepth == 0)
        return path;

    for(;;){    //Trzeba wymyśleś jakiś sprytny algorytm wyszukiwania ruchów
        temp=alphaBeta(*board, searchingDepth-1, INT_MIN, INT_MAX);
        if(temp > alpha){
        path.clear();
        // path.push_back();   //??? Zapamiętujemy, którym pionem mamy się poruszyć żeby było w porządku
        alpha = temp;
        }
    }
    return path;
}

int ArtificialPlayer::alphaBeta(Board board, int actualDepth, int alpha, int beta)
{
    if(isEndOfGame(&board) || searchingDepth == 0)
        return scorePawnsPosition(&board);

    if((searchingDepth-actualDepth)%2 == 0) { //my się poruszamy
        for(;;){    //Trzeba wymyśleś jakiś sprytny algorytm wyszukiwania ruchów
            alpha = std::max(alpha, alphaBeta(board, actualDepth-1, alpha, beta));
            if(alpha>=beta)
                return beta;
        }
        return alpha;
    }
    else {
        for(;;){    //Trzeba wymyśleś jakiś sprytny algorytm wyszukiwania ruchów
            beta = std::min(beta, alphaBeta(board, actualDepth-1, alpha, beta));
            if(alpha>=beta)
                return alpha;
        }
        return beta;
    }
}

//albo któryś z graczy nie ma już pionów, albo nie może ich poruszyć
bool ArtificialPlayer::isEndOfGame(Board* board)
{
    return false;
}

//nasza heurystyczna metoda wyliczająca, ile warte jest ustawienie pionów na planszy
int ArtificialPlayer::scorePawnsPosition(Board* board)
{
    return 10;
}

