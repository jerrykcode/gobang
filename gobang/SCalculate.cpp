#include "SCalculate.h"

SCalculate::SCalculate() {
	initScore();
}

SCalculate::~SCalculate() {

}

void SCalculate::initScore() {
	//各种情况对应的分数
	score[NONE] = 0;
	score[BLOCKED_ONE] = 1;
	score[ONE] = 10;
	score[BLOCKED_TWO] = 10;
	score[TWO] = 100;
	score[BLOCKED_THREE] = 100;
	score[THREE] = 1000;
	score[BLOCKED_FOUR] = 2000;
	score[FOUR] = 10000;
	score[FIVE] = 100000000;
}

int SCalculate::calculatePointScore(Board board, int row, int col, ChessType chessType, PSituation pBestSituation) {
	if (board[row][col] != chessType) return 0;
	int result = 0;
	Situation bestSituation = NONE, currentSituation;

	//水平方向
	currentSituation = calculateHorizontalScore(board, row, col, chessType);
	if (currentSituation > bestSituation) bestSituation = currentSituation;
	result += score[currentSituation];

	//竖直方向
	currentSituation = calculateVerticalScore(board, row, col, chessType);
	if (currentSituation > bestSituation) bestSituation = currentSituation;
	result += score[currentSituation];

	//对角线
	currentSituation = calculateDiagonalScore(board, row, col, chessType);
	if (currentSituation > bestSituation) bestSituation = currentSituation;
	result += score[currentSituation];

	//对角线2
	currentSituation = calculateDiagonalScore2(board, row, col, chessType);
	if (currentSituation > bestSituation) bestSituation = currentSituation;
	result += score[currentSituation];

	*pBestSituation = bestSituation;

	return result;
}

int SCalculate::calculateBoardScore(Board board, ChessType chessType) {
	int result = 0;	
	Situation bestSituation = NONE, tmpSituation;
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) {
			//(row, col)为空, continue
			if (board[row][col] == EMPTY) continue;
			//(row, col)为chessType类型棋子，(row, col)坐标的分数加在result上
			else if (board[row][col] == chessType) result += calculatePointScore(board, row, col, chessType, &tmpSituation);
			else {
				//(row, col)为反方棋子，(row, col)坐标的分数被result减去
				int s = calculatePointScore(board, row, col, REVERSE_CHESS_TYPE(chessType), &tmpSituation);
				result -= s;
				if (tmpSituation > bestSituation) bestSituation = tmpSituation;
			}
		}
	//调用calculateBoardScore时，是在chessType类型落子后，而反方尚未落子的情况下。故对于反方最好的situation，反方可落子
	//使其变为对他更好的situation。如反方有一个FOUR， 他可以落子使其变为FIVE。
	switch (bestSituation) {
	case NONE: break;
	case BLOCKED_ONE: result += score[BLOCKED_ONE]; result -= score[BLOCKED_TWO]; break;
	case ONE: result += score[ONE]; result -= score[TWO]; break;
	case BLOCKED_TWO: result += score[BLOCKED_TWO]; result -= score[BLOCKED_THREE]; break;
	case TWO: result += score[TWO]; result -= score[THREE]; break;
	case BLOCKED_THREE: result += score[BLOCKED_THREE]; result -= score[BLOCKED_FOUR]; break;
	case THREE: result += score[THREE]; result -= score[FOUR]; break;
	case BLOCKED_FOUR: case FOUR: result += score[bestSituation]; result -= score[FIVE]; break;
	case FIVE: break;
	default: break;
	}
	return result;
}

bool SCalculate::isGameOver(Board board, int row, int col, ChessType chessType) {
	if (board[row][col] != chessType) return false;
	//若四个方向中有一个成FIVE，则游戏结束
	if (calculateHorizontalScore(board, row, col, chessType) == FIVE) return true;
	if (calculateVerticalScore(board, row, col, chessType) == FIVE) return true;
	if (calculateDiagonalScore(board, row, col, chessType) == FIVE) return true;
	if (calculateDiagonalScore2(board, row, col, chessType) == FIVE) return true;
	return false;
}

