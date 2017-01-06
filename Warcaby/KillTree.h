#ifndef KILLTREE_H
#define KILLTREE_H

#include "Status.h"
#include "Board.h"

#define SIZE 10

struct kTree {
	int x, y;
	struct kTree *son;
	struct kTree *brother;
	int lenght;
	int path;
};

/* Kolejność:
 * setPlayer();
 * create*Tree()
 * gotoRoot()
 * loop{
 *     next()
 *     getCoordinates()
 *     <do_stuff>
 * } until (isLeaf())
 * ezpz
 */

/*Alternatywnie:
	create();

*/

class KillTree
{
public:
	KillTree();
	KillTree(const KillTree &kt);
	~KillTree();
	//GŁÓWNE
	bool create(Board p, int x, int y);
	void gotoRoot();
	void getCoordinates(int &x, int &y);	//wsp aktualnego kroku
	sf::Vector2i KillTree::getCoordinates();
	int getLength();	//maksymalna ilość bić

	int getPaths();
	void setPath(int path); //UWAGA OD 1 do getPaths() włącznie

	void createKillTree(Board p, int x, int y);
	void createDameKillTree(Board p, int x, int y);
	void setPlayer(Status p);

    void next(); //gotoNext
    bool isLeaf(); //jesteśmy w lisciu
	void print();
	void clear();
	bool isEmpty();


protected:
	struct kTree* getKillsPrelude(Board p, int x, int y);
	struct kTree* getKills_R(Board p, int depth, int x, int y);
	struct kTree* getDameKills_R(Board p, int depth, int x, int y);
	void printTree_R(struct kTree* act, int depth);
	void deleteTree_R(struct kTree* act);
	struct kTree* copy_R(struct kTree* act);
	void countPathsAmount(struct kTree* act);
	bool isEnemy(Board p, int x, int y);
	bool isPlayer(Board p, int x, int y);
	void kill(Board & p, int x, int y);
	void revive(Board & p, int x, int y);
private:
	struct kTree *_root;
	struct kTree *_current;
	int _max_lenght, _max_paths;	//maksymalna ilość bić, liczba scieżek, które dają maksymalną liczbę bić;
	int _path, _path_counter;		//wybrana ścieżka, pomocniczy licznik przy 
    Status _player;
	Status _player_dame;
	Status _pawn;
};

#endif // KILLTREE_H
