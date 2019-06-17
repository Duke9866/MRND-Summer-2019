#include <stdio.h>
#include <stdlib.h>

//printing spiral matrix using iteration
void spiral_iterative(int** mat, int m, int n) {
	int i;
	int k = 0, l = 0;
	while (k < m && l < n) {
		for (i = l; i < n; i++) {
			printf("%d ", mat[k][i]);
		}
		k++;
		for (i = k; i < m; i++) {
			printf("%d ", mat[i][n - 1]);
		}
		n--;
		if (k < m)
		for (i = n - 1; i >= l; i--) {
			printf("%d ", mat[m - 1][i]);
		}
		m--;
		if (l < n)
		for (i = m - 1; i >= k; i--) {
			printf("%d ", mat[i][l]);
		}
		l++;
	}
}

//printing spiral matrix using recursion
void spiral_recursive(int **mat, int rowStart, int colStart, int rowLength, int colLength) {
	int i;
	for (i = rowStart; i <= colLength; i++) {
		printf("%d ", mat[rowStart][i]);
	}
	for (i = rowStart + 1; i <= rowLength; i++) {
		printf("%d ", mat[i][colLength]);
	}
	if (rowStart + 1 <= rowLength){
		for (i = colLength - 1; i >= colStart; i--) {
			printf("%d ", mat[rowLength][i]);
		}
	}
	if (colStart + 1 <= colLength) {
		for (i = rowLength - 1; i > rowStart; i--) {
			printf("%d ", mat[i][colStart]);
		}
	}
	if (rowStart + 1 <= rowLength - 1 && colStart + 1 <= colLength - 1){
		spiral_recursive(mat, rowStart + 1, colStart + 1, rowLength - 1, colLength - 1);
	}
}

void printMatrix(int** mat, int m, int n) {
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int** input() {
	int** mat;
	int i, j, counter = 1, m = 4, n = 3;
	mat = (int**)malloc(sizeof(int*) * m);
	for (i = 0; i < m; i++) {
		mat[i] = (int*)malloc(sizeof(int) * n);
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			mat[i][j] = counter++;
		}
	}
	return mat;
}

void init() {
	int m = 4, n = 3;
	int** mat = input();
	printMatrix(mat, m, n);
	
	spiral_iterative(mat, m, n);
	printf("\n");
	spiral_recursive(mat, 0, 0, m - 1, n - 1);

}