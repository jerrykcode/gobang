#pragma once
typedef char **Board;
typedef char ChessType;
#define BLACK 'B'
#define WHITE 'W'
#define EMPTY '-'

#define NROWS 15
#define NCOLS 15

typedef struct Coordinate {
	Coordinate(int row, int col) : row(row), col(col) {}
	Coordinate() {}
	int row;
	int col;
} *PCoordinate;
