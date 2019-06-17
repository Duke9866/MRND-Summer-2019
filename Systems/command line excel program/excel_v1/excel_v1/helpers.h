#ifndef _HELPERS_H
#define _HELPERS_H

#define _CRT_SECURE_NO_WARNINGS
#define CSV_SIZE 10

typedef struct cell {
	int row;
	int col;
} CELL;

typedef struct token {
	char* str;
	int value;
	struct token* next;
} TOKEN;

char* getFileName(char* fname);
char* scanTokenString(FILE*, char=' ');
CELL* getCell(TOKEN*);
int getCommand(int**, TOKEN*);
void setCommand(int**, TOKEN*);
TOKEN* new_token();
TOKEN* read_command();
FILE* initialiseSheet();
void execute(int**, TOKEN*);
void importCommand(FILE* fp, int** sheet);
void exportCommand(FILE* fp, int** sheet);

#endif