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

class KillTree
{
public:
	KillTree();
	KillTree(const KillTree &kt);
	~KillTree();
	bool create(Status **p, int x, int y);
	void createKillTree(Status **p, int x, int y);
	void createDameKillTree(Status **p, int x, int y);
	void setPlayer(Status p);
    int getLength();
    void gotoRoot();
    void getCoordinates(int &x, int &y);
    void next(); //gotoNext
    bool isLeaf(); //jesteśmy w lisciu
	void print();
	void clear();
	bool isEmpty();
protected:
	struct kTree* getKillsPrelude(Status **p, int x, int y);
	struct kTree* getKills_R(Status **p, int depth, int x, int y);
	struct kTree* getDameKills_R(Status **p, int depth, int x, int y);
	void printTree_R(struct kTree* act, int depth);
	void deleteTree_R(struct kTree* act);
	struct kTree* copy(struct kTree* act);
	bool isEnemy(Status **p, int x, int y);
	bool isPlayer(Status **p, int x, int y);
	void kill(Status **p, int x, int y);
	void revive(Status **p, int x, int y);
private:
	struct kTree *_root;
	struct kTree *_current;
	//int _player;
    Status _player;
	Status _player_dame;
	Status _pawn;
};

#endif // KILLTREE_H
