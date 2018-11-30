#pragma once
#include "Board.h"
#include "ZMap.h"
#include "SCalculate.h"
#include <vector>
#include <queue>
using namespace std;

class Gobang {

public:

	Gobang(ChessType humanChessType) : computerChessType(REVERSE_CHESS_TYPE(humanChessType)),
										isComputerTurn(computerChessType == BLACK),
										nChess(0)
	{
		board = new char *[NROWS];
		for (int i = 0; i < NROWS; i++) {
			board[i] = new char[NCOLS];
			fill(board[i], board[i] + NCOLS, EMPTY);
		}		
	}

	Gobang() {}

	~Gobang(){
		for (int i = 0; i < NROWS; i++)
			free(board[i]);
		free(board);
	}

	/* 判断用户在(row, col)坐标落子是否valid */
	bool isValid(int row, int col);

	/* 用户在(row, col)坐标落子，更新board */
	void human(int row, int col);

	/* 电脑落子， 更新board */
	void computer();

	/* 返回电脑落子的row坐标 */
	int getComputerRow() { return computerRow; }

	/* 返回电脑落子的col坐标 */
	int getComputerCol() { return computerCol; }

	/* 电脑落子后更新为该用户落子 */
	void updateTurnAfterComputer();

	/* 判断游戏是否因为在(row, col)坐标落子而结束 */
	bool isGameOver(int row, int col);

private:

	/* 在(row, col)坐标下黑棋 */
	void addBlack(int row, int col);

	/* 在(row, col)坐标下白棋 */
	void addWhite(int row, int col);

	/* MAX-MIN搜索(alpha-beta剪枝，zobrist缓存) */
	BestResult getBestResult(int preBestScore, ChessType chessType, int level);

	/* 用pCoordinates返回需要计算的坐标 */
	void bfsGetCoordinates(vector<PCoordinate>& pCoordinates);
	PCoordinate bfsGetAround(queue<PCoordinate>& q, vector<PCoordinate>& pCoordinates, bool **collected, int row, int col);

	/* 返回搜索层数 */
	int getSearchLevel();

	int computerRow, computerCol; //电脑最后落子的坐标
	int nChess; //棋盘上的棋子数量
	bool isComputerTurn; //若该电脑走则为true
	ChessType computerChessType; //电脑棋子类型
	Board board;
	ZMap zmap;
	SCalculate scalculate;
};
