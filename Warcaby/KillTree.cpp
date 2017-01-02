#include "KillTree.h"
#include <iostream>
#include <algorithm>
KillTree::KillTree()
{
	_root = _current = nullptr;
	_player = Status::None;
	_max_lenght = 0;
	_max_paths = 0;
	_path = 0;
}
KillTree::KillTree(const KillTree &kt)
{
	_root = copy_R(kt._root);
	_player = kt._player;
	_player = kt._player_dame;
	_current = _root;
	_max_lenght = kt._max_lenght;
	_max_paths = kt._max_paths;
	_path = kt._path;
}
KillTree::~KillTree()
{
	clear();
}
struct kTree* KillTree::copy_R(struct kTree* act)
{
	if (!act) return nullptr;
	struct kTree* result = new struct kTree;
	result->x = act->x;
	result->y = act->y;
	result->lenght = act->lenght;
	result->brother = copy_R(act->brother);
	result->son = copy_R(act->son);
	return result;
}
//Tworzy drzewo bić dla zwykłego pionka
void KillTree::createKillTree(Status **p, int x, int y)
{
	if(!isEmpty()) clear();
	_root = getKillsPrelude(p, x, y);
}
//Tworzy drzewo bić dla damki
void KillTree::createDameKillTree(Status **p, int x, int y)
{
	if(!isEmpty()) clear();
	p[x][y] = Status::None;
	_root= getDameKills_R(p, 0, x, y);
	p[x][y] = _player_dame;
}
//Ustaw gracza który ma wykonywać bicie
void KillTree::setPlayer(Status p)
{
	//_player = p;
	switch (p){
	case Status::Player: _player = Status::Player; _player_dame = Status::PlayerKing; break;
	case Status::Enemy: _player = Status::Enemy; _player_dame = Status::EnemyKing; break;
	case Status::PlayerKing: _player = Status::Player; _player_dame = Status::PlayerKing; break;
	case Status::EnemyKing: _player = Status::Enemy; _player_dame = Status::EnemyKing; break;
	}
}
//Sprawdza czy na podanym polu znajduje sie przeciwnik
bool KillTree::isEnemy(Status **p, int x, int y)
{

	if (p[x][y] == Status::Player || p[x][y] == Status::PlayerKing){
		if (_player == Status::Enemy || _player == Status::EnemyKing)
			return true;
	}
	if (p[x][y] == Status::Enemy || p[x][y] == Status::Enemy){
		if (_player == Status::Player || _player == Status::PlayerKing)
			return true;
	}
	return false;
}
//Sprawdza czy na podanym polu znajduje sie pionek gracza
bool KillTree::isPlayer(Status **p, int x, int y)
{
	return !isEnemy(p, x, y); //powinno starczyć
}
//Zwraca maksymalna ilosc bic
int KillTree::getLength()
{
	return _root->lenght;
}
//Przejdź na pole początkowe
void KillTree::gotoRoot()
{
	_current = _root;
}
//Zwróć współrzędne aktualnego ruchu/bicia
void KillTree::getCoordinates(int &x, int &y)
{
	x = _current->x;
	y = _current->y;
}
//Ustaw ścieżkę która bedziemy przechodzić w przypadku wielu różnych możliwości dokonania maksymalnego bicia
void KillTree::setPath(int path){
	_path = path;
}
int KillTree::getPaths(){
	return _max_paths;
}
//Przejdź do kolejnego bicia/kroku
void KillTree::next()
{
	if(_current->son == nullptr) return;
	struct kTree* act = _current->son;
	struct kTree* prev = nullptr;
	while (act && act->path <= _path){
		if (act->lenght == _max_lenght)
			prev = act;
		act = act->brother;
	}
	_current = prev;
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
	_player = Status::None;
	_max_lenght = 0;
	_max_paths = 0;
	_path = 0;
}
bool KillTree::isEmpty()
{
	return _root == nullptr;
}
//Tworzy drzewo bić w oparciu o pionek znajdujący się na podanym polu. Uogólnia wszystkie inne metody tworzenia i ustawiania gracza.
bool KillTree::create(Status **p, int x, int y){
	clear();
	_pawn = p[x][y];
	setPlayer(_pawn);
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
	result->path = 0;
	if (_max_lenght == depth){
		_max_paths++;
		result->path = _max_paths;
	}
	if (_max_lenght < depth){
		_max_lenght = depth;
		_max_paths = 1;
		result->path = _max_paths;
	}

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
					if (result->lenght < act->lenght){
						result->lenght = std::max(result->lenght, act->lenght); //Maksymalna głębokość
						result->path = act->path;
					}
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
//Ustaw Status Pionka na polu [x][y] na zbity
void KillTree::kill(Status **p, int x, int y)
{
	switch (p[x][y]){
	case Status::Enemy:	p[x][y] = Status::KilledEnemy; break;
	case Status::EnemyKing:	p[x][y] = Status::KilledEnemyKing; break;
	case Status::Player:	p[x][y] = Status::KilledPlayer; break;
	case Status::PlayerKing:	p[x][y] = Status::KilledPlayerKing; break;
	default: std::cout << "cos sie zjebalo - kill\n";	exit(1); break;	//safety net
	}
}
//Ustaw Status Pionka na polu [x][y] na normalny
void KillTree::revive(Status **p, int x, int y)
{
	switch (p[x][y]){
	case Status::KilledEnemy:	p[x][y] = Status::Enemy; break;
	case Status::KilledEnemyKing:	p[x][y] = Status::EnemyKing; break;
	case Status::KilledPlayer:	p[x][y] = Status::Player; break;
	case Status::KilledPlayerKing:	p[x][y] = Status::PlayerKing; break;
	default:  std::cout << "cos sie zjebalo -- revive\n"; exit(1); break; //safety net
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
	result->path = 0;
	if (_max_lenght == depth){
		_max_paths++;
		result->path = _max_paths;
	}
	if (_max_lenght < depth){
		_max_lenght = depth;
		_max_paths = 1;
		result->path = _max_paths;
	}

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
						if (result->lenght < act->lenght){
							result->lenght = std::max(result->lenght, act->lenght); //Maksymalna głębokość
							result->path = act->path;
						}
						//result->lenght = std::max(result->lenght, act->lenght); // zapisujemy najgłębszego syna
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

	std::cout << '[' << act->x << "][" << act->y << "]" << act->lenght << 'p'<< act->path << std::endl;;
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