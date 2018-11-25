#pragma once
#include "Board.h"
#include "Situation.h"

#define MIN_SCORE -2000000000
#define MAX_SCORE 2000000000

class SCalculate {
public:
	SCalculate();
	~SCalculate();

	int calculatePointScore(Board board, int row, int col, ChessType chessType);
	int calculateBoardScore(Board board, ChessType chessType);
	bool isGameOver(Board board, int row, int col, ChessType chessType);
private:
	void initScore();
	Situation calculateHorizontalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateVerticalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateDiagonalScore(Board board, int row, int col, ChessType chessType);
	Situation calculateDiagonalScore2(Board board, int row, int col, ChessType chessType);
	Situation getSituation(int len, int slen, bool isBlocked1, bool isBlocked2);
	int score[N_SITUATION];
};