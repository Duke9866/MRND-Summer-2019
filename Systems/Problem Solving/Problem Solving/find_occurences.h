#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pair {
	int x;
	int y;
} pair;

pair* new_pair(int x, int y) {
	pair* p = (pair*)malloc(sizeof(pair));
	p->x = x;
	p->y = y;
	return p;
}

pair* checkOccurence(char* str, char input[][4], int row, int col, int rowInc, int colInc, int pos = 0) {
	if (str[pos] == '\0') {
		return new_pair(rowInc, colInc);
	}
	if (input[row][col] == str[pos]) {
		row += pos * rowInc;
		col += pos * colInc;
		checkOccurence(str, input, row, col, rowInc, colInc, pos + 1);
	}
	else {
		return NULL;
	}
}

int traverse(char* str, char input[][4], int row, int col, int nrows, int ncols) {
	int count = 0;
	pair *p;
	//travel west
	p = checkOccurence(str, input, row, col, 0, -1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}
	
	//travel east
	p = checkOccurence(str, input, row, col, 0, 1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}
	
	//travel north
	p = checkOccurence(str, input, row, col, -1, 0);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}
	
	//travel south
	p = checkOccurence(str, input, row, col, 1, 0);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}

	//travel north-west
	p = checkOccurence(str, input, row, col, -1, -1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}

	//travel north-east
	p = checkOccurence(str, input, row, col, -1, 1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}

	//travel south-west
	p = checkOccurence(str, input, row, col, 1, -1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}

	//travel south-east
	p = checkOccurence(str, input, row, col, 1, 1);
	if (p != NULL) {
		printf("(%d, %d)->(%d, %d)\n", row, col, p->x, p->y);
		count++;
	}

	return count;
}

void rec_find_occurences(char* str, char input[][4], int rows, int cols, int* count, int row_start = 0, int col_start = 0) {
	if (col_start == cols) {
		col_start = 0;
		row_start += 1;
	}
	else if (row_start == rows) {
		return;
	}
	if (input[row_start][col_start] == str[0]) {
		*count = traverse(str, input, row_start, col_start, rows, cols);
	}
	rec_find_occurences(str, input, rows, cols, count, row_start, col_start + 1);
}

//iterative method
int find_occurences(char* str, char input[][4], int m, int n) {
	int i, j, k;
	int ti, tj;
	int count = 0;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if (input[i][j] == str[0]) {
				//checking row right
				ti = i;
				tj = j;
				k = 0;
				while (tj < n && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					tj++;
					k++;
				}
				if (str[k] == '\0') {
					tj--;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking row left
				ti = i;
				tj = j;
				k = 0;
				while (tj >= 0 && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					tj--;
					k++;
				}
				if (str[k] == '\0') {
					tj++;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking column up
				ti = i;
				tj = j;
				k = 0;
				while (ti >= 0 && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti--;
					k++;
				}
				if (str[k] == '\0') {
					tj++;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking column down
				ti = i;
				tj = j;
				k = 0;
				while (ti < m && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti++;
					k++;
				}
				if (str[k] == '\0') {
					ti--;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking left diagonal down
				ti = i;
				tj = j;
				k = 0;
				while (ti < m && tj < n && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti++;
					tj++;
					k++;
				}
				if (str[k] == '\0') {
					ti--;
					tj--;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking left diagonal up
				ti = i;
				tj = j;
				k = 0;
				while (ti >= 0 && tj >= 0 && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti--;
					tj--;
					k++;
				}
				if (str[k] == '\0') {
					ti++;
					tj++;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking right diagonal down
				ti = i;
				tj = j;
				k = 0;
				while (ti < m && tj >= 0 && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti++;
					tj--;
					k++;
				}
				if (str[k] == '\0') {
					ti--;
					tj++;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}

				//checking right diagonal up
				ti = i;
				tj = j;
				k = 0;
				while (ti >= 0 && tj < n && str[k]) {
					if (str[k] != input[ti][tj]) {
						break;
					}
					ti--;
					tj++;
					k++;
				}
				if (str[k] == '\0') {
					ti++;
					tj--;
					printf("(%d, %d)->(%d, %d)\n", i, j, ti, tj);
					count++;
				}
			}
		}
	}
	return count;
}

void init() {
	char str[] = "cat";
	char inp[][4] = { 
	{ 'c', 'a', 't', 'c' },
	{ 'a', 'a', 'a', 't' },
	{ 't', 't', 'a', 'c' },
	{ 't', 'c', 'a', 't' } };
	int m = 4, n = 4;
	int* count = (int*)malloc(sizeof(int));
	*count = 0;
	rec_find_occurences(str, inp, m, n, count);
	printf("%d\n", count);
}