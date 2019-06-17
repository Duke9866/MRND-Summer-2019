// excel.c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "helpers.h"

//typedef struct token {
//	char* str;
//	struct token* next;
//} TOKEN;

int main(int argc, char* argv[]) {
	TOKEN_LIST* command;
	CELL*** sheet = initialiseSheet();
	
	while ((command = read_command()) != NULL) {
		execute(sheet, command);
	}
	return 0;
}