Situation SCalculate::calculateHorizontalScore(Board board, int row, int col, ChessType chessType) {	
	if (board[row][col] != chessType) return NONE;
	int leftEndCol = col, rightEndCol = col;
	while (leftEndCol - 1 >= 0 && board[row][leftEndCol - 1] == chessType) leftEndCol--; //向左
	while (rightEndCol + 1 < NCOLS && board[row][rightEndCol + 1] == chessType) rightEndCol++; //向右
	int len = rightEndCol - leftEndCol + 1; 
	int slen = len; //连续的chessType类型的棋子长度
	//若左边为空且左边的左边还是chessType类型的棋子，则可以向左边延续
	bool canLeft = leftEndCol - 2 >= 0 && board[row][leftEndCol - 1] == EMPTY && board[row][leftEndCol - 2] == chessType;
	//若右边为空且右边的右边还是chessType类型的棋子，则可以向右边延续
	bool canRight = rightEndCol + 2 < NCOLS && board[row][rightEndCol + 1] == EMPTY && board[row][rightEndCol + 2] == chessType;
	int leftGain = 0, rightGain = 0; //左边延续数量与右边延续数量
	if (canLeft) {
		leftGain = 1;
		while (leftEndCol - 1 - leftGain >= 0 && board[row][leftEndCol - 1 - leftGain] == chessType)
			leftGain++;
		leftGain--;
	}
	if (canRight) {
		rightGain = 1;
		while (rightEndCol + 1 + rightGain < NCOLS && board[row][rightEndCol + 1 + rightGain] == chessType)
			rightGain++;
		rightGain--;
	}
	if (leftGain != 0 || rightGain != 0) {
		//左右只能往一个方向延续
		if (leftGain > rightGain) {
			len += leftGain;
			leftEndCol = leftEndCol - 1 - leftGain;
		}
		else {
			len += rightGain;
			rightEndCol = rightEndCol + 1 + rightGain;
		}
	}	
	bool isLeftBlocked = (leftEndCol - 1 < 0 || board[row][leftEndCol - 1] != EMPTY); //左边是否阻塞
	bool isRightBlocked = (rightEndCol + 1 >= NCOLS || board[row][rightEndCol + 1] != EMPTY); //右边是否阻塞
	return getSituation(len, slen, isLeftBlocked, isRightBlocked);
}

Situation SCalculate::calculateVerticalScore(Board board, int row, int col, ChessType chessType) {
	if (board[row][col] != chessType) return NONE;
	int topEndRow = row, bottomEndRow = row;
	while (topEndRow - 1 >= 0 && board[topEndRow - 1][col] == chessType) topEndRow--;
	while (bottomEndRow + 1 < NROWS && board[bottomEndRow + 1][col] == chessType) bottomEndRow++;
	int len = bottomEndRow - topEndRow + 1;
	int slen = len;
	bool canUp = topEndRow - 2 >= 0 && board[topEndRow - 1][col] == EMPTY && board[topEndRow - 2][col] == chessType;
	bool canDown = bottomEndRow + 2 < NROWS && board[bottomEndRow + 1][col] == EMPTY && board[bottomEndRow + 2][col] == chessType;
	int topGain = 0, bottomGain = 0;
	if (canUp) {
		topGain = 1;
		while (topEndRow - 1 - topGain >= 0 && board[topEndRow - 1 - topGain][col] == chessType)
			topGain++;
		topGain--;
	}
	if (canDown) {
		bottomGain = 1;
		while (bottomEndRow + 1 + bottomGain < NROWS && board[bottomEndRow + 1 + bottomGain][col] == chessType)
			bottomGain++;
		bottomGain--;
	}
	if (topGain != 0 || bottomGain != 0) {
		if (topGain > bottomGain) {
			len += topGain;
			topEndRow = topEndRow - 1 - topGain;
		}
		else {
			len += bottomGain;
			bottomEndRow = bottomEndRow + 1 + bottomGain;
		}
	}
	bool isTopBlocked = topEndRow - 1 < 0 || board[topEndRow - 1][col] != EMPTY;
	bool isBottomBlocked = bottomEndRow + 1 >= NROWS || board[bottomEndRow + 1][col] != EMPTY;
	return getSituation(len, slen, isTopBlocked, isBottomBlocked);
}

