#include "Gobang.h"
#include <algorithm>

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
	zmap.deleteMap(nChess - 1);
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
	zmap.deleteMap(nChess - 1);
}

void Gobang::bfsGetCoordinates(vector<PCoordinate>& pCoordinates) {
	bool **collected = new bool*[NROWS];
	for (int row = 0; row < NROWS; row++) {
		collected[row] = new bool[NCOLS];
		for (int col = 0; col < NCOLS; col++)
			collected[row][col] = false;
	}
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			if (board[row][col] != EMPTY && !collected[row][col]) {
				queue<PCoordinate> q;
				PCoordinate root = new Coordinate(row, col);
				q.push(root);
				collected[row][col] = true;
				PCoordinate last = root, tail = root;
				int level = 0;
				while (!q.empty()) {
					PCoordinate back = q.back();
					q.pop();
					int bRow = back->row, bCol = back->col;					
					PCoordinate pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow - 1, bCol - 1);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow - 1, bCol);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow - 1, bCol + 1);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow, bCol - 1);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow, bCol + 1);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow + 1, bCol - 1);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow + 1, bCol);
					if (pCoordinate != NULL) last = pCoordinate;
					pCoordinate = bfsGetAround(q, pCoordinates, collected, bRow + 1, bCol + 1);
					if (pCoordinate != NULL) last = pCoordinate;
					if (back == tail) {
						tail = last;
						level++;
					}
					if (level == 2) break;
				}
			}
		}
	for (int row = 0; row < NROWS; row++)
		free(collected[row]);
	free(collected);
}

PCoordinate Gobang::bfsGetAround(queue<PCoordinate>& q, vector<PCoordinate>& pCoordinates, bool **collected, int row, int col) {
	if (row >= 0 && row < NROWS && col >= 0 && col < NCOLS && board[row][col] == EMPTY && !collected[row][col]) {
		PCoordinate pCoordinate = new Coordinate(row, col);
		q.push(pCoordinate);
		pCoordinates.push_back(pCoordinate);
		collected[row][col] = true;
		return pCoordinate;
	}
	return NULL;
}

BestResult Gobang::getBestResult(int preBestScore, ChessType chessType, int level) {
	if (zmap.hasKey(board)) {
		BestResult bestResult = zmap.getValue(board);
		if (bestResult.level >= level) return bestResult;
	}
	//
	SearchType searchType = chessType == computerChessType ? SEARCH_MAX : SEARCH_MIN;
	int bestScore = searchType == SEARCH_MAX ? MIN_SCORE - 1 : MAX_SCORE + 1;
	int bestRow = 0, bestCol = 0;
	bool hasAlphaBeta = false;
	vector<PCoordinate> pCoordinates;
	bfsGetCoordinates(pCoordinates);
	vector<Position> positions;
	for (PCoordinate pCoordinate : pCoordinates) {
		int row = pCoordinate->row, col = pCoordinate->col;
		board[row][col] = chessType;
		int currentScore = scalculate.calculateBoardScore(board, computerChessType);		
		if (level == 1) {
			if (searchType == SEARCH_MAX && currentScore > bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			if (searchType == SEARCH_MIN && currentScore < bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
		}
		board[row][col] = EMPTY;
		positions.push_back(Position(*pCoordinate, currentScore));
	}
	if (level == 1) {
		goto END;
	}
	sort(positions.begin(), positions.end(), compare);		
	for (auto it = positions.begin(); it != positions.end(); it++) {
		int row = it->c.row, col = it->c.col;
		board[row][col] = chessType;
		int currentScore;
		if (scalculate.isGameOver(board, row, col, chessType)) currentScore = chessType == computerChessType ? MAX_SCORE : MIN_SCORE;
		else {
			BestResult bestResult = getBestResult(bestScore, REVERSE_CHESS_TYPE(chessType), level - 1);
			currentScore = bestResult.score;	
		}
		board[row][col] = EMPTY;
		switch (searchType)
		{
		case SEARCH_MAX: {
			if (currentScore > bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			if (preBestScore != NO_ALPHA_BETA && bestScore >= preBestScore) {
				hasAlphaBeta = true;
				goto END;
			}
			break;
		}
		case SEARCH_MIN: {
			if (currentScore < bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			if (preBestScore != NO_ALPHA_BETA && bestScore <= preBestScore) {
				hasAlphaBeta = true;
				goto END;
			}
			break;
		}
		default:
			break;
		}
	} //for
END:
	for (auto it = pCoordinates.begin(); it != pCoordinates.end(); it++) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	vector<PCoordinate>().swap(pCoordinates);
	vector<Position>().swap(positions);
	BestResult bestResult(Coordinate(bestRow, bestCol), level, bestScore);
	if (preBestScore != NO_ALPHA_BETA && !hasAlphaBeta) {
		zmap.insert(board, bestResult);
	}
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
	return 3;
}