// excel.c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "helpers.h"

//typedef struct token {
//	char* str;
//	struct token* next;
//} TOKEN;

int main(int argc, char* argv[]) {
	int i;
	TOKEN* command;
	int** tempSheet = (int**)malloc(sizeof(int*) * CSV_SIZE);
	for (i = 0; i < CSV_SIZE; i++) {
		tempSheet[i] = (int*)calloc(CSV_SIZE, sizeof(int));
	}
	while ((command = read_command()) != NULL) {
		execute(tempSheet, command);
	}
	return 0;
}

