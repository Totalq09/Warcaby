#include "KillTree.h"
#include <iostream>
#include <algorithm>
KillTree::KillTree()
{
	_root = _current = nullptr;
	_player = Status::None;
}
KillTree::KillTree(const KillTree &kt)
{
	_root = copy(kt._root);
	_player = kt._player;
	_current = _root;
}

KillTree::~KillTree()
{
	clear();
}

struct kTree* KillTree::copy(struct kTree* act)
{
	if (!act) return nullptr;
	struct kTree* result = new struct kTree;
	result->x = act->x;
	result->y = act->y;
	result->lenght = act->lenght;
	result->brother = copy(act->brother);
	result->son = copy(act->son);
	return result;
}

void KillTree::createKillTree(Status **p, int x, int y)
{
	if(!isEmpty()) clear();
	_root = getKillsPrelude(p, x, y);
}
void KillTree::createDameKillTree(Status **p, int x, int y)
{
	if(!isEmpty()) clear();
	p[x][y] = Status::None; //na enuma
	_root= getKills_R(p, 0, x, y);
	p[x][y] = _player_dame; //na enuma
}
void KillTree::setPlayer(Status p)
{
	//_player = p;
	switch (p){
	case Status::Player: _player = Status::Player; _player_dame = Status::PlayerKing; break;
	case Status::Enemy: _player = Status::Enemy; _player_dame = Status::EnemyKing; break;
	}
}
bool KillTree::isEnemy(Status **p, int x, int y)
{
	if (p[x][y] == Status::Player || p[x][y] == Status::PlayerKing){
		if (_player == Status::Player || _player == Status::PlayerKing)
			return false;
		else
			return true;
	}
	if (p[x][y] == Status::Enemy || p[x][y] == Status::Enemy){
		if (_player == Status::Enemy || _player == Status::EnemyKing)
			return false;
		else
			return true;
	}
}
bool KillTree::isPlayer(Status **p, int x, int y)
{
	return !isEnemy(p, x, y);
}
int KillTree::getLength()
{
	return _root->lenght;
}
void KillTree::gotoRoot()
{
	_current = _root;
}
void KillTree::getCoordinates(int &x, int &y)
{
	x = _current->x;
	y = _current->y;
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

bool KillTree::create(Status **p, int x, int y){
	clear();
	_pawn = p[x][y];
	p[x][y] = Status::None;
	if (_pawn == _player)
		createKillTree(p, x, y);
	if (_pawn == _player_dame)
		createDameKillTree(p, x, y);
	p[x][y] = _pawn;
	if (_root)
		return true;
	else
		return false;
}


struct kTree* KillTree::getKills_R(Status **p, int depth, int x, int y)
{
	if(p[x][y] != Status::None)    //Trafiliśmy na pionek po przeskoczeniu
		return nullptr;
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
	for(int i = 2; i--;) {  //Pomocnicze pętle bo trzeba 4 kierunki sprawdzić
		for(int j = 2; j--;) {
			if(x + t_x > 0 && y + t_y > 0 && x + t_x < SIZE - 1 && y + t_y < SIZE - 1
			   && isEnemy(p,x + t_x,y + t_y)) {   //Jeśli pole po skosie nie wychodzi poza planszę i jest tam przeciwnik
				kill(p,x + t_x,y + t_y);   //Zabij przeciwnika
				act = getKills_R(p, depth + 1, x + 2 * t_x, y + 2 * t_y); //Wywołaj się na polu po zabiciu
				revive(p,x + t_x,y + t_y);    //Wskrześ przeciwnika
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
void KillTree::kill(Status **p, int x, int y)
{
	switch (p[x][y]){
	case Status::Enemy:	p[x][y] = Status::KilledEnemy; break;
	case Status::EnemyKing:	p[x][y] = Status::KilledEnemyKing; break;
	case Status::Player:	p[x][y] = Status::KilledPlayer; break;
	case Status::PlayerKing:	p[x][y] = Status::KilledPlayerKing; break;
	default:	exit(1); break;
	}
}
void KillTree::revive(Status **p, int x, int y)
{
	switch (p[x][y]){
	case Status::KilledEnemy:	p[x][y] = Status::Enemy; break;
	case Status::KilledEnemyKing:	p[x][y] = Status::EnemyKing; break;
	case Status::KilledPlayer:	p[x][y] = Status::Player; break;
	case Status::KilledPlayerKing:	p[x][y] = Status::PlayerKing; break;
	default:	exit(1); break;
	}
}
struct kTree* KillTree::getDameKills_R(Status **p, int depth, int x, int y)
{
	if(p[x][y] != Status::None)
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
				if(p[x + k * t_x][y + k * t_y] != Status::None) {
					//if(p[x + k * t_x][y + k * t_y] == 1) //Przeciwnik znaleziony
					if(isEnemy(p, x + k * t_x,y + k * t_y)) //Przeciwnik znaleziony
						found_enemy = true;
					break;
				}
			//Zbicie i rekurencja po polach za nim
			if(found_enemy) {
				kill(p,x + k * t_x,y + k * t_y); //zabijamy
				for(int l = k + 1; x + t_x * l < SIZE && y + t_y * l < SIZE &&
				    x + t_x * l >= 0 && y + t_y * l >= 0; l++) { //lądujemy na wszystkich polach za zabitym
					if(p[x + l * t_x][y + l * t_y] != Status::None) break; //inny pionek znaleziony to kończymy
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


				revive(p, x + k * t_x, y + k * t_y); //wskrzeszamy
			} //found_enemy
			t_y = -t_y;
		}//for j
		t_x = -t_x;
	}// for i

	result->son = head;
	if(last) last->brother = nullptr;
	return result;
};

struct kTree* KillTree::getKillsPrelude(Status **p, int x, int y)
{
	p[x][y] = Status::None; //na enuma
	struct kTree* result = getKills_R(p, 0, x, y);
	p[x][y] = _player; //na enuma
	return result;
};

void KillTree::printTree_R(struct kTree* act, int depth)
{
	if(!act) return;
	for(int i = 0; i < depth; i++)
		std::cout << "----";

	std::cout << '[' << act->x << "][" << act->y << "]" << act->lenght << std::endl;;
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
