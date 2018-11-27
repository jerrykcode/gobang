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
	int nChess = nChessOnBoard(board);
	return map_[nChess].find(key) != map_[nChess].end();
}

BestResult ZMap::getValue(Board board) {
	int key = getZobristCode(board);
	int nChess = nChessOnBoard(board);
	return map_[nChess][key];
}

void ZMap::insert(Board board, BestResult bestRes) {
	int key = getZobristCode(board);
	int nChess = nChessOnBoard(board);
	map_[nChess][key] = bestRes;
}

void ZMap::deleteMap(int nChess) {
	for (auto it = map_[nChess].begin(); it != map_[nChess].end(); it++)
		delete &(it->second);
	map<int, BestResult>().swap(map_[nChess]);
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

int ZMap::nChessOnBoard(Board board)
{
	int nChess = 0;
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			if (board[row][col] != 0)
				nChess++;
		}
	return nChess;
}