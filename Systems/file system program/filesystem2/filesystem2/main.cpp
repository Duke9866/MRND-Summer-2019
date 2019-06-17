// filesystem.cpp : Defines the entry point for the console application.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "commandline.h"
#include "diskio.h"

typedef struct _file_record {
	char file_name[20];
	long size;
	int no_of_blocks;
	int start_block;
} FILE_RECORD;

typedef struct _file_table {
	int numberOfFiles;
	char bitVector[32];
	FILE_RECORD* file_entry_list[32];
} FILE_TABLE;

FILE_RECORD* new_file_entry() {
	FILE_RECORD* entry = (FILE_RECORD*)calloc(1, sizeof(FILE_RECORD));
	return entry;
}

FILE_TABLE* new_file_table() {
	FILE_TABLE* fileTable = (FILE_TABLE*)calloc(1, sizeof(FILE_TABLE));
	return fileTable;
}

//void writeMetaData(FILE_TABLE* fileTable) {
//	void* buffer = malloc(BLOCK_SIZE);
//	writeBlock(buffer, 0);
//	writeBlock(buffer, 0);
//	memcpy(buffer, fileTable->file_entry_list, sizeof(FILE_RECORD) * 32);
//	memcpy(buffer, &fileTable->numberOfFiles, sizeof(int));
//	memcpy(buffer, &fileTable->bitVector, sizeof(bool) * FREE_BLOCKS);
//}

//void readMetaData(FILE_TABLE* fileTable) {
//	void* buffer = malloc(BLOCK_SIZE);
//	readBlock(buffer, 0);
//	memcpy(fileTable->file_entry_list, (char*)buffer + 4, sizeof(FILE_RECORD) * 32);
//	memcpy(&fileTable->numberOfFiles, (char*)buffer + 4 + (32 * 32), sizeof(int));
//	memcpy(&fileTable->bitVector, (char*)buffer + 4 + (32 * 32) + 4, sizeof(bool) * FREE_BLOCKS);
//}

char** scanArguments(char* instruction) {
	int argNo = 0, index;
	char** argv = (char**)malloc(sizeof(char*) * 5);
	while (*instruction) {
		argv[argNo] = (char*)malloc(sizeof(char) * 20);
		index = 0;
		while (*instruction == ' ') {
			instruction++;
		}
		while (*instruction != ' ' && *instruction != '\0') {
			argv[argNo][index] = *instruction;
			instruction++;
			index++;
		}
		argv[argNo][index] = '\0';
		argNo++;
	}
	return argv;
}

void executeInstruction(FILE_TABLE* fileTable, char** argv, unsigned int block_size) {
	if (strcmp(argv[0], "COPYTOFS") == 0) {
		copy_to_fs(argv[1], argv[2]);
	}
	else if (strcmp(argv[0], "COPYFROMFS") == 0) {
		copy_from_fs(argv[1], argv[2]);
	}
	else if (strcmp(argv[0], "FORMAT") == 0) {
		format(BLOCK_SIZE);
	}
	else if (strcmp(argv[0], "LS") == 0) {
		list_files();
	}
	else if (strcmp(argv[0], "DELETE") == 0) {
		delete_file(argv[1]);
	}
}

int getIntegerValue(char* string) {
	int number = 0;
	while (*string != '\0') {
		number = number * 10 + *string - '0';
		string++;
	}
	return number;
}

int main() {
	void* buffer;
	char* instruction = (char*)malloc(sizeof(char) * 50);
	char** argv;
	bool fileMounted = false, running = true;
	char choice;
	int signRead;
	int no_of_blocks;
	unsigned int block_size;
	FILE_TABLE* fileTable = new_file_table();

	while (running) {
		printf("\n>> ");
		gets(instruction);
		fflush(stdin);
		argv = scanArguments(instruction);
		if (!fileMounted) {
			if (strcmp(argv[0], "MOUNT") != 0) {
				printf("disk not mounted.\n");
			}
			else {
				block_size = getIntegerValue(argv[2]);
				no_of_blocks = mount(argv[1], block_size);
				if (no_of_blocks > 0) {
					fileMounted = true;
				}
			}
		}
		else if (strcmp(argv[0], "UNMOUNT") == 0) {
			unmount();
			fileMounted = false;
		}
		else if(strcmp(argv[0], "EXIT") == 0) {
			running = false;
		}
		else {
			executeInstruction(fileTable, argv, block_size);
		}
	}
	return 0;
}