#include "Board.h"
#include <stack>
using namespace std;

#define NO_STEP -1

class Step {
public:
	Step();
	~Step();

	void addStep(int row, int col);

	//undoStep��redoStepֻ�������ظ�һ����ʵ�ʻ����ǳ����������ظ�Ҳ���ظ�������
	//undoStepֻ�����û�������Ժ󣬲��Ҹ��û��ߵ�ʱ��ʹ�á�
	//redoStepֻ���ڻ����ʹ��
	Coordinate undoStep();
	Coordinate redoStep();

	int numSteps() {
		return steps.size();
	}

	int numRemovedSteps() {
		return removedSteps.size();
	}

private:
	stack<Coordinate> steps;
	stack<Coordinate> removedSteps;
};