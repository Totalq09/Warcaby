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
	void createKillTree(int p[SIZE][SIZE], int x, int y);
	void createDameKillTree(int p[SIZE][SIZE], int x, int y);
	void setPlayer(int p);
    int getLength();
    void gotoRoot();
    void getCoordinates(int &x, int &y);
    void next(); //gotoNext
    bool isLeaf(); //jesteśmy w lisciu
	void print();
	void clear();
	bool isEmpty();
protected:
	struct kTree* getKillsPrelude(int p[SIZE][SIZE], int x, int y);
	struct kTree* getKills_R(int p[SIZE][SIZE], int depth, int x, int y);
	struct kTree* getDameKills_R(int p[SIZE][SIZE], int depth, int x, int y);
	void printTree_R(struct kTree* act, int depth);
	void deleteTree_R(struct kTree* act);
	struct kTree* copy(struct kTree* act);
private:
	struct kTree *_root;
	struct kTree *_current;
	int _player;
    //Status _player;
	//Status _enemy;
};

#endif // KILLTREE_H
