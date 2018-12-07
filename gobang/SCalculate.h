#pragma once
#include "Board.h"
#include "Situation.h"

#define MIN_SCORE -2000000000
#define MAX_SCORE 2000000000

typedef enum {
	HORIZONTAL = 0,
	VERTICAL = 1,
	DIAGONAL = 2,
	DIAGONAL2 = 3,
} Direction;

//计算分数类
class SCalculate {
public:
	SCalculate();
	~SCalculate();
	
	/* 返回(row, col)坐标对于chessType类型的分数，并用pBestSituation记录(row, col)坐标的最佳situation */
	int calculatePointScore(Board board, int row, int col, ChessType chessType, PSituation pBestSituation);

	/* 返回整个棋盘对于chessType类型的分数 */
	int calculateBoardScore(Board board, ChessType chessType);

	/* 判断游戏是否因为在(row, col)坐标落子而结束 */
	bool isGameOver(Board board, int row, int col, ChessType chessType);
private:
	void initScore();
	Situation calculateScore(Board board, int row, int col, ChessType chessType, Direction direction);
	Situation calculateHorizontalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateVerticalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateDiagonalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateDiagonalScore2(Board board, int row, int col, ChessType chessType);
	Situation getSituation(int len, int slen, bool isBlocked1, bool isBlocked2);
	int score[N_SITUATION];
};