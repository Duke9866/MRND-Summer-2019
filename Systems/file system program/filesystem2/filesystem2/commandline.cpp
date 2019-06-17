#include "commandline.h"

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

int getFreeBlock(FILE_TABLE* fileTable, FILE_RECORD* entry) {
	int i, count = 0;
	for (i = 0; i < FREE_BLOCKS && count < entry->no_of_blocks; i++) {
		if (fileTable->bitVector[i] == 0) {
			count++;
		}
		else {
			count = 0;
		}
	}
	if (count == entry->no_of_blocks) {
		while (count) {
			i--;
			count--;
			fileTable->bitVector[i] = true;
		}
		return i + 4;
	}
	return -1;
}

void assignFreeBlock(FILE_TABLE* fileTable, FILE_RECORD* entry) {
	int i = entry->start_block, count = 0;
	for (count = 0; count < entry->no_of_blocks; count++, i++) {
		fileTable->bitVector[i] = false;
	}
}

void add_entry(FILE_TABLE* fileTable, FILE_RECORD* entry) {
	if (fileTable->numberOfFiles < 32) {
		fileTable->file_entry_list[fileTable->numberOfFiles] = entry;
		fileTable->numberOfFiles++;
	}
	else {
		printf("disk ran out of space.\n");
	}
}

void remove_entry(FILE_TABLE* fileTable, FILE_RECORD* entry) {
	int i;
	for (i = 0; i < fileTable->numberOfFiles; i++) {
		if (strcmp(fileTable->file_entry_list[i]->file_name, entry->file_name) == 0) {
			assignFreeBlock(fileTable, entry);
			free(fileTable->file_entry_list[i]);
			if (i + 1 == fileTable->numberOfFiles) {
				fileTable->numberOfFiles--;
				return;
			}
			else {
				fileTable->file_entry_list[i] = fileTable->file_entry_list[fileTable->numberOfFiles - 1];
				fileTable->file_entry_list[fileTable->numberOfFiles - 1] = NULL;
				fileTable->numberOfFiles--;
			}
		}
	}
}

void copy_to_fs(FILE_TABLE* fileTable, char** argv) {
	int blockCount = 0, blockNo;
	FILE* fin = fopen(argv[1], "rb");
	FILE_RECORD* entry = new_file_entry();
	strcpy(entry->file_name, argv[2]);
	void* buffer = malloc(BLOCK_SIZE);
	if (find_entry(fileTable, entry) != NULL) {
		printf("file already exists.\n");
		free(entry);
		return;
	}
	//entry file name
	strcpy(entry->file_name, argv[1]);

	//entry file length
	fseek(fin, 0, SEEK_END);
	entry->file_length = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	//entry file no of blocks
	entry->no_of_blocks = (entry->file_length / BLOCK_SIZE) + 1;

	//entry start block
	if ((entry->start_block = getFreeBlock(fileTable, entry)) == -1) {
		printf("disk ran out of space.\n");
		free(entry);
	}
	add_entry(fileTable, entry);

	blockNo = entry->start_block;
	while (blockCount < entry->no_of_blocks - 1) {
		fread(buffer, BLOCK_SIZE, 1, fin);
		//memcpy(buffer, fin, BLOCK_SIZE);
		writeBlock(buffer, blockNo);
		blockCount++;
		blockNo++;
	}
	fread(buffer, BLOCK_SIZE, 1, fin);
	writeBlock(buffer, blockNo);

	fclose(fin);
}

void copyToDiskCommand(FILE_TABLE* fileTable, char** argv) {
	int blockCount = 0, blockNo;
	FILE* fout = fopen(argv[2], "wb");
	FILE_RECORD* entry = new_file_entry();
	void* buffer = malloc(BLOCK_SIZE);
	void* tempBuffer;
	strcpy(entry->file_name, argv[1]);
	if ((entry = find_entry(fileTable, entry)) == NULL) {
		printf("file does not exist.\n");
		return;
	}

	blockNo = entry->start_block;
	while (blockCount < entry->no_of_blocks - 1) {
		readBlock(buffer, blockNo);
		memcpy(fout, buffer, BLOCK_SIZE);
		blockCount++;
		blockNo++;
	}
	readBlock(buffer, blockNo);
	tempBuffer = malloc(entry->file_length % BLOCK_SIZE);
	memcpy(tempBuffer, buffer, entry->file_length % BLOCK_SIZE);
	fwrite(tempBuffer, entry->file_length % BLOCK_SIZE, 1, fout);

	//remove_entry(fileTable, entry);
	fclose(fout);
}

void debugCommand(FILE_TABLE* fileTable) {
	int i, freeBlocks = 0;
	printf("NUMBER OF FILES\n");
	printf("%d\n\n", fileTable->numberOfFiles);
	printf("FILE TABLE\n");
	for (i = 0; i < fileTable->numberOfFiles; i++) {
		printf("%20s %4d %4d %8ld\n", fileTable->file_entry_list[i]->file_name, fileTable->file_entry_list[i]->start_block, fileTable->file_entry_list[i]->no_of_blocks, fileTable->file_entry_list[i]->file_length);
	}
	printf("\n");
	printf("FREE BLOCKS\n");
	for (i = 0; i < FREE_BLOCKS; i++) {
		freeBlocks += fileTable->bitVector[i];
	}
	printf("%d\n", FREE_BLOCKS - freeBlocks);
}