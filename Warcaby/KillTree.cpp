#include "KillTree.h"
#include <iostream>
#include <algorithm>
KillTree::KillTree()
{
	_root = _current = nullptr;
	_player = Status::Player;
	_playerKing = Status::PlayerKing;

	_opponent = Status::Enemy;
	_opponentKing = Status::EnemyKing;
}

KillTree::KillTree(Status st)
{
	_root = _current = nullptr;

	_player = st;

	if (st == Status::Player)
	{
		_playerKing = Status::PlayerKing;

		_opponent = Status::Enemy;
		_opponentKing = Status::EnemyKing;
	}
	else
	{
		_playerKing = Status::EnemyKing;

		_opponent = Status::Player;
		_opponentKing = Status::PlayerKing;
	}
}

KillTree::KillTree(const KillTree & kTree)
{

}

KillTree::~KillTree()
{
	clear();
}
void KillTree::createKillTree(Board board, int x, int y)
//void KillTree::createKillTree(int p[SIZE][SIZE], int x, int y)
{
	if(!isEmpty()) clear();
	_root = getKillsPrelude(board, x, y);
}
void KillTree::createDameKillTree(int p[SIZE][SIZE], int x, int y)
{
	if(!isEmpty()) clear();
	//p[x][y] = 0;
//	_root = getKillsPrelude(p, x, y);
	//p[x][y] = _player;
}
void KillTree::setPlayer(Status st)
{
	_player = st;

	if (st == Status::Player)
	{
		_playerKing = Status::PlayerKing;

		_opponent = Status::Enemy;
		_opponentKing = Status::EnemyKing;
	}
	else
	{
		_playerKing = Status::EnemyKing;

		_opponent = Status::Player;
		_opponentKing = Status::PlayerKing;
	}
	/*switch (_player){
	case Status::Player: _enemy = Status::Enemy; break;
	case Status::PlayerKing:_enemy = Status::Enemy; break;
	case Status::Enemy:_enemy = Status::Player; break;
	case Status::EnemyKing:_enemy = Status::Player; break;
	} */
}
int KillTree::getLength()
{
	if (_root == nullptr)
		return 0;
	else
		return _root->lenght;
}
void KillTree::gotoRoot()
{
	_current = _root;
}
sf::Vector2i KillTree::getCoordinates()
{
	int x = _current->x;
	int y = _current->y;

	return sf::Vector2i(x, y);
}
void KillTree::next()
{
	if(_current->son == nullptr) return;
	struct kTree* temp = _current->son;
	while(temp && temp->lenght != _current->lenght)
		temp = temp->brother;
	_current = temp;
}
bool KillTree::isLeaf()
{
    return _current->son == nullptr;
}
void KillTree::print()
{
    printTree_R(_root,0);
}
void KillTree::clear()
{
    deleteTree_R(_root);
    _root = _current = nullptr;
}
bool KillTree::isEmpty()
{
	return _root == nullptr;
}

struct kTree* KillTree::getKills_R(Board & board, int depth, int x, int y)
//struct kTree* KillTree::getKills_R(int p[SIZE][SIZE], int depth, int x, int y)
{
	//if(p[x][y] != 0)    //Trafiliśmy na pionek po przeskoczeniu
	//	return nullptr;
	
	if (board.getElementStatus(x, y) != Status::None)
		return nullptr;
	std::cout << "NOWE DRZEWO" << std::endl;
	//Tworzenie nowego elementu drzewa zabić
	struct kTree* result = new struct kTree;
	result->x = x;
	result->y = y;
	result->lenght = depth;
	result->son = result->brother = nullptr;
	struct kTree *head, *last, *act;
	head = last = act = nullptr;
	int t_x, t_y;
	t_x = t_y = -1;
	for (int i = 2; i > 0; i--) {  //Pomocnicze pętle bo trzeba 4 kierunki sprawdzić
		for (int j = 2; j > 0; j--) {
			std::cout << "Wspolrzedne " << x + t_x << " " << y + t_y << std::endl;
			//if (board.getElementStatus(x + t_x, y + t_y) == Status::None) std::cout << "TO" << std::endl;
			if(x + t_x > 0 && y + t_y > 0 && x + t_x < SIZE - 1 && y + t_y < SIZE - 1
			 //  && p[x + t_x][y + t_y] == 1) {   //Jeśli pole po skosie nie wychodzi poza planszę i jest tam przeciwnik
				&& (board.getElementStatus(x + t_x, y + t_y) == _opponent || board.getElementStatus(x + t_x, y + t_y) == _opponentKing)){
				//p[x + t_x][y + t_y] = -1;   //Zabij przeciwnika
				Status tempStatus = board.getElementStatus(x + t_x, y + t_y);
				board.setElementStatus(x + t_x, y + t_y, Status::Captured);
				act = getKills_R(board, depth + 1, x + 2 * t_x, y + 2 * t_y); //Wywołaj się na polu po zabiciu
				//p[x + t_x][y + t_y] = 1;    //Wskrześ przeciwnika
				board.setElementStatus(x + t_x, y + t_y, tempStatus);
				if(act) {   //Kill confirmed
					if(!head) //Nie ma jeszcze synów
						head = last = act;
					else {  //Dodaj nowego brata do listy synów
						last->brother = act;
						last = act;
					}
					result->lenght = std::max(result->lenght, act->lenght); //Maksymalna głębokość
				}
				act = nullptr;
			}
			t_y = -t_y; //Zmień kierunek y
		}
		t_x = -t_x; //Zmień kierunek x
	}

	result->son = head;
	if(last) last->brother = nullptr;
	return result;
};

