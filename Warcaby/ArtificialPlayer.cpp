#include "ArtificialPlayer.hpp"
#define BOARDSIZE 8
#include <limits.h>

#include<iostream>

ArtificialPlayer::ArtificialPlayer(int depth, bool colorOfPawns)
{
    this->searchingDepth = depth;
    this->pawnColor = colorOfPawns;
}

//zmieniony algorytm obcinania AlphaBeta
//-> zamiast zwracać wartość alfa lub beta, zwraca współrzędne piona do przesunięcia
std::vector<sf::Vector2i>* ArtificialPlayer::movePawn(Board& board)
{
    int alpha = INT_MIN;
    int tempAlpha;
    int bestMoveIndex = 0;

    //zbieramy info, jakie ruchy można wykonać w danej kolejce
    std::vector<std::vector<sf::Vector2i>* > allPossibleMoves = getAllPossibleMoves(pawnColor, board);

    //if(isEndOfGame(allPossibleMoves))
        //EXCEPTION

    //tworzymy nowe stany gry
    for(int i=0; i<allPossibleMoves.size(); ++i){
        tempAlpha=alphaBetaMin(board, *allPossibleMoves[i], searchingDepth-1, INT_MIN, INT_MAX);
        if(tempAlpha > alpha){  //dana gałąź lepsza od dotychczas znalezionej
            alpha = tempAlpha;
            bestMoveIndex = i;
        }
    }
    for(int i =0; i<allPossibleMoves.size(); ++i){
        if(i!=bestMoveIndex)
            delete(allPossibleMoves[i]);
    }
    return allPossibleMoves[bestMoveIndex];
}

ArtificialPlayer::~ArtificialPlayer()
{
}

//UWAGA
/*tutaj trzeba bedzie troche zmienic ze wzgledu na damki
1) jesli pionek dochodzi do odpowiedniej krawedzi to zmienia sie w damke, chyba ze
wlasnie jest w sekwencji bicia i po dojsciu do tej krawedzi musi dalej bic,
wtedy oczywiscie bije, ale wczesniej NIE zamienia sie w damke
aby zmienic pion w damke:
ruszasz sie: board.movePawn(actualMove[i], actualMove[i + 1]); 
a potem
if (board.shouldBeKing(selectedSquare) == true && PION_NIE_JEST_W_TRAKCIE_SEKWENCJI_BICIA)
board.setKing(selectedSquare);
gdzie selected square to bedzie actualMove[i+1] (czyli to pole gdzie bedzie stajemy)

gbyby nie to to w funkcji movePawn mozna by zrealizowac zamiane na damke, a tak to troche dookola trzeba
*/

int ArtificialPlayer::alphaBetaMax(Board board, std::vector<sf::Vector2i>& actualMove, int actualDepth, int alpha, int beta)
{
	for (int i = 0; i + 1 < actualMove.size(); ++i)
		//board.movePawn(actualMove[i], actualMove[i+1], board.getElementStatus(actualMove[i]));
		//zamienilem na to, bo wywalilem ten status, juz nie potrzebny
		board.movePawn(actualMove[i], actualMove[i + 1]);

    std::vector<std::vector<sf::Vector2i>* > allPossibleMoves = getAllPossibleMoves(pawnColor, board);

    if(actualDepth == 0 || isEndOfGame(allPossibleMoves))
    {
        for(int i = 0; i<allPossibleMoves.size();++i)
            delete(allPossibleMoves[i]);
        return scorePawnsPosition(board);
    }
    for(int i=0; i<allPossibleMoves.size(); ++i){
        alpha = std::max(alpha, alphaBetaMin(board, *allPossibleMoves[i], actualDepth-1, alpha, beta));
        if(alpha>=beta)
        {
            for(int i = 0; i<allPossibleMoves.size();++i)
                delete(allPossibleMoves[i]);
            return beta;
        }
    }
    for(int i = 0; i<allPossibleMoves.size();++i)
            delete(allPossibleMoves[i]);
    return alpha;
}
int ArtificialPlayer::alphaBetaMin(Board board, std::vector<sf::Vector2i>& actualMove, int actualDepth, int alpha, int beta)
{
    for(int i=0; i+1<actualMove.size(); ++i)
       // board.movePawn(actualMove[i], actualMove[i+1], board.getElementStatus(actualMove[i]));
		//zamienilem na to, bo wywalilem ten status, juz nie potrzebny
		board.movePawn(actualMove[i], actualMove[i + 1]);

    std::vector<std::vector<sf::Vector2i>* > allPossibleMoves = getAllPossibleMoves(!pawnColor, board);

    if(actualDepth == 0 || isEndOfGame(allPossibleMoves))
    {
        for(int i = 0; i<allPossibleMoves.size();++i)
            delete(allPossibleMoves[i]);
        return scorePawnsPosition(board);
    }

    for(int i=0; i<allPossibleMoves.size(); ++i){
        beta = std::min(beta, alphaBetaMax(board, *allPossibleMoves[i], actualDepth-1, alpha, beta));
        if(alpha>=beta)
        {
            for(int i = 0; i<allPossibleMoves.size();++i)
                delete(allPossibleMoves[i]);
            return alpha;
        }
       alphaBetaMax(board, *allPossibleMoves[i], actualDepth-1, alpha, beta);
    }
    for(int i = 0; i<allPossibleMoves.size();++i)
            delete(allPossibleMoves[i]);
    return beta;
}

