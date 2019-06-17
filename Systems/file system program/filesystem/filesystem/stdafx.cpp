// stdafx.cpp : source file that includes just the standard includes
// filesystem.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

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



void copy_to_fs(FILE_TABLE* fileTable, char** argv) {
	int blockCount = 0, blockNo;
	int no_of_files = fileTable->no_of_files;
	void* buffer;
	FILE* fin = fopen(argv[2], "rb");

	//checking if the file exists
	if (findEntry(fileTable, argv[1])) {
		printf("file already exists.\n");
	}

	//checking for disk space
	if (fileTable->no_of_files >= 32) {
		printf("disk ran out of space.\n");
		return;
	}

	//dest file name
	strcpy(fileTable->file_entry_list[no_of_files].file_name, argv[1]);

	//dest file length
	fseek(fin, 0, SEEK_END);
	fileTable->file_entry_list[no_of_files].file_length = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	//dest file no. of blocks
	fileTable->file_entry_list[no_of_files].no_of_blocks = (fileTable->file_entry_list[no_of_files].file_length / BLOCK_SIZE) + 1;

	//dest file start block
	if (!allocate_blocks(fileTable, &fileTable->file_entry_list[no_of_files])) {
		return;
	}

	fileTable->no_of_files++;
	buffer = malloc(BLOCK_SIZE);
	blockNo = fileTable->file_entry_list[no_of_files].start_block;
	for (blockCount = 0; blockCount < fileTable->file_entry_list[no_of_files].no_of_blocks; blockCount++) {
		fread(buffer, BLOCK_SIZE, 1, fin);
		writeBlock(buffer, blockNo);
		blockNo++;
	}
	writeMetaData(fileTable);
	free(buffer);
	fclose(fin);
}



void copy_from_fs(FILE_TABLE* fileTable, char** argv) {
	int blockCount = 0;
	int no_of_files = fileTable->no_of_files - 1;
	int* block_numbers;
	void* buffer;
	FILE* fout = fopen(argv[2], "wb");

	if (!findEntry(fileTable, argv[1])) {
		printf("file doesn't exist.\n");
	}

	buffer = malloc(BLOCK_SIZE);
	if (fileTable->file_entry_list[no_of_files].no_of_blocks == 1) {
		readBlock(buffer, fileTable->file_entry_list[no_of_files].start_block);
		fwrite(buffer, fileTable->file_entry_list[no_of_files].file_length % BLOCK_SIZE, 1, fout);
	}
	else {
		block_numbers = (int*)malloc(sizeof(int) * (fileTable->file_entry_list[no_of_files].no_of_blocks - 1));
		readBlock(buffer, fileTable->file_entry_list[no_of_files].start_block);
		memcpy(block_numbers, buffer, sizeof(int) * (fileTable->file_entry_list[no_of_files].no_of_blocks - 1));
		for (blockCount = 0; blockCount < fileTable->file_entry_list[no_of_files].no_of_blocks - 2; blockCount++) {
			readBlock(buffer, block_numbers[blockCount]);
			fwrite(buffer, BLOCK_SIZE, 1, fout);
		}
		readBlock(buffer, block_numbers[blockCount]);
		fwrite(buffer, fileTable->file_entry_list[no_of_files].file_length % BLOCK_SIZE, 1, fout);
	}
	free(buffer);
	fclose(fout);
}

void debug(FILE_TABLE* fileTable) {
	int i;
	int block_size = BLOCK_SIZE;
	int no_of_blocks = fileTable->no_of_blocks;
	int no_of_free_blocks = fileTable->no_of_free_blocks;
	int no_of_files = fileTable->no_of_files;

	printf("BLOCK SIZE\n");
	printf("%d\n", fileTable->block_size);

	printf("NUMBER OF BLOCKS\n");
	printf("%d\n", fileTable->no_of_blocks);

	printf("NUMBER OF FREE BLOCKS\n");
	printf("%d\n", fileTable->no_of_free_blocks);

	printf("NUMBER OF FILES\n");
	printf("%d\n", fileTable->no_of_files);

	printf("FILES\n");
	for (i = 0; i < fileTable->no_of_files; i++) {
		printf("filename: %10s file_length: %10d start_block: %10d no_of_blocks: %10d\n", fileTable->file_entry_list[i].file_name, fileTable->file_entry_list[i].file_length, fileTable->file_entry_list[i].start_block, fileTable->file_entry_list[i].no_of_blocks);
	}
}

