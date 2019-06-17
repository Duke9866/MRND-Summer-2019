#include "diskio.h"
#define DISK_FILE_SIZE (83886080) // 100 mb
#define META_DATA_SIZE (1048576) // 1 mb

long getFileOffset(int block_number) {
	return block_number * BLOCK_SIZE;
}

void readBlock(void* buffer, int block_id) {
	FILE* hardDisk;
	if ((hardDisk = fopen(FILE_NAME, "rb+")) == NULL) {
		exit(0);
	}
	long offset = getFileOffset(block_id);
	fseek(hardDisk, offset, SEEK_SET);
	fread(buffer, BLOCK_SIZE, 1, hardDisk);
	if (fclose(hardDisk) == EOF) {
		exit(0);
	}
}

void writeBlock(void* buffer, int block_id) {
	FILE* hardDisk;
	if ((hardDisk = fopen(FILE_NAME, "rb+")) == NULL) {
		exit(0);
	}
	long offset = getFileOffset(block_id);
	fseek(hardDisk, offset, SEEK_SET);
	fwrite(buffer, BLOCK_SIZE, 1, hardDisk);
	if (fclose(hardDisk) == EOF) {
		exit(0);
	}
}

void format(int block_size) {
	char temp = '\0';
	int num_of_blocks = 0, num_of_free_blocks = 0;
	int magic_number = 0x444E524D, scan_number;
	char choice;
	char* buffer;
	FILE_RECORD* record = (FILE_RECORD*)calloc(1, sizeof(FILE_RECORD));
	if ((buffer = (char*)malloc(block_size)) == NULL) {
		exit(0);
	}
	BLOCK_SIZE = block_size;
	num_of_free_blocks = num_of_blocks = (DISK_FILE_SIZE - META_DATA_SIZE) / BLOCK_SIZE;
	readBlock(buffer, 0);
	memcpy(&scan_number, buffer, sizeof(int));
	if (scan_number != magic_number) {
		printf("the filesystem is not valid. do you want to format it? (y/n) - ");
		scanf("%c", &choice);
		if (choice != 'y') {
			free(record);
			free(buffer);
			return;
		}
	}
	memcpy(buffer, &temp, block_size);
	memcpy(buffer, &magic_number, sizeof(int));
	memcpy(buffer + 4, &block_size, sizeof(int));
	memcpy(buffer + 4 + 4, &num_of_blocks, sizeof(int));
	memcpy(buffer + 4 + 4 + 4, &num_of_free_blocks, sizeof(int));
	writeBlock(buffer, 0);
	free(record);
	free(buffer);
}

unsigned int mount(char* filename, int block_size) {
	int num_of_blocks = 0, num_of_free_blocks = 0;
	int magic_number = 0x444E524D, scan_number;
	char choice;
	char* buffer;
	FILE_RECORD* record[32];
	BLOCK_SIZE = block_size;
	buffer = (char*)malloc(block_size);
	readBlock(buffer, 0);
	memcpy(&scan_number, buffer, sizeof(int));
	if (scan_number != magic_number) {
		printf("the filesystem is not valid. do you want to format it? (y/n) - ");
		scanf("%c", &choice);
		if (choice == 'y') {
			format(block_size);
			free(buffer);
			return num_of_blocks;
		}
	}

	readBlock(buffer, 0);
	writeBlock(buffer, 0);
	free(buffer);
	return num_of_blocks;
}

void unmount() {

}