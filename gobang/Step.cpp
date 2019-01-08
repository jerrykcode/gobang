#include "Step.h"

Step::Step() {

}

Step::~Step() {

}

void Step::addStep(int row, int col) {
	while (!removedSteps.empty())
		removedSteps.pop();
	steps.push(Coordinate((BYTE)row, (BYTE)col));
}

Coordinate Step::undoStep(){
	if (steps.empty()) {	
		return Coordinate(NO_STEP, NO_STEP);
	}
	Coordinate coordinate = steps.top();
	steps.pop();
	removedSteps.push(coordinate);	
	return coordinate;
}

Coordinate Step::redoStep() {
	if (removedSteps.empty()) {		
		return Coordinate(NO_STEP, NO_STEP);
	}
	Coordinate coordinate = removedSteps.top();
	removedSteps.pop();
	steps.push(coordinate);	
	return coordinate;
}
