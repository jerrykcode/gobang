#include "ZMap.h"

ZMap::ZMap() {
	blackBoard = initRandom2DArray();
	whiteBoard = initRandom2DArray();
	emptyBoard = initRandom2DArray();
}

ZMap::~ZMap() {
	free2DArray(blackBoard);
	free2DArray(whiteBoard);
	free2DArray(emptyBoard);
}

int ** ZMap::initRandom2DArray() {
	int **arr = new int *[NROWS];
	for (int i = 0; i < NROWS; i++) {
		arr[i] = new int[NCOLS];
		for (int j = 0; j < NCOLS; j++) {
			arr[i][j] = rand();
		}
	}
	return arr;
}

void ZMap::free2DArray(int ** arr) {
	for (int i = 0; i < NROWS; i++)
		free(arr[i]);
	free(arr);
}

bool ZMap::hasKey(Board board) {
	int key = getZobristCode(board);
	return map_.find(key) != map_.end();
}

BestResult ZMap::getValue(Board board) {
	int key = getZobristCode(board);
	return map_[key];
}

void ZMap::insert(Board board, BestResult bestRes) {
	int key = getZobristCode(board);
	map_[key] = bestRes;
}

int ZMap::getZobristCode(Board board) {
	int result = 0;
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			int zcode;
			switch (board[row][col]) {
			case BLACK: zcode = blackBoard[row][col]; break;
			case WHITE: zcode = whiteBoard[row][col]; break;
			case EMPTY: zcode = emptyBoard[row][col]; break;
			}
			result ^= zcode;
		}
	return result;
}