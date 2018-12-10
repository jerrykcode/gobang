#pragma once
typedef char **Board;
typedef char ChessType;
typedef unsigned char BYTE;
#define BLACK 'B'
#define WHITE 'W'
#define EMPTY '-'
#define REVERSE_CHESS_TYPE(type) (type == EMPTY ? EMPTY : (type == BLACK ? WHITE : BLACK))

#define NROWS 15
#define NCOLS 15

typedef struct Coordinate {
	Coordinate(int row, int col) {
		c = (BYTE)row * NROWS + (BYTE)col;
	}
	Coordinate() {}
	int row() {
		return c / NROWS;
	}
	int col() {
		return c % NROWS;
	}
private:
	BYTE c;
} *PCoordinate;
