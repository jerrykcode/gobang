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

//��װstd::map���飬����zobrist����
class ZMap {
public:

	ZMap();
	~ZMap();

	/* �����Ƿ���board���� */
	bool hasKey(Board board);

	/* ����board��Ӧ��BestResult */
	BestResult getValue(Board board);

	/* �����ֵ��<board, bestRes> */
	void insert(Board board, BestResult bestRes);

	/* ɾ��nChess����������Ļ��� */
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