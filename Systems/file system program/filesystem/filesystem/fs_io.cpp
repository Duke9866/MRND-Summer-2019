#include "stdafx.h"
#include "fs_io.h"

long getFileOffset(int block_number) {
	return block_number * BLOCK_SIZE;
}

void readBlock(void* buffer, int block_number) {
	FILE* hardDisk;
	if ((hardDisk = fopen("hardDisk.hdd", "rb+")) == NULL) {
		exit(0);
	}
	long offset = getFileOffset(block_number);
	fseek(hardDisk, offset, SEEK_SET);
	fread(buffer, BLOCK_SIZE, 1, hardDisk);
	if (fclose(hardDisk) == EOF) {
		exit(0);
	}
}

void writeBlock(void* buffer, int block_number) {
	FILE* hardDisk;
	if ((hardDisk = fopen("hardDisk.hdd", "rb+")) == NULL) {
		exit(0);
	}
	long offset = getFileOffset(block_number);
	fseek(hardDisk, offset, SEEK_SET);
	fwrite(buffer, BLOCK_SIZE, 1, hardDisk);
	if (fclose(hardDisk) == EOF) {
		exit(0);
	}	
}

void writeMetaData(FILE_TABLE *fileTable) {
	void* buffer = (void*)malloc(BLOCK_SIZE);
	memset(buffer, 0, BLOCK_SIZE);
	memcpy(buffer, fileTable, sizeof(FILE_TABLE));
	writeBlock(buffer, 0);
	free(buffer);
}

void readMetaData(FILE_TABLE *fileTable) {
	void* buffer = (void*)malloc(BLOCK_SIZE);
	memset(buffer, 0, BLOCK_SIZE);
	readBlock(buffer, 0);
	memcpy(fileTable, buffer, sizeof(FILE_TABLE));
	free(buffer);
}