void format(FILE_TABLE *fileTable) {
	fileTable->magic_number = MAGIC_NO;
	fileTable->block_size = BLOCK_SIZE;
	fileTable->no_of_blocks = TOTAL_BLOCKS;
	fileTable->no_of_free_blocks = FREE_BLOCKS;
	memset(fileTable->bitVector, 0, FREE_BLOCKS);
	fileTable->no_of_files = 0;
	memset(fileTable->file_entry_list, 0, sizeof(FILE_RECORD) * 32);
	writeMetaData(fileTable);
}

void delete_file(FILE_TABLE* fileTable, char** argv) {
	int i;
	for (i = 0; i < fileTable->no_of_files; i++) {
		if (strcmp(fileTable->file_entry_list[i].file_name, argv[1]) == 0) {
			if (i + 1 == fileTable->no_of_files) {
				fileTable->no_of_files--;
				deallocate_blocks(fileTable, &fileTable->file_entry_list[i]);
			}
			else {
				memcpy(&fileTable->file_entry_list[i], &fileTable->file_entry_list[fileTable->no_of_files], sizeof(FILE_RECORD));
				fileTable->no_of_files--;
				deallocate_blocks(fileTable, &fileTable->file_entry_list[i]);
			}
		}
	}
}

void list_files(FILE_TABLE* fileTable) {
	int i;
	for (i = 0; i < fileTable->no_of_files; i++) {
		printf("%s\n", fileTable->file_entry_list[i].file_name);
	}
}

bool findEntry(FILE_TABLE* fileTable, char* file_name) {
	int i;
	for (i = 0; i < fileTable->no_of_files; i++) {
		if (strcmp(fileTable->file_entry_list[i].file_name, file_name) == 0) {
			return true;
		}
	}
	return false;
}

void deallocate_blocks(FILE_TABLE* fileTable, FILE_RECORD* record) {
	int i;
	void* buffer;
	int* block_numbers;
	if (record->no_of_blocks > 1) {
		buffer = malloc(BLOCK_SIZE);
		block_numbers = (int*)malloc(record->no_of_blocks);
		readBlock(buffer, record->start_block);
		memcpy(block_numbers, buffer, record->no_of_blocks);
		for (i = 0; i < record->no_of_blocks - 1; i++) {
			fileTable->bitVector[block_numbers[i]] = 0;
		}
	}
	fileTable->bitVector[record->start_block] = 0;
	fileTable->no_of_free_blocks += record->no_of_blocks;
}

bool allocate_blocks(FILE_TABLE* fileTable, FILE_RECORD* record) {
	int i, j;
	unsigned int* block_numbers;
	void* buffer;
	if (fileTable->no_of_free_blocks < record->no_of_blocks) {
		printf("disk ran out of space.\n");
		return false;
	}
	block_numbers = (unsigned int*)malloc(sizeof(unsigned int) * record->no_of_blocks);
	buffer = malloc(BLOCK_SIZE);
	for (i = 0; i < FREE_BLOCKS; i++) {
		if (fileTable->bitVector[i] == 0) {
			fileTable->bitVector[i] = 1;
			record->start_block = i + META_BLOCKS;
			break;
		}
	}
	if (record->no_of_blocks > 1) {
		//first block for indirection
		record->no_of_blocks++;
		for (j = 0; i < FREE_BLOCKS && j < record->no_of_blocks; i++) {
			if (fileTable->bitVector[i] == 0) {
				fileTable->bitVector[i] = 1;
				block_numbers[j] = i + META_BLOCKS;
				j++;
			}
		}
	}
	fileTable->no_of_free_blocks -= record->no_of_blocks;
	memcpy(buffer, block_numbers, BLOCK_SIZE);
	writeBlock(buffer, record->start_block);
	//free(block_numbers);
	free(buffer);
	return true;
}