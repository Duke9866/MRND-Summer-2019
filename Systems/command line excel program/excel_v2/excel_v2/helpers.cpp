#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

CELL*** initialiseSheet() {
	CELL*** sheet = (CELL***)malloc(sizeof(CELL**) * CSV_SIZE);
	int i, j;
	for (i = 0; i < CSV_SIZE; i++) {
		sheet[i] = (CELL**)malloc(sizeof(CELL*) * CSV_SIZE);
		for (j = 0; j < CSV_SIZE; j++) {
			sheet[i][j] = (CELL*)malloc(sizeof(CELL));
			sheet[i][j]->row = i;
			sheet[i][j]->col = j;
			sheet[i][j]->value = NULL;
			sheet[i][j]->exp = NULL;
			sheet[i][j]->parents = new_cell_list();
		}
	}
	return sheet;
}

char* getFileName(char* fname, FILE_TYPE ft) {
	char* ptr = fname;
	char temp[20] = "";
	while (*ptr != '.' && *ptr != '\0') {
		ptr++;
	}
	*ptr = '\0'; 
	if (ft == CSV) {
		strcpy(temp, "datafiles\\");
		strcat(temp, fname);
		strcpy(fname, temp);
		strcat(fname, ".csv");
	}
	else if (ft == TXT) {
		strcpy(temp, "metafiles\\");
		strcat(temp, fname);
		strcpy(fname, temp);
		strcat(fname, ".txt");
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

char* removeSpaces(char* str) {
	int i, j;
	if (str == NULL) {
		return NULL;
	}
	for (i = j = 0; str[i]; i++) {
		if (str[i] != ' ') {
			str[j] = str[i];
			j++;
		}
	}
	str[j] = '\0';
	return str;
}

bool isOperator(char op) {
	int i = 0;
	char* op_list = "+-*/%";
	for (i = 0; op_list[i]; i++) {
		if (op == op_list[i]) {
			return true;
		}
	}
	return false;
}

CELL* getCell(char* str) {
	CELL* cell = (CELL*)malloc(sizeof(CELL));
	cell->exp = NULL;
	cell->parents = NULL;
	cell->value = NULL;
	cell->col = str[0] - 'A';
	cell->row = str[1] - '1';
	if (str[2] != '\0') {
		cell->row += 10 - 1;
	}
	return cell;
}

char* getCellName(CELL* cell) {
	char* cell_name = (char*)malloc(sizeof(char) * 5);
	cell_name[0] = cell->col + 'A';
	cell_name[1] = (cell->row + 1) < 10 ? cell->row + '1' : '1';
	cell_name[2] = (cell->row + 1) > 10 ? '0' : '\0';
	cell_name[3] = '\0';
	return cell_name;
}

TOKEN* new_token() {
	TOKEN* t = (TOKEN*)malloc(sizeof(TOKEN));
	return t;
}

TOKEN_LIST* new_token_list(TOKEN* token) {
	TOKEN_LIST* token_list = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
	token_list->token = token;
	token_list->next = NULL;
	return token_list;
}

CELL_LIST* new_cell_list() {
	int i;
	CELL_LIST* cell_list = (CELL_LIST*)malloc(sizeof(CELL_LIST));
	cell_list->cell_name = NULL;
	cell_list->next = NULL;
	return cell_list;
}

void add_token(TOKEN_LIST* token_list, TOKEN* token) {
	while (token_list->next != NULL) {
		token_list = token_list->next;
	}
	token_list->next = new_token_list(token);
}

void add_parent(CELL_LIST* parentList, char* parentName) {
	if (parentList->cell_name == NULL) {
		parentList->cell_name = parentName;
		return;
	}
	if (strcmp(parentList->cell_name, parentName) == 0) {
		return;
	}
	while (parentList->next) {
		if (strcmp(parentList->cell_name, parentName) == 0) {
			return;
		}
		parentList = parentList->next;
	}
	parentList->next = new_cell_list();
	parentList->next->cell_name = parentName;
}

TOKEN_LIST* scanTokenValue(char* str, CELL*** sheet) {
	int i = 0, value = 0;
	bool tokenScanned = false;
	TOKEN_LIST* tokens = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
	TOKEN* token;
	str = removeSpaces(str);
	while (str[i] != '\0') {
		if (str[i] >= '0' && str[i] <= '9') {
			while (str[i] >= '0' && str[i] <= '9') {
				value = value * 10 + str[i] - '0';
				i++;
			}
			token = new_token();
			token->value = value;
			token->type = Integer;
			if (tokenScanned) {
				add_token(tokens, token);
			}
			else {
				tokens = new_token_list(token);
				tokenScanned = true;
			}
		}
		else if (isOperator(str[i])) {
			token = new_token();
			token->str[0] = str[i];
			token->str[1] = '\0';
			i++;
			token->type = Operator;
			if (tokenScanned) {
				add_token(tokens, token);
			}
			else {
				tokens = new_token_list(token);
				tokenScanned = true;
			}
		}
		else if (str[i] >= 'A' && str[i] <= 'J') {
			token = new_token();
			token->cell = (CELL*)malloc(sizeof(CELL));
			token->cell->col = str[i++] - 'A';
			token->cell->row = str[i++] - '1';
			if (str[i] >= '0' && str[i] <= '9') {
				token->cell->row = 10 - 1;
				i++;
			}
			token->cell = sheet[token->cell->row][token->cell->col];
			token->type = Cell;
			if (tokenScanned) {
				add_token(tokens, token);
			}
			else {
				tokens = new_token_list(token);
				tokenScanned = true;
			}
		}
	}
	return tokenScanned ? tokens : NULL;
}

bool check_loop(CELL* child, CELL* ref, CELL*** sheet) {
	CELL_LIST* parents = child->parents;
	CELL* parent;
	if (child == ref) {
		return true;
	}
	while (parents && parents->cell_name) {
		parent = getCell(parents->cell_name);
		if (check_loop(sheet[parent->row][parent->col], ref, sheet)) {
			return true;
		}
		parents = parents->next;
	}
	return false;
}

int evaluate(CELL* cell, char* expression, CELL*** sheet) {
	if (expression == NULL) {
		return sheet[cell->row][cell->col]->value;
	}
	TOKEN_LIST* tokens = scanTokenValue(expression, sheet);
	char optr = NULL;
	int ind_oprand = 0, oprand[2] = { 0 };
	CELL* tempCell;
	while (tokens) {
		if (tokens->token->type == Cell) {
			tempCell = tokens->token->cell;
			if (tempCell->exp) {
				add_parent(sheet[cell->row][cell->col]->parents, getCellName(tempCell));
				oprand[ind_oprand] = evaluate(sheet[tempCell->row][tempCell->col], sheet[tempCell->row][tempCell->col]->exp, sheet);
				ind_oprand++;
			}
			else {
				return sheet[cell->row][cell->col]->value;
			}
		}
		else if (tokens->token->type == Operator) {
			optr = tokens->token->str[0];
		}
		else if (tokens->token->type == Integer) {
			oprand[ind_oprand] = tokens->token->value;
			ind_oprand++;
		}
		tokens = tokens->next;
	}
	switch (optr) {
	case '+':
		return oprand[0] + oprand[1];
	case '-':
		return oprand[0] - oprand[1];
	case '*':
		return oprand[0] * oprand[1];
	case '/':
		return oprand[0] / oprand[1];
	case '%':
		return oprand[0] % oprand[1];
	default:
		return oprand[0];
	}
	return 0;
}

TOKEN_LIST* read_command() {
	TOKEN_LIST* tokens;
	TOKEN* token = new_token();
	printf("\n>");
	fflush(stdin);
	FILE *fptr = fopen("inp_buffer.txt", "w+");
	char ch;
	while ((ch = getchar()) != '\n') {
		putc(ch, fptr);
	}
	fseek(fptr, 0, SEEK_SET);

	//getting the command string
	strcpy(token->str, scanTokenString(fptr));
	token->type = String;

	//initialising the token list
	tokens = new_token_list(token);

	//checking for commands
	if (strcmp(token->str, "GET") == 0) {
		//getting the cell
		token = new_token();
		token->cell = getCell(scanTokenString(fptr));
		token->type = Cell;

		//adding to token list
		add_token(tokens, token);
	}
	else if (strcmp(token->str, "SET") == 0) {
		//getting the cell
		token = new_token();
		token->cell = getCell(scanTokenString(fptr, '='));
		token->type = Cell;

		//adding to token list
		add_token(tokens, token);
		//getting the value
		token = new_token();

		//getting rid of '=' operator
		fseek(fptr, -1, SEEK_CUR);
		while ((ch = fgetc(fptr)) != '=');
		
		//scanning the rest of the input as expression
		fgets(token->str, 30, fptr);
		token->type = Expression;

		//adding to token list
		add_token(tokens, token);
	}
	else if (strcmp(tokens->token->str, "IMPORT") == 0) {
		//getting the filename
		token = new_token();
		strcpy(token->str, scanTokenString(fptr));
		token->type = String;

		//adding to token list
		add_token(tokens, token);
	}
	else if (strcmp(tokens->token->str, "EXPORT") == 0) {
		//getting the filename
		token = new_token();
		strcpy(token->str, scanTokenString(fptr));
		token->type = String;

		//adding to token list
		add_token(tokens, token);
	}
	else if (strcmp(tokens->token->str, "SAVE") == 0) {
		FILE* fptr = fopen("metafiles\\_filenames.txt", "r");
		token = new_token();
		char fname[30];
		if (fptr != NULL) {
			strcpy(fname, scanTokenString(fptr, '\\'));
			strcpy(fname, scanTokenString(fptr));
			add_token(tokens, token);
		}
		else {
			printf("Invalid command\n");
		}
	}
	fclose(fptr);
	return tokens;
}

void exportCommand(TOKEN_LIST* tokens, CELL*** sheet) {
	int i, j;
	tokens = tokens->next;
	char filename[30] = "";
	strcpy(filename, tokens->token->str);
	FILE* csvFile = fopen(getFileName(filename, CSV), "w");
	strcpy(filename, tokens->token->str);
	FILE* txtFile = fopen(getFileName(filename, TXT), "w");
	for (i = 0; i < CSV_SIZE; i++) {
		for (j = 0; j < CSV_SIZE; j++) {
			fprintf(csvFile, "%d,", evaluate(sheet[i][j], sheet[i][j]->exp, sheet));
			fprintf(txtFile, "%s\n", sheet[i][j]->exp);
		}
		fprintf(csvFile, "\n");
	}
	fclose(csvFile);
	fclose(txtFile);
}

void importCommand(TOKEN_LIST* tokens, CELL*** sheet) {
	int i, j;
	tokens = tokens->next;
	FILE* initFileName = fopen("metafiles\\_filenames.txt", "w");
	char filename[30] = "";
	int value;
	char* exp = (char*)malloc(sizeof(char) * 50);
	strcpy(filename, tokens->token->str);
	FILE* csvFile = fopen(getFileName(filename, CSV), "r");
	fprintf(initFileName, "%s\n", filename);
	strcpy(filename, tokens->token->str);
	FILE* txtFile = fopen(getFileName(filename, TXT), "r");
	fprintf(initFileName, "%s\n", filename);
	for (i = 0; i < CSV_SIZE; i++) {
		for (j = 0; j < CSV_SIZE; j++) {
			fscanf(csvFile, "%d,", &value);
			fscanf(txtFile, "%s\n", exp);
			sheet[i][j]->value = value;
			sheet[i][j]->exp = exp;
		}
	}
	fclose(csvFile);
	fclose(txtFile);
	fclose(initFileName);
}

void printCommand(CELL*** sheet) {
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
			printf("|%5d", sheet[i][j]->value);
		}
		printf("|\n");
	}
	for (i = 0; i <= CSV_SIZE; i++) {
		printf("x-----");
	}
	printf("x\n");
}

