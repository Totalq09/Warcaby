#include "ArtificialPlayer.hpp"
#define BOARDSIZE 10
#include <limits.h>
#include "KillTree.h"
#include<iostream>

ArtificialPlayer::ArtificialPlayer(int depth, bool colorOfPawns)
{
    this->searchingDepth = depth;
    this->pawnColor = colorOfPawns;
    this->beatFlag = false;
}

//zmieniony algorytm obcinania AlphaBeta
//-> zamiast zwracać wartość alfa lub beta, zwraca współrzędne piona do przesunięcia
void ArtificialPlayer::movePawn(Board& board, std::vector<sf::Vector2i>& moves)
{
    int alpha = INT_MIN;
    int tempAlpha;
    int bestMoveIndex = 0;

    //zbieramy info, jakie ruchy można wykonać w danej kolejce
    std::vector<std::vector<char>* > allPossibleMoves;
    getAllPossibleMoves(pawnColor, board, allPossibleMoves);
    std::cout<<allPossibleMoves.size();

/// std::cout<<"INdex: "<<allPossibleMoves.size();
 ///   exit(14);


    //tworzymy nowe stany gry
    for(int i=0; i<allPossibleMoves.size(); ++i){
        tempAlpha=alphaBetaMin(board, *allPossibleMoves[i], searchingDepth-1, INT_MIN, INT_MAX);
        if(tempAlpha > alpha){  //dana gałąź lepsza od dotychczas znalezionej
            alpha = tempAlpha;
            bestMoveIndex = i;
        }
    }
    if(allPossibleMoves.size()!=0)
        for(int i=0; i<allPossibleMoves[bestMoveIndex]->size(); ++i)
            moves.push_back(getVector((*allPossibleMoves[bestMoveIndex])[i]));

    for(int i =0; i<allPossibleMoves.size(); ++i)
        delete(allPossibleMoves[i]);
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

int ArtificialPlayer::alphaBetaMax(Board board, std::vector<char>& actualMove, int actualDepth, int alpha, int beta)
{
   // std::cout<<"*** "<<actualDepth<<"\n";
    /* Wedle wskazówki wyżej */
    int i;

    for(int i=1; i<actualMove.size(); ++i){
        if(std::abs(getVector(actualMove[i-1]).x-getVector(actualMove[i]).x) > 1)
            capturePawn(getVector(actualMove[i-1]), getVector(actualMove[i]), board);
        else
            board.movePawn(getVector(actualMove[i-1]), getVector(actualMove[i]));
    }
    if(actualMove.size()>0)
        if(board.shouldBeKing(getVector(actualMove[actualMove.size()-1])) == true) //zatrzymaliśmy się na krańcu planszy - powinniśmy być damką
            board.setKing(getVector(actualMove[actualMove.size()-1]));

    if(actualDepth == 0)
        return scorePawnsPosition(board);

    std::vector<std::vector<char>* > allPossibleMoves;
    getAllPossibleMoves(pawnColor, board, allPossibleMoves);

    if(isEndOfGame(allPossibleMoves))
        return scorePawnsPosition(board);

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
int ArtificialPlayer::alphaBetaMin(Board board, std::vector<char>& actualMove, int actualDepth, int alpha, int beta)
{
    //std::cout<<"*** "<<actualDepth<<"\n";
    /* Wedle wskazówki wyżej */
    int i;
	 for(int i=1; i<actualMove.size(); ++i){
        if(std::abs(getVector(actualMove[i-1]).x-getVector(actualMove[i]).x) > 1)
            capturePawn(getVector(actualMove[i-1]), getVector(actualMove[i]), board);
        else
            board.movePawn(getVector(actualMove[i-1]), getVector(actualMove[i]));
    }
    if(actualMove.size()>0)
        if(board.shouldBeKing(getVector(actualMove[actualMove.size()-1])) == true) //zatrzymaliśmy się na krańcu planszy - powinniśmy być damką
            board.setKing(getVector(actualMove[actualMove.size()-1]));

    if(actualDepth == 0)
        return scorePawnsPosition(board);

    std::vector<std::vector<char>* > allPossibleMoves;
    getAllPossibleMoves(!pawnColor, board, allPossibleMoves);

    if(isEndOfGame(allPossibleMoves))
        return scorePawnsPosition(board);

    for(int i=0; i<allPossibleMoves.size(); ++i){
        beta = std::min(beta, alphaBetaMax(board, *allPossibleMoves[i], actualDepth-1, alpha, beta));
        if(alpha>=beta)
        {
            for(int i = 0; i<allPossibleMoves.size();++i)
                delete(allPossibleMoves[i]);
            return alpha;
        }
    }
    for(int i = 0; i<allPossibleMoves.size();++i)
            delete(allPossibleMoves[i]);
    return beta;
}

bool ArtificialPlayer::isEndOfGame(std::vector<std::vector<char>* >& possibleMoves)
{
    if(possibleMoves.size() == 0)
        return true;
    return false;
}

//DZIALA
//nasza heurystyczna metoda wyliczająca, ile warte jest ustawienie pionów na planszy
//wartość = (liczba pionków + 5*liczba damek) - (liczba pionków przeciwnika - 5*liczba damek)
/** link do funkcji oceniającej
    http://sequoia.ict.pwr.wroc.pl/~witold/aiarr/2009_projekty/warcaby/ */
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


	std::cout << score << std::endl;

    if(pawnColor == true)
        return score;
    return -1*score;
}

void ArtificialPlayer::getAllPossibleMoves(bool color, Board& board, std::vector<std::vector<char>* >& moves)
{
    int i, tempLenght;
    std::vector<std::vector<char>* >::iterator it;

    beatFlag = false;
    if(color)
        getAllPossiblePlayerMoves(board, moves);
    else
        getAllPossibleEnemyMoves(board, moves);

    if(beatFlag == true){   //w wektorze bicia -> wyszukaj te najdłuższe
        it = moves.begin();
        i = 0;
        tempLenght = 0;

        while(it != moves.end())    //zostawiam tylko najdłuższe bicia
        {
            if(tempLenght < moves[i]->size())   //znaleziony wektor dłuższy od poprzednich
            {
                tempLenght = moves[i]->size();
                for(int z=0; z<i; ++z)
                    delete moves[z];
                moves.erase(moves.begin(), it);
                it = moves.begin();
                i = 0;
            }
            if(tempLenght > moves[i]->size())   //znaleziony wektor krótszy od poprzednich
            {
                delete moves[i];
                moves.erase(it);
                it = moves.begin();
                i = 0;
            }
            if(tempLenght == moves[i]->size())
            {
                ++it;
                ++i;
            }
        }
    }
}

void ArtificialPlayer::getAllPossiblePlayerMoves(Board& board, std::vector<std::vector<char>* >& moves)
{
    for(int i=0; i<BOARDSIZE; ++i)
        for(int j=0; j<BOARDSIZE; ++j)
        {
            if(board.getElementStatus(i, j) == Status::Player)
                setPossiblePawnMoves(board, i, j, Status::Player, moves);
            if(board.getElementStatus(i, j) == Status::PlayerKing)
                setPossibleKingMoves(board, i, j, Status::PlayerKing, moves);
        }
}

void ArtificialPlayer::getAllPossibleEnemyMoves(Board& board, std::vector<std::vector<char>* >& moves)
{
    for(int i=0; i<BOARDSIZE; ++i)
        for(int j=0; j<BOARDSIZE; ++j)
        {
            if(board.getElementStatus(i, j) == Status::Enemy)
                setPossiblePawnMoves(board, i, j, Status::Enemy, moves);
            if(board.getElementStatus(i, j) == Status::EnemyKing)
                setPossibleKingMoves(board, i, j, Status::EnemyKing, moves);
        }
}
//DZIALA
void ArtificialPlayer::setPossiblePawnMoves(Board& board, int i, int j, Status status,
    std::vector<std::vector<char>* >& moves)
{
    setBeatingMove(board, moves, status, i, j);
    if(beatFlag == false)   //nie ma znalezionych na razie bić
        setNormalMovesOfPawn(board, moves, status, i, j);
}

void ArtificialPlayer::setPossibleKingMoves(Board& board, int i, int j, Status status,
    std::vector<std::vector<char>* >& moves)
{
    setBeatingMove(board, moves, status, i, j);
    if(beatFlag == false)   //nie ma znalezionych na razie bić
        setNormalMovesOfKing(board, moves, status, i, j);
}

void ArtificialPlayer::setNormalMovesOfKing(Board& board, std::vector<std::vector<char>* >& normalMoves,
                    Status status, int i, int j)
{
    int temp = 1;

    while(i+temp<BOARDSIZE && j+temp<BOARDSIZE){
        if(board.getElementStatus(i+temp, j+temp) == Status::None){    //możliwe normalne przejście piona
            normalMoves.push_back(new std::vector<char>);
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i+temp, j+temp));
            ++temp;
        }
        else    //jeśli napotkaliśmy jakiegoś piona, którego nie możemy przeskoczyć
            break;
    }
    temp=1;
    while(i-temp>=0 && j-temp>=0){
        if(board.getElementStatus(i-temp, j-temp) == Status::None){    //możliwe normalne przejście piona
            normalMoves.push_back(new std::vector<char>);
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i-temp, j-temp));
            ++temp;
        }
        else    //jeśli napotkaliśmy jakiegoś piona, którego nie możemy przeskoczyć
            break;
    }
    temp=1;
    while(i+temp<BOARDSIZE && j-temp>=0){
        if(board.getElementStatus(i+temp, j-temp) == Status::None){    //możliwe normalne przejście piona
            normalMoves.push_back(new std::vector<char>);
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i+temp, j-temp));
            ++temp;
        }
        else    //jeśli napotkaliśmy jakiegoś piona, którego nie możemy przeskoczyć
            break;
    }

    temp=1;
    while(i-temp>=0 && j+temp<BOARDSIZE){
        if(board.getElementStatus(i-temp, j+temp) == Status::None){    //możliwe normalne przejście piona
            normalMoves.push_back(new std::vector<char>);
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
            normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i-temp, j+temp));
            ++temp;
        }
        else    //jeśli napotkaliśmy jakiegoś piona, którego nie możemy przeskoczyć
            break;
    }
}

