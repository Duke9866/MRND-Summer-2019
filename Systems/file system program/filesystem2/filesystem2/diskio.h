#ifndef _DISKIO_H
#define _DISKIO_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

static char FILE_NAME[20];
static long BLOCK_SIZE;

long getFileOffset(int block_number);
void format(int block_size);
unsigned int mount(char* file_name, int block_number);
void unmount();
void readBlock(void* buffer, int block_id);
void writeBlock(void* buffer, int block_id);

#endif