#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define AVAILABLE 0
#define NOT_AVAILABLE 1
enum DIRECTION { NORTH, EAST, WEST, SOUTH, CENTER };

void getDirection(DIRECTION dir[4], int starti, int startj, int endi, int endj, DIRECTION prevDir = CENTER) {
	int index = 0;
	int dirBitVector[4] = { 0 };
	if (prevDir == CENTER) {
		dir[0] = NORTH;
		dir[1] = EAST;
		dir[2] = WEST;
		dir[3] = SOUTH;
		return;
	}
	if (prevDir == SOUTH || prevDir == NORTH) {
		dir[3] = prevDir == SOUTH ? NORTH : SOUTH;
		dirBitVector[dir[3]] = 1;
	}
	else if (prevDir == EAST || prevDir == WEST) {
		dir[3] = prevDir == EAST ? WEST : EAST;
		dirBitVector[dir[3]] = 1;
	}
	if (starti != endi) {
		dir[index] = starti < endi ? SOUTH : NORTH;
		dirBitVector[dir[index]] = 1;
		index++;
	}
	if (startj != endj) {
		dir[index] = startj < endj ? EAST : WEST;
		dirBitVector[dir[index]] = 1;
		index++;
	}
	if (dirBitVector[NORTH] == 0) {
		dir[index++] = NORTH;
		dirBitVector[NORTH] = 1;
	}
	if (dirBitVector[EAST] == 0) {
		dir[index++] = EAST;
		dirBitVector[EAST] = 1;
	}
	if (dirBitVector[WEST] == 0) {
		dir[index++] = WEST;
		dirBitVector[WEST] = 1;
	}
	if (dirBitVector[SOUTH] == 0) {
		dir[index++] = SOUTH;
		dirBitVector[SOUTH] = 1;
	}
}

void printSolution(int maze[][50], int row, int col) {
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

bool safe(int maze[][50], int rowInd, int colInd, int rows, int cols) {
	if (rowInd >= 0 && rowInd < rows && colInd >= 0 && colInd < cols) {
		if (maze[rowInd][colInd] == AVAILABLE) {
			return true;
		}
	}
	return false;
}

bool solve(int maze[][50], int starti, int startj, int endi, int endj, int sol[][50], int m, int n, DIRECTION prevDir = CENTER) {
	int i;
	DIRECTION dir[4];
	maze[starti][startj] = sol[starti][startj] = NOT_AVAILABLE;
	if (starti == endi && startj == endj) {
		return true;
	}
	getDirection(dir, starti, startj, endi, endj, prevDir);
	for (i = 0; i < 4; i++) {
		switch (dir[i]) {

		case NORTH:
			if (safe(maze, starti - 1, startj, m, n)) {
				if (solve(maze, starti - 1, startj, endi, endj, sol, m, n, NORTH)) {
					return true;
				}
			}
			break;

		case EAST:
			if (safe(maze, starti, startj + 1, m, n)) {
				if (solve(maze, starti, startj + 1, endi, endj, sol, m, n, EAST)) {
					return true;
				}
			}
			break;

		case WEST:
			if (safe(maze, starti, startj - 1, m, n)) {
				if (solve(maze, starti, startj - 1, endi, endj, sol, m, n, WEST)) {
					return true;
				}
			}
			break;

		case SOUTH:
			if (safe(maze, starti + 1, startj, m, n)) {
				if (solve(maze, starti + 1, startj, endi, endj, sol, m, n, SOUTH)) {
					return true;
				}
			}
			break;
		}
	}
	maze[starti][startj] = sol[starti][startj] = AVAILABLE;
	return false;
}

void init() {
	int m = 5, n = 4;
	int maze[][50] = {
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 } };
	int sol[][50] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
	int starti = 4, startj = 0;
	int endi = 0, endj = 3;
	if (solve(maze, starti, startj, endi, endj, sol, m, n)) {
		printSolution(sol, m, n);
	}
	else {
		printf("No solution found\n");
	}
}