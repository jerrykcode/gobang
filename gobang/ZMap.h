#pragma once
#include "Board.h"
#include <map>
#include <random>
using namespace std;

struct BestResult {
	BestResult(Coordinate c, int level, int score) : c(c), level(level), score(score) {}
	BestResult() {}
	Coordinate c;
	int level;
	int score;
};

//封装std::map数组，用于zobrist缓存
class ZMap {
public:

	ZMap();
	~ZMap();

	/* 返回是否有board缓存 */
	bool hasKey(Board board);

	/* 返回board对应的BestResult */
	BestResult getValue(Board board);

	/* 插入键值对<board, bestRes> */
	void insert(Board board, BestResult bestRes);

	/* 删除nChess颗棋子情况的缓存 */
	void deleteMap(int nChess);

private:

	int getZobristCode(Board board);
	int nChessOnBoard(Board board);

	int** initRandom2DArray();
	void free2DArray(int **arr);

	int **blackBoard;
	int **whiteBoard;
	int **emptyBoard;
	
	map<int, BestResult> map_[NROWS*NCOLS + 1];
	default_random_engine rand;
};