int getCommand(CELL*** sheet, TOKEN_LIST* tokens) {
	char ch;
	int count = 0;
	tokens = tokens->next;
	CELL* cell = tokens->token->cell;
	return evaluate(sheet[cell->row][cell->col], sheet[cell->row][cell->col]->exp, sheet);
}

void setCommand(CELL*** sheet, TOKEN_LIST* tokens) {
	bool notSettable = false;
	char* exp;
	tokens = tokens->next;
	CELL* cell = tokens->token->cell;
	tokens = tokens->next;
	exp = removeSpaces(tokens->token->str);
	TOKEN_LIST* expTokens = scanTokenValue(exp, sheet);
	//checking for loops
	while (expTokens && !notSettable) {
		if (expTokens->token->type == Cell) {
			notSettable = check_loop(sheet[expTokens->token->cell->row][expTokens->token->cell->col], sheet[cell->row][cell->col], sheet);
		}
		expTokens = expTokens->next;
	}
	//if the loop is not found then set the expression
	if (!notSettable) {
		sheet[cell->row][cell->col]->exp = exp;
		sheet[cell->row][cell->col]->parents = new_cell_list();
		sheet[cell->row][cell->col]->value = evaluate(sheet[cell->row][cell->col], sheet[cell->row][cell->col]->exp, sheet);
	}
	else {
		printf("Invalid command\n");
	}
}

void execute(CELL*** sheet, TOKEN_LIST* tokens) {
	if (strcmp(tokens->token->str, "GET") == 0) {
		printf("%d\n", getCommand(sheet, tokens));
	}
	else if (strcmp(tokens->token->str, "SET") == 0) {
		setCommand(sheet, tokens);
	}
	else if (strcmp(tokens->token->str, "PRINT") == 0) {
		printCommand(sheet);
	}
	else if (strcmp(tokens->token->str, "IMPORT") == 0) {
		importCommand(tokens, sheet);
	}
	else if (strcmp(tokens->token->str, "EXPORT") == 0) {
		exportCommand(tokens, sheet);
	}
	else if (strcmp(tokens->token->str, "SAVE") == 0) {
		exportCommand(tokens, sheet);
	}
	else if (strcmp(tokens->token->str, "EXIT") == 0) {
		fputc(EOF, fopen("_fnames.txt", "w"));
		remove("metafiles\\_filenames.txt");
		exit(1);
	}
}