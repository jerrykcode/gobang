#include "Board.h"
#include <stack>
using namespace std;

#define NO_STEP -1

class Step {
public:
	Step();
	~Step();

	void addStep(int row, int col);

	//undoStep和redoStep只撤销、重复一步；实际悔棋是撤销两步；重复也是重复两步。
	//undoStep只能在用户落过子以后，并且该用户走的时候使用。
	//redoStep只能在悔棋后使用
	Coordinate undoStep();
	Coordinate redoStep();

	int getLastRow() {
		return lastRow;
	}

	int getLastCol() {
		return lastCol;
	}

private:
	stack<Coordinate> steps;
	stack<Coordinate> removedSteps;

	int lastRow;
	int lastCol;
};