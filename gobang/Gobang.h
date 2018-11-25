#pragma once
#include "Board.h"
#include "ZMap.h"
#include "SCalculate.h"

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

	bool isValid(int row, int col);
	void human(int row, int col);
	void computer();
	int getComputerRow() { return computerRow; }
	int getComputerCol() { return computerCol; }
	void updateTurnAfterComputer();
	bool isGameOver(int row, int col);

private:

	void addBlack(int row, int col);
	void addWhite(int row, int col);

	BestResult getBestResult(int preBestScore, ChessType chessType, int level);

	int getSearchLevel();

	int computerRow, computerCol;
	int nChess;
	bool isComputerTurn;
	ChessType computerChessType;
	Board board;
	ZMap zmap;
	SCalculate scalculate;
};
