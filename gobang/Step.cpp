#include "Step.h"

Step::Step() : lastRow(NO_STEP), lastCol(NO_STEP) {

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
		lastRow = NO_STEP;
		lastCol = NO_STEP;
		return Coordinate(NO_STEP, NO_STEP);
	}
	Coordinate coordinate = steps.top();
	steps.pop();
	removedSteps.push(coordinate);
	lastRow = coordinate.row();
	lastCol = coordinate.col();
	return coordinate;
}

Coordinate Step::redoStep() {
	if (removedSteps.empty()) {
		lastRow = NO_STEP;
		lastCol = NO_STEP;
		return Coordinate(NO_STEP, NO_STEP);
	}
	Coordinate coordinate = removedSteps.top();
	removedSteps.pop();
	steps.push(coordinate);
	lastRow = coordinate.row();
	lastCol = coordinate.col();
	return coordinate;
}
