#include "Gobang.h"
#include <algorithm>
#include <vector>
using namespace std;

#define NO_ALPHA_BETA (MIN_SCORE - 1)

typedef enum {
	SEARCH_MAX,
	SEARCH_MIN,
} SearchType;

struct Position {
	Position(Coordinate c, int score) : c(c), score(score) {}
	Position() {}
	Coordinate c;
	int score;
};
bool compare(Position a, Position b) {
	return a.score > b.score;
}

bool Gobang::isValid(int row, int col) {
	if (isComputerTurn) return false;
	return board[row][col] == EMPTY;
}

void Gobang::human(int row, int col) {
	if (computerChessType == BLACK)
		addWhite(row, col);
	else
		addBlack(row, col);
	nChess++;
	isComputerTurn = true;
}

void Gobang::addBlack(int row, int col) {
	board[row][col] = BLACK;
}

void Gobang::addWhite(int row, int col) {
	board[row][col] = WHITE;
}

void Gobang::computer() {
	BestResult bestResult = getBestResult(NO_ALPHA_BETA, computerChessType, getSearchLevel());
	computerRow = bestResult.c.row;
	computerCol = bestResult.c.col;
	if (computerChessType == BLACK)
		addBlack(computerRow, computerCol);
	else addWhite(computerRow, computerCol);
	nChess++;
}

BestResult Gobang::getBestResult(int preBestScore, ChessType chessType, int level) {
	if (zmap.hasKey(board)) {
		BestResult bestRes = zmap.getValue(board);
		if (bestRes.level >= level) {
			return bestRes;
		}
	}
	SearchType searchType = chessType == computerChessType ? SEARCH_MAX : SEARCH_MIN;
	vector<Position> positions;
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			if (board[row][col] == EMPTY) {
				board[row][col] = chessType;
				int score = scalculate.calculatePointScore(board, row, col, chessType);
				board[row][col] = EMPTY;
				positions.push_back(Position(Coordinate(row, col), score));
			}
		}
	sort(positions.begin(), positions.end(), compare);
	int bestScore = searchType == SEARCH_MAX ? MIN_SCORE : MAX_SCORE;
	int bestRow = 0, bestCol = 0;
	for (auto it = positions.begin(); it != positions.end(); it++) {
		int row = it->c.row, col = it->c.col;		
		int currentScore;
		board[row][col] = chessType;
		if (level == 1) {
			currentScore = scalculate.calculateBoardScore(board, chessType);
		}
		else {
			BestResult result = getBestResult(bestScore, REVERSE_CHESS_TYPE(chessType), level - 1);
			currentScore = result.score;
		}
		board[row][col] = EMPTY;
		switch (searchType) {
		case SEARCH_MAX: {
			if (currentScore > bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			if (preBestScore != NO_ALPHA_BETA && currentScore > preBestScore) {
				return BestResult(Coordinate(row, col), level, currentScore);
			}
			break;
		}
		case SEARCH_MIN: {
			if (currentScore < bestScore) {
				bestScore = currentScore;
				bestRow = row;
			}
			if (preBestScore != NO_ALPHA_BETA && currentScore < preBestScore) {
				return BestResult(Coordinate(row, col), level, currentScore);
			}
		}
		default:
			break;
		}
	} //for
	BestResult bestResult(Coordinate(bestRow, bestCol), level, bestScore);
	zmap.insert(board, bestResult);
	return bestResult;
}

void Gobang::updateTurnAfterComputer() {
	isComputerTurn = false;
}

bool Gobang::isGameOver(int row, int col) {
	if (board[row][col] == EMPTY) return false;
	return scalculate.isGameOver(board, row, col, board[row][col]);
}

int Gobang::getSearchLevel() {
	if (nChess < 5)
		return 4;
	else if (nChess < 20)
		return 5;
	else if (nChess < 70)
		return 8;
	else return 10;
	return 0;
}