#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

char* getFileName(char* fname) {
	if (!strstr(fname, ".csv")) {
		strcat(fname, ".csv");
	}
	return fname;
}

char* scanTokenString(FILE* buf, char delimiter) {
	int len = 0;
	char* res = (char*)malloc(sizeof(char) * 50);
	char ch;
	while ((ch = fgetc(buf)) == ' ');
	while (ch != delimiter && ch != '\n' && ch != ' ' && ch != EOF) {
		res[len++] = ch;
		ch = fgetc(buf);
	}
	res[len] = '\0';
	return res;
}

int scanTokenValue(FILE* buf) {
	char ch;
	int value;
	while ((ch = fgetc(buf)) < '0' || ch > '9');
	fseek(buf, -1, SEEK_CUR);
	fscanf(buf, "%d", &value);
	return value;
}

TOKEN* new_token() {
	TOKEN* t = (TOKEN*)malloc(sizeof(TOKEN));
	t->str = (char*)malloc(sizeof(char) * 50);
	t->next = NULL;
	return t;
}

TOKEN* read_command() {
	TOKEN* tokens = new_token(), *head = NULL;
	head = tokens;
	printf(">");
	fflush(stdin);
	FILE *fptr = fopen("inp_buffer.txt", "w+");
	char ch;
	while ((ch = getchar()) != '\n') {
		putc(ch, fptr);
	}
	fseek(fptr, 0, SEEK_SET);
	tokens->str = scanTokenString(fptr);
	if (strcmp(tokens->str, "GET") == 0) {
		tokens->next = new_token();
		tokens = tokens->next;
		tokens->str = scanTokenString(fptr);
		//fscanf(fptr, "%s", tokens->str);
	}
	else if (strcmp(tokens->str, "SET") == 0) {
		//while ((ch = fgetc(fptr)) == ' ');
		tokens->next = new_token();
		tokens = tokens->next;	
		tokens->str = scanTokenString(fptr, '=');
		tokens->next = new_token();
		tokens = tokens->next;
		tokens->value = scanTokenValue(fptr);
	}
	else if (strcmp(tokens->str, "IMPORT") == 0) {
		tokens->next = new_token();
		tokens = tokens->next;
		tokens->str = scanTokenString(fptr);
	}
	else if (strcmp(tokens->str, "EXPORT") == 0) {
		tokens->next = new_token();
		tokens = tokens->next;
		tokens->str = scanTokenString(fptr);
	}
	else if (strcmp(tokens->str, "EXIT") == 0) {
		exit(0);
	}
	fclose(fptr);
	return head;
}

FILE* initialiseSheet() {
	int i, j;
	FILE* fptr = fopen("temp.csv", "w+");
	for (i = 0; i < CSV_SIZE; i++) {
		for (j = 0; j < CSV_SIZE; j++) {
			fprintf(fptr, "0;");
		}
		fprintf(fptr, "\n");
	}
	return fptr;
}

CELL* getCell(TOKEN* token) {
	CELL* cell = (CELL*)malloc(sizeof(CELL));
	cell->col = token->str[0] - 'A';
	cell->row = token->str[1] - '1';
	if (token->str[2] != '\0') {
		cell->row += 10 - 1;
	}
	return cell;
}

void exportCommand(TOKEN* fp, int** sheet) {
	int i, j;

	FILE* fptr = fopen(getFileName(fp->str), "w");
	for (i = 0; i < CSV_SIZE; i++) {
		for (j = 0; j < CSV_SIZE; j++) {
			fprintf(fptr, "%d,", sheet[i][j]);
		}
		printf("\n");
	}
	fclose(fptr);
}

void importCommand(TOKEN* fp, int** sheet) {
	int i, j;
	FILE* fptr = fopen(getFileName(fp->str), "r+");
	for (i = 0; i < CSV_SIZE; i++) {
		for (j = 0; j < CSV_SIZE; j++) {
			sheet[i][j] = scanTokenValue(fptr);
		}
	}
	fclose(fptr);
}

void printCommand(int** sheet) {
	int i, j, k;
	for (i = 0; i <= CSV_SIZE; i++) {
		printf("x-----");
	}
	printf("x\n|-----");
	for (i = 0; i < CSV_SIZE; i++) {
		printf("|%5c", i + 'A');
	}
	printf("|\n");
	for (i = 0; i < CSV_SIZE; i++) {
		printf("x-----");
		for (j = 0; j < CSV_SIZE; j++) {
			printf("x-----");
		}
		printf("x\n|%5d", i + 1);
		for (j = 0; j < CSV_SIZE; j++) {
			printf("|%5d", sheet[i][j]);
		}
		printf("|\n");
	}
	for (i = 0; i <= CSV_SIZE; i++) {
		printf("x-----");
	}
	printf("x\n");
}

int getCommand(int** sheet, TOKEN* tokens) {
	char ch;
	int count = 0;
	CELL* cell = getCell(tokens);
	return sheet[cell->row][cell->col];
}

void setCommand(int** sheet, TOKEN* tokens) {
	char *str = (char*)malloc(sizeof(char) * 25);
	CELL* cell = getCell(tokens);
	sheet[cell->row][cell->col] = tokens->next->value;
}

void execute(int** sheet, TOKEN* command) {
	if (strcmp(command->str, "GET") == 0) {
		printf("%d\n", getCommand(sheet, command->next));
	}
	else if (strcmp(command->str, "SET") == 0) {
		setCommand(sheet, command->next);
	}
	else if (strcmp(command->str, "PRINT") == 0) {
		printCommand(sheet);
	}
	else if (strcmp(command->str, "IMPORT") == 0) {
		importCommand(command->next, sheet);
	}
	else if (strcmp(command->str, "EXPORT") == 0) {
		exportCommand(command->next, sheet);
	}
}