//nie można poruszyć pionka w danyj turze
//albo gracz nie ma już pionków, albo nie może ich poruszyćacademic
bool ArtificialPlayer::isEndOfGame(std::vector<std::vector<sf::Vector2i>* >& possibleMoves)
{
    if(possibleMoves.size() == 0)
        return true;
    return false;
}

//DZIALA
//nasza heurystyczna metoda wyliczająca, ile warte jest ustawienie pionów na planszy
//wartość = (liczba pionków + 5*liczba damek) - (liczba pionków przeciwnika - 5*liczba damek)
int ArtificialPlayer::scorePawnsPosition(Board& board)
{
    int score = 0;
    for(int i=0; i<BOARDSIZE; ++i)
        for(int j=0; j<BOARDSIZE; ++j)
            switch(board.getElementStatus(i, j)){
                case Status::Enemy:
                    score -= 1;
                    break;
                case Status::EnemyKing:
                    score -= 5;
                    break;
                case Status::Player:
                    score += 1;
                    break;
                case Status::PlayerKing:
                    score += 5;
                    break;
                default:
                    continue;
            }
    if(pawnColor == true)
        return score;
    return -1*score;
}

std::vector<std::vector<sf::Vector2i>* > ArtificialPlayer::getAllPossibleMoves(bool color, Board& board)
{
    if(color)
        return getAllPossiblePlayerMoves(board);
    return getAllPossibleEnemyMoves(board);
}

std::vector<std::vector<sf::Vector2i>* > ArtificialPlayer::getAllPossiblePlayerMoves(Board& board)
{
    std::vector<std::vector<sf::Vector2i>* > normalMoves;
    std::vector<std::vector<sf::Vector2i>* > beatMoves;
    std::vector<std::vector<sf::Vector2i>* >::iterator it;
    int tempLenght = 0;
    int i = 0;

    for(int i=0; i<BOARDSIZE; ++i)
        for(int j=0; j<BOARDSIZE; ++j)
        {
            if(board.getElementStatus(i, j) == Status::Player)
                setPossiblePawnMoves(board, i, j, Status::Player, normalMoves, beatMoves);
            if(board.getElementStatus(i, j) == Status::PlayerKing)
                setPossibleKingMoves(board, i, j, Status::PlayerKing, normalMoves, beatMoves);
        }
   if(beatMoves.size() != 0)   //w możliwych ruchach występują bicia
    {
        it = beatMoves.begin();
        i = 0;
        while(it != beatMoves.end())    //zostawiam tylko najdłuższe bicia
        {
            if(tempLenght < beatMoves[i]->size())
            {
                tempLenght = beatMoves[i]->size();
                beatMoves.erase(beatMoves.begin(), it);
                it = beatMoves.begin();
                i = 0;
            }
            if(tempLenght == beatMoves[i]->size())
            {
                ++it;
                ++i;
            }
            if(tempLenght > beatMoves[i]->size())
            {
                beatMoves.erase(it);
                it = beatMoves.begin();
                i = 0;
            }
        }
        return beatMoves;
    }
    return normalMoves;
}

