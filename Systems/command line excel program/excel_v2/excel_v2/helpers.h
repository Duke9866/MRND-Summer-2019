#ifndef _HELPERS_H
#define _HELPERS_H

#define _CRT_SECURE_NO_WARNINGS
#define CSV_SIZE 10

typedef enum { Integer, String, Cell, Expression, Operator } TYPE;
typedef enum { CSV, TXT } FILE_TYPE;

typedef struct cells_list {
	char* cell_name;
	struct cells_list* next;
} CELL_LIST;

typedef struct cell {
	int row;
	int col;
	int value;
	char* exp;
	CELL_LIST* parents;
} CELL;

typedef struct {
	TYPE type;
	union {
		int value;
		char str[30];
		CELL *cell;
	};
} TOKEN;

typedef struct tokens_list {
	TOKEN* token;
	struct tokens_list* next;
} TOKEN_LIST;

char* getFileName(char* fname, FILE_TYPE ft);
char* scanTokenString(FILE*, char=' ');
TOKEN_LIST* scanTokenValue(char* str, CELL*** sheet);
CELL* getCell(char* str);
int getCommand(int**, TOKEN*);
void setCommand(int**, TOKEN*);
TOKEN* new_token();
CELL_LIST* new_cell_list();
void add_token(TOKEN_LIST* token_list, TOKEN* token);
TOKEN_LIST* read_command();
CELL*** initialiseSheet();
void execute(CELL*** sheet, TOKEN_LIST* tokens);
void importCommand(TOKEN* fp, CELL*** sheet);
void exportCommand(TOKEN* fp, CELL*** sheet);
int evaluate(CELL* cell, char* expression, CELL*** sheet);
char* removeSpaces(char* str);
bool isOperator(char op);

#endif