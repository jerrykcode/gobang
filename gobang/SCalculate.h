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

//���������
class SCalculate {
public:
	SCalculate();
	~SCalculate();
	
	/* ����(row, col)�������chessType���͵ķ���������pBestSituation��¼(row, col)��������situation */
	int calculatePointScore(Board board, int row, int col, ChessType chessType, PSituation pBestSituation);

	/* �����������̶���chessType���͵ķ��� */
	int calculateBoardScore(Board board, ChessType chessType);

	/* �ж���Ϸ�Ƿ���Ϊ��(row, col)�������Ӷ����� */
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