std::vector<std::vector<sf::Vector2i>* > ArtificialPlayer::getAllPossibleEnemyMoves(Board& board)
{
    std::vector<std::vector<sf::Vector2i>* > normalMoves;
    std::vector<std::vector<sf::Vector2i>* > beatMoves;
    std::vector<std::vector<sf::Vector2i>* >::iterator it;
    int tempLenght = 0;
    int i = 0;

    for(int i=0; i<BOARDSIZE; ++i)
        for(int j=0; j<BOARDSIZE; ++j)
        {
            if(board.getElementStatus(i, j) == Status::Enemy)
                setPossiblePawnMoves(board, i, j, Status::Enemy, normalMoves, beatMoves);
            if(board.getElementStatus(i, j) == Status::EnemyKing)
                setPossibleKingMoves(board, i, j, Status::EnemyKing, normalMoves, beatMoves);
        }
    if(beatMoves.size() != 0)   //w możliwych ruchach występują bicia
    {
        it = beatMoves.begin();
        i = 0;
        while(it != beatMoves.end())    //zostawiam tylko najdłuższe bicia
        {
            if(tempLenght < beatMoves[i]->size())
            {
                tempLenght = beatMoves[i]->size();
                beatMoves.erase(beatMoves.begin(), it);
                it = beatMoves.begin();
                i = 0;
            }
            if(tempLenght == beatMoves[i]->size())
            {
                ++it;
                ++i;
            }
            if(tempLenght > beatMoves[i]->size())
            {
                beatMoves.erase(it);
                it = beatMoves.begin();
                i = 0;
            }
        }
        return beatMoves;
    }
    return normalMoves;
}
//DZIALA
void ArtificialPlayer::setPossiblePawnMoves(Board& board, int i, int j, Status status,
    std::vector<std::vector<sf::Vector2i>* >& normalMoves, std::vector<std::vector<sf::Vector2i>* >& beatMoves)
{
    setBeatingMovesOfPawn(board, beatMoves, status, i, j);
    if(beatMoves.size() == 0){   //nie ma znalezionych na razie bić
        setNormalMovesOfPawn(board, normalMoves, status, i, j);
    } else {
        for(int i=0; i<normalMoves.size(); ++i)
            delete(normalMoves[i]);
    }
}

void ArtificialPlayer::setPossibleKingMoves(Board& board, int i, int j, Status status,
    std::vector<std::vector<sf::Vector2i>* >& normalMoves, std::vector<std::vector<sf::Vector2i>* >& beatMoves)
{

}

//Dziala
void ArtificialPlayer::setNormalMovesOfPawn(Board& board, std::vector<std::vector<sf::Vector2i>* >& normalMoves,
                    Status status, int i, int j)
{
    if(status == Status::Player) {//Zakładam, że PLAYER normalnie porusza się w kierunku dodatnim
            if(i+1<BOARDSIZE && j+1<BOARDSIZE)
                if(board.getElementStatus(i+1, j+1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<sf::Vector2i>);
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i+1, j+1));
                }
            if(i+1<BOARDSIZE && j-1>=0)
                if(board.getElementStatus(i+1, j-1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<sf::Vector2i>);
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i+1, j-1));
                }
    }
    if(status == Status::Enemy){
            if(i-1>=0 && j+1<BOARDSIZE)
                if(board.getElementStatus(i-1, j+1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<sf::Vector2i>);
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i-1, j+1));
                }
            if(i-1>=0 && j-1>=0)
                if(board.getElementStatus(i-1, j-1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<sf::Vector2i>);
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(sf::Vector2i(i-1, j-1));
                }
    }
}

void ArtificialPlayer::setBeatingMovesOfPawn(Board& board, std::vector<std::vector<sf::Vector2i>*>& normalMoves,
                    Status status, int i, int j)
{

}


