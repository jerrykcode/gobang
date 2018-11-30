#include "Gobang.h"
#include <algorithm>

#define NO_ALPHA_BETA (MIN_SCORE - 2)

//搜索类型
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
	zmap.deleteMap(nChess - 1); //当前已有nChess颗棋子，(nChess - 1)颗棋子情况的缓存可以删除
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
	zmap.deleteMap(nChess - 1); //当前已有nChess颗棋子，(nChess - 1)颗棋子情况的缓存可以删除
}

void Gobang::bfsGetCoordinates(vector<PCoordinate>& pCoordinates) {
	bool **collected = new bool*[NROWS];
	for (int row = 0; row < NROWS; row++) {
		collected[row] = new bool[NCOLS];
		for (int col = 0; col < NCOLS; col++)
			collected[row][col] = false;
	}
	//遍历棋盘上的所有坐标
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			if (board[row][col] != EMPTY && !collected[row][col]) {
				//BFS
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
					//周围8个坐标
					//记录最后进队列的为last
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
					if (back == tail) { //出队列的是上一层最后一个，进入新一层
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
	//该坐标存在且该坐标为空且未被收录过
	if (row >= 0 && row < NROWS && col >= 0 && col < NCOLS && board[row][col] == EMPTY && !collected[row][col]) {
		PCoordinate pCoordinate = new Coordinate(row, col);
		//注意队列及vector存储的是指针
		q.push(pCoordinate);
		pCoordinates.push_back(pCoordinate);
		collected[row][col] = true; //收录
		return pCoordinate;
	}
	return NULL;
}

BestResult Gobang::getBestResult(int preBestScore, ChessType chessType, int level) {
	if (zmap.hasKey(board)) { //缓存中有board
		BestResult bestResult = zmap.getValue(board);
		if (bestResult.level >= level) return bestResult; //缓存中的结果的搜索层数足够
	}
	vector<PCoordinate> pCoordinates;
	bfsGetCoordinates(pCoordinates); //bfs计算可能落子的坐标，vector存储的是Coordinate的地址
	vector<Position> positions; //可能落子的坐标及在该坐标的分数
	//若chessType与computerChessType相同，则找最大分，否则找最小分
	SearchType searchType = chessType == computerChessType ? SEARCH_MAX : SEARCH_MIN;
	int bestScore = searchType == SEARCH_MAX ? MIN_SCORE - 1 : MAX_SCORE + 1;
	int bestRow = 0, bestCol = 0;
	bool hasAlphaBetaPruning = false; //标记是否剪枝
	for (PCoordinate pCoordinate : pCoordinates) { //遍历坐标
		int row = pCoordinate->row, col = pCoordinate->col;
		board[row][col] = chessType; //在该坐标落子
		int currentScore = scalculate.calculateBoardScore(board, chessType); //计算分数
		board[row][col] = EMPTY; //还原为EMPTY
		if (level == 1) { //若只搜索一层，则currentScore即在该坐标落子的最终分数
			if (searchType == SEARCH_MAX && currentScore > bestScore) { //MAX层搜索最大分数
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			else if (searchType == SEARCH_MIN && currentScore < bestScore) { //MIN层搜索最小分数
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
		}
		else { //若不止搜索一层，记录当前坐标与分数
			positions.push_back(Position(*pCoordinate, currentScore));
		}
	}
	if (level == 1)
		goto END;
	sort(positions.begin(), positions.end(), compare); //按计算一层的分数排序，排序后更接近按最终分数的排序，可增加剪枝数量
	for (auto it = positions.begin(); it != positions.end(); it++) {
		int row = it->c.row, col = it->c.col;
		board[row][col] = chessType; //在该坐标落子
		int currentScore;
		if (scalculate.isGameOver(board, row, col, chessType)) { //若落子后游戏结束
			//若电脑获胜则分数为最大值，否则为最小值
			currentScore = chessType == computerChessType ? MAX_SCORE : MIN_SCORE; 
		}
		else { //若落子后没有结束
			//递归，当前bestScore作为下一层的preBestScore用于剪枝
			BestResult result = getBestResult(bestScore, REVERSE_CHESS_TYPE(chessType), level - 1); //层数减少
			currentScore = result.score;
		}
		board[row][col] = EMPTY; //还原为EMPTY
		switch (searchType)
		{
		case SEARCH_MAX: { //若搜索最大分数
			if (currentScore > bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			//若可以剪枝(preBestScore != NO_ALPHA_BETA)
			//currentScore >= preBestScore，而本层搜索最大分数，故返回到上一层的分数最小为preBestScore,
			//而上一层搜索最小分数，需要<preBestScore才能更新，故本层返回的结果不可能被上一层使用，应剪枝
			if (preBestScore != NO_ALPHA_BETA && currentScore >= preBestScore) {
				hasAlphaBetaPruning = true;
				goto END;
			}
			break;
		}
		case SEARCH_MIN: { //若搜索最小分数
			if (currentScore < bestScore) {
				bestScore = currentScore;
				bestRow = row;
				bestCol = col;
			}
			//同上应剪枝
			if (preBestScore != NO_ALPHA_BETA && currentScore <= preBestScore) {
				hasAlphaBetaPruning = true;
				goto END;
			}
			break;
		}
		default: break;
		}		
	}
END:
	//释放内存
	for (auto it = pCoordinates.begin(); it != pCoordinates.end(); it++) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	vector<PCoordinate>().swap(pCoordinates);
	vector<Position>().swap(positions);
	BestResult bestResult(Coordinate(bestRow, bestCol), level, bestScore);
	//剪枝返回的结果并不是最佳结果，不能缓存
	if (preBestScore != NO_ALPHA_BETA && !hasAlphaBetaPruning) {
		//缓存
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