//DO PRZEROBIENIA
struct kTree* KillTree::getDameKills_R(int p[SIZE][SIZE], int depth, int x, int y)
{
	if(p[x][y] != 0)
		return nullptr;
	struct kTree* result = new struct kTree;
	result->x = x;
	result->y = y;
	result->lenght = depth;
	result->son = result->brother = nullptr;
	struct kTree *head, *last, *act;
	head = last = act = nullptr;
	int t_x, t_y;
	t_x = t_y = -1;
	int k;
	bool found_enemy;
	for(int i = 2; i--;) {
		for(int j = 2; j--;) {
			found_enemy = false;
			//Znajdowanie pionka na jednej ze skośnych
			for(k = 1; x + t_x * k < SIZE && y + t_y * k < SIZE &&
			    x + t_x * k >= 0 && y + t_y * k >= 0; k++) //idziemy aż wyjdziemy
				if(p[x + k * t_x][y + k * t_y] != 0) {
					if(p[x + k * t_x][y + k * t_y] == 1) //Przeciwnik znaleziony
						found_enemy = true;
					break;
				}
			//Zbicie i rekurencja po polach za nim
			if(found_enemy) {
				p[x + k * t_x][y + k * t_y] = -1; //zabijamy
				for(int l = k + 1; x + t_x * l < SIZE && y + t_y * l < SIZE &&
				    x + t_x * l >= 0 && y + t_y * l >= 0; l++) { //lądujemy na wszystkich polach za zabitym
					if(p[x + l * t_x][y + l * t_y] != 0) break; //inny pionek znaleziony to kończymy
					act = getDameKills_R(p, depth + 1, x + l * t_x, y + l * t_y); //wywołujemy się dla wszystkich tych pól
					if(act) { // wyniki pakujemy do listy synów
						if(!head)
							head = last = act;
						else {
							last->brother = act;
							last = act;
						}
						result->lenght = std::max(result->lenght, act->lenght); // zapisujemy najgłębszego syna
					}
					act = nullptr;
				}


				p[x + k * t_x][y + k * t_y] = 1; //wskrzeszamy
			} //found_enemy
			t_y = -t_y;
		}//for j
		t_x = -t_x;
	}// for i

	result->son = head;
	if(last) last->brother = nullptr;
	return result;
};

struct kTree* KillTree::getKillsPrelude(Board & board, int x, int y)
//struct kTree* KillTree::getKillsPrelude(int p[SIZE][SIZE], int x, int y)
{
	//p[x][y] = 0; //na enuma
	//struct kTree* result = getKills_R(p, 0, x, y);
	//p[x][y] = _player; //na enuma
	//bo to moze byc pion albo damka
	Status tempStatus = board.getElementStatus(x, y);
	board.setElementStatus(x, y, Status::None);
	struct kTree* result = getKills_R(board, 0, x, y);
	board.setElementStatus(x, y, tempStatus);
	return result;
};

void KillTree::printTree_R(struct kTree* act, int depth)
{
	if(!act) return;
	for(int i = 0; i < depth; i++)
		std::cout << "----";

	std::cout << '[' << act->x << "][" << act->y << "] " << act->lenght << std::endl;;
	printTree_R(act->son, depth + 1);
	printTree_R(act->brother, depth);
}

void KillTree::deleteTree_R(struct kTree* act)
{
	if(!act) return;
	deleteTree_R(act->son);
	deleteTree_R(act->brother);
	delete act;
}