Situation SCalculate::calculateDiagonalScore(Board board, int row, int col, ChessType chessType) {
	if (board[row][col] != chessType) return NONE;
	int leftLen = 0, rightLen = 0;
	while (row - leftLen - 1 >= 0 && col - leftLen - 1 >= 0 && board[row - leftLen - 1][col - leftLen - 1] == chessType)
		leftLen++;
	while (row + rightLen + 1 < NROWS && row + rightLen + 1 < NROWS && board[row + rightLen + 1][col + rightLen + 1] == chessType)
		rightLen++;
	int len = leftLen + rightLen + 1;
	int slen = len;
	bool canLeft = row - leftLen - 2 >= 0 && col - leftLen - 2 >= 0 && board[row - leftLen - 1][col - leftLen - 1] == EMPTY
		&& board[row - leftLen - 2][col - leftLen - 2] == chessType;
	bool canRight = row + rightLen + 2 < NROWS && col + rightLen + 2 < NROWS && board[row + rightLen + 1][col + rightLen + 1] == EMPTY
		&& board[row + rightLen + 2][col + rightLen + 2] == chessType;
	int leftGain = 0, rightGain = 0;
	if (canLeft) {
		leftGain = 1;
		while (row - leftLen - 1 - leftGain >= 0 && col - leftLen - 1 - leftGain >= 0
			&& board[row - leftLen - 1 - leftGain][col - leftLen - 1 - leftGain] == chessType)
			leftGain++;
		leftGain--;
	}
	if (canRight) {
		rightGain = 1;
		while (row + rightLen + 1 + rightGain < NROWS && col + rightLen + 1 + rightGain < NROWS
			&& board[row + rightLen + 1 + rightGain][col + rightLen + 1 + rightGain] == chessType)
			rightGain++;
		rightGain--;
	}
	if (leftGain != 0 || rightGain != 0) {
		if (leftGain > rightGain) {
			len += leftGain;
			leftLen += leftGain + 1;
		}
		else {
			len += rightGain;
			rightLen += rightGain + 1;
		}
	}
	bool isLeftBlocked = row - leftLen - 1 < 0 || col - leftLen - 1 < 0 || board[row - leftLen - 1][col - leftLen - 1] != EMPTY;
	bool isRightBlocked = row + rightLen + 1 >= NROWS || col + rightLen + 1 >= NCOLS || board[row + rightLen + 1][col + rightLen + 1] != EMPTY;
	return getSituation(len, slen, isLeftBlocked, isRightBlocked);
}

Situation SCalculate::calculateDiagonalScore2(Board board, int row, int col, ChessType chessType) {
	if (board[row][col] != chessType) return NONE;
	int leftLen = 0, rightLen = 0;
	while (row + leftLen + 1 < NROWS && col - leftLen - 1 >= 0 && board[row + leftLen + 1][col - leftLen - 1] == chessType)
		leftLen++;
	while (row - rightLen - 1 >= 0 && col + rightLen + 1 < NCOLS && board[row - rightLen - 1][col + rightLen + 1] == chessType)
		rightLen++;
	int len = leftLen + rightLen + 1;
	int slen = len;
	bool canLeft = row + leftLen + 2 < NROWS && col - leftLen - 2 >= 0 && board[row + leftLen + 1][col - leftLen - 1] == EMPTY
		&& board[row + leftLen + 2][col - leftLen - 2] == chessType;
	bool canRight = row - rightLen - 2 >= 0 && col + rightLen + 2 < NCOLS && board[row - rightLen - 1][col + rightLen + 1] == EMPTY
		&& board[row - rightLen - 2][col + rightLen + 2] == chessType;
	int leftGain = 0, rightGain = 0;
	if (canLeft) {
		leftGain = 1;
		while (row + leftLen + 1 + leftGain < NROWS && col - leftLen - 1 - leftGain >= 0
			&& board[row + leftLen + 1 + leftGain][col - leftLen - 1 - leftGain] == chessType)
			leftGain++;
		leftGain--;
	}
	if (canRight) {
		rightGain = 1;
		while (row - rightLen - 1 - rightGain >= 0 && col + rightLen + 1 + rightGain < NCOLS
			&& board[row - rightLen - 1 - rightGain][col + rightLen + 1 + rightGain] == chessType)
			rightGain++;
		rightGain--;
	}
	if (leftGain != 0 || rightGain != 0) {
		if (leftGain > rightGain) {
			len += leftGain;
			leftLen += leftGain + 1;
		}
		else {
			len += rightGain;
			rightLen += rightGain + 1;
		}
	}
	bool isLeftBlocked = row + leftLen + 1 >= NROWS || col - leftLen - 1 < 0 || board[row + leftLen + 1][col - leftLen - 1] != EMPTY;
	bool isRightBlocked = row - rightLen - 1 < 0 || col + rightLen + 1 >= NCOLS || board[row - rightLen - 1][col + rightLen + 1] != EMPTY;
	return getSituation(len, slen, isLeftBlocked, isRightBlocked);
}

Situation SCalculate::getSituation(int len, int slen, bool isBlocked1, bool isBlocked2)
{
	Situation situation;
	if (slen >= 5) return FIVE; //连续的数量大于等于5
	if (len >= 5) return FOUR;
	if (isBlocked1 && isBlocked2) return NONE; //两边均阻塞
	switch (len)
	{
	case 0: situation = NONE; break;
	case 1: {
		if (isBlocked1 || isBlocked2) situation = BLOCKED_ONE;
		else situation = ONE;
		break;
	}
	case 2: {
		if (isBlocked1 || isBlocked2) situation = BLOCKED_TWO;
		else situation = TWO;
		break;
	}
	case 3: {
		if (isBlocked1 || isBlocked2) situation = BLOCKED_THREE;
		else situation = THREE;
		break;
	}
	case 4: {
		if (isBlocked1 || isBlocked2) situation = BLOCKED_FOUR;
		else situation = FOUR;
		break;
	}
	default:
		situation = NONE;
		break;
	}
	return situation;
}
