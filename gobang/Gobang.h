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

	/* �ж��û���(row, col)���������Ƿ�valid */
	bool isValid(int row, int col);

	/* �û���(row, col)�������ӣ�����board */
	void human(int row, int col);

	/* �������ӣ� ����board */
	void computer();

	/* ���ص������ӵ�row���� */
	int getComputerRow() { return computerRow; }

	/* ���ص������ӵ�col���� */
	int getComputerCol() { return computerCol; }

	/* �������Ӻ����Ϊ���û����� */
	void updateTurnAfterComputer();

	/* �ж���Ϸ�Ƿ���Ϊ��(row, col)�������Ӷ����� */
	bool isGameOver(int row, int col);

private:

	/* ��(row, col)�����º��� */
	void addBlack(int row, int col);

	/* ��(row, col)�����°��� */
	void addWhite(int row, int col);

	/* MAX-MIN����(alpha-beta��֦��zobrist����) */
	BestResult getBestResult(int preBestScore, ChessType chessType, int level);

	/* ��pCoordinates������Ҫ��������� */
	void bfsGetCoordinates(vector<PCoordinate>& pCoordinates);
	PCoordinate bfsGetAround(queue<PCoordinate>& q, vector<PCoordinate>& pCoordinates, bool **collected, int row, int col);

	/* ������������ */
	int getSearchLevel();

	int computerRow, computerCol; //����������ӵ�����
	int nChess; //�����ϵ���������
	bool isComputerTurn; //���õ�������Ϊtrue
	ChessType computerChessType; //������������
	Board board;
	ZMap zmap;
	SCalculate scalculate;
};
