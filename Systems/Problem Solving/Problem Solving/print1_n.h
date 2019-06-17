#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int getNumber(int* state, int n) {
	int i, num = 0;
	for (i = 0; i < n; i++) {
		num = num * 10 + state[i];
	}
	return num;
}

char* getMirror(int num) {
	int len = 0;
	char* res = (char*)malloc(sizeof(char) * 5);
	char* map = "012Eh59L86";
	while (num) {
		res[len++] = map[num % 10];
		num /= 10;
	}
	res[len] = '\0';
	_strrev(res);
	return res;
}

void print1_NUtil(int n, int* state, int level = 0) {
	int i, num;
	if (level == n - 1) {
		if (num = getNumber(state, n - 1)) {
			printf("%d\n", num);
		}
		return;
	}
	for (i = 0; i < 10; i++) {
		state[level] = i;
		print1_NUtil(n, state, level + 1);
	}
}

void printMirror1_NUtil(int n, int* state, int level = 0) {
	int i, num;
	if (level == n - 1) {
		if (num = getNumber(state, n - 1)) {
			printf("%s\n", getMirror(num));
		}
		return;
	}
	for (i = 0; i < 10; i++) {
		state[level] = i;
		printMirror1_NUtil(n, state, level + 1);
	}
}

void printMirror1_N(int n) {
	int* state = (int*)calloc(n - 1, sizeof(int));
	printMirror1_NUtil(n, state);
}

void print1_N(int n) {
	int* state = (int*)calloc(n - 1, sizeof(int));
	print1_NUtil(n, state);
}

void init() {
	int n = 2;
	//print1_N(n);
	printMirror1_N(n);
}