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

class ZMap {
public:

	ZMap();
	~ZMap();

	bool hasKey(Board board);
	BestResult getValue(Board board);
	void insert(Board board, BestResult bestRes);

private:

	int getZobristCode(Board board);

	int** initRandom2DArray();
	void free2DArray(int **arr);

	int **blackBoard;
	int **whiteBoard;
	int **emptyBoard;
	
	map<int, BestResult> map_;
	default_random_engine rand;
};