//Dziala
void ArtificialPlayer::setNormalMovesOfPawn(Board& board, std::vector<std::vector<char>* >& normalMoves,
                    Status status, int i, int j)
{
    if(status == Status::Player) {//Zakładam, że PLAYER normalnie porusza się w kierunku dodatnim
            if(i+1<BOARDSIZE && j+1<BOARDSIZE)
                if(board.getElementStatus(i+1, j+1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<char>);
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i+1, j+1));
                }
            if(i+1<BOARDSIZE && j-1>=0)
                if(board.getElementStatus(i+1, j-1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<char>);
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i+1, j-1));
                }
    }
    if(status == Status::Enemy){
            if(i-1>=0 && j+1<BOARDSIZE)
                if(board.getElementStatus(i-1, j+1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<char>);
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i-1, j+1));
                }
            if(i-1>=0 && j-1>=0)
                if(board.getElementStatus(i-1, j-1) == Status::None){    //możliwe normalne przejście piona
                    normalMoves.push_back(new std::vector<char>);
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i, j));
                    normalMoves[normalMoves.size()-1]->push_back(getCharTypePos(i-1, j-1));
                }
    }
}

void ArtificialPlayer::setBeatingMove(Board& board, std::vector<std::vector<char>*>& beatMoves,
                    Status status, int i, int j)
{
    int tempIndex;
    KillTree kTree;
    kTree.setPlayer(status);
    kTree.create(board, i, j);
    kTree.gotoRoot();
    if(kTree.getLength() == 0)
        return;
    kTree.setPath(kTree.getPaths());

    if(beatFlag == false){
        beatFlag = true;
        for (std::vector<char>* vec : beatMoves)
            delete vec;
        beatMoves.clear();
    }


    beatMoves.push_back(new std::vector<char>);
    tempIndex = beatMoves.size()-1;
    while(!kTree.isLeaf())
    {
        beatMoves[tempIndex]->push_back(this->getCharTypePos(kTree.getCoordinates()));
   //  std::cout<<"ROZMIAR: \n"<<kTree.getCoordinates().x<<" "<<kTree.getCoordinates().y<<"\n";
        kTree.next();
    // std::cout<<"ROZMIAR: \n"<<kTree.getCoordinates().x<<" "<<kTree.getCoordinates().y<<"\n";
    }
    beatMoves[tempIndex]->push_back(this->getCharTypePos(kTree.getCoordinates()));
   // std::cout<<"ROZMIAR: \n"<<kTree.getCoordinates().x<<" "<<kTree.getCoordinates().y<<"\n";

    //exit(15);
    return;
}

char ArtificialPlayer::getCharTypePos(int xPos, int yPos)
{
    char x = (char)xPos;
    char y = (char)yPos;
    x = x<<4;
    y = y&0x0F;
    return (x|y);
}

char ArtificialPlayer::getCharTypePos(sf::Vector2i vec)
{
    char x = (char)vec.x;
    char y = (char)vec.y;
    x = x<<4;
    y = y&0x0F;
    return (x|y);
}

sf::Vector2i ArtificialPlayer::getVector(char val)
{
    int x = (val>>4)&0x0F;  //&0x0F bo musimy wyzerować bit znaku
    int y = val&0x0F;
    return sf::Vector2i(x , y);
}

void ArtificialPlayer::capturePawn(sf::Vector2i selectedPawn, sf::Vector2i newPlace, Board& board)
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

