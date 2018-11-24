#pragma once
#include "Board.h"

class Gobang {

public:
	Gobang() {}
	~Gobang(){}

	bool isValid(int row, int col);
	void human(int row, int col);
	void computer();
	int getComputerRow() { return computerRow; }
	int getComputerCol() { return computerCol; }
	void updateTurnAfterComputer();
	bool isGameOver(int row, int col);
private:

	int computerRow, computerCol;

	bool isComputerTurn;
	ChessType computerChessType;
	Board board;
};
