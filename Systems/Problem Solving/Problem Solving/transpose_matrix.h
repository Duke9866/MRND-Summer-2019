#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
	int** mat;
	int m;
	int n;
} matrix;

matrix* new_matrix(int m, int n) {
	int i;
	matrix* inpMat = (matrix*)malloc(sizeof(matrix));
	inpMat->m = m;
	inpMat->n = n;
	inpMat->mat = (int**)malloc(sizeof(int*) * m);
	for (i = 0; i < m; i++) {
		inpMat->mat[i] = (int*)calloc(n, sizeof(int));
	}
	return inpMat;
}

matrix* transpose(matrix* inpMat) {
	int i, j;
	matrix* outpMat = new_matrix(inpMat->n, inpMat->m);
	for (i = 0; i < inpMat->m; i++) {
		for (j = 0; j < inpMat->n; j++) {
			outpMat->mat[j][i] = inpMat->mat[i][j];
		}
	}
	return outpMat;
}

void printMatrix(matrix* inpMat) {
	int i, j;
	for (i = 0; i < inpMat->m; i++) {
		for (j = 0; j < inpMat->n; j++) {
			printf("%d ", inpMat->mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

matrix* input() {
	int m = 4, n = 3;
	int i, j, counter = 1;
	matrix* inpMat = new_matrix(m, n);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			inpMat->mat[i][j] = counter++;
		}
	}
	return inpMat;
}

void init() {
	matrix* inpMat = input();
	printMatrix(inpMat);
	matrix* outpMat = transpose(inpMat);
	printMatrix(outpMat);
}