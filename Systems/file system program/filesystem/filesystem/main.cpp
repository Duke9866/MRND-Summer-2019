// filesystem.cpp : Defines the entry point for the console application.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "stdafx.h"

bool executeInstruction(FILE_TABLE *fileTable, char* instruction) {
	char** argv = scanArguments(instruction);
	if (strcmp(argv[0], "copyfromdisk") == 0) {
		copy_to_fs(fileTable, argv);
	}
	else if (strcmp(argv[0], "copytodisk") == 0) {
		copy_from_fs(fileTable, argv);
	}
	else if (strcmp(argv[0], "list") == 0) {
		list_files(fileTable);
	}
	else if (strcmp(argv[0], "debug") == 0) {
		debug(fileTable);
	}
	else if (strcmp(argv[0], "format") == 0 || strcmp(argv[0], "init") == 0) {
		format(fileTable);
	}
	else if (strcmp(argv[0], "delete") == 0) {
		delete_file(fileTable, argv);
	}
	else if (strcmp(argv[0], "exit") == 0) {
		return false;
	}
	else {
		readMetaData(fileTable);
	}
	return true;
}

int main() {
	bool running = true;
	char choice;
	char* instruction = (char*)malloc(sizeof(char) * 100);
	FILE_TABLE fileTable;
	readMetaData(&fileTable);
	if (fileTable.magic_number != MAGIC_NO) {
		printf("the file is either corrupted or unknown. do want to format it? (y/n) - ");
		scanf("%c", &choice);
		fflush(stdin);
		if (choice == 'y') {
			executeInstruction(&fileTable, "format");
		}
		else {
			exit(0);
		}
	}
	while (running) {
		printf("\n>> ");
		gets(instruction);
		fflush(stdin);
		running = executeInstruction(&fileTable, instruction);
	}
	return 0;
}