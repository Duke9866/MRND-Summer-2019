#ifndef _FILESYSTEMIO_H
#define _FILESYSTEMIO_H

#include <stdio.h>
#include <string.h>
#include "stdafx.h"

#define BLOCK_SIZE (16384)
#define TOTAL_BLOCKS (6400)
#define META_BLOCKS (4)
#define FREE_BLOCKS (6396)
#define _100MB (104857600)
#define MAGIC_NO (0x444E524D)

typedef struct _file_record {
	char file_name[20];
	int start_block;
	int no_of_blocks;
	long file_length;
} FILE_RECORD;

typedef struct _file_table {
	int magic_number;
	int block_size;
	int no_of_blocks;
	int no_of_free_blocks;
	char bitVector[FREE_BLOCKS];
	int no_of_files;
	FILE_RECORD file_entry_list[32];
} FILE_TABLE;

long getFileOffset(int block_number);
void readBlock(void* buffer, int block_number);
void writeBlock(void* buffer, int block_number);
void readMetaData(FILE_TABLE *fileTable);
void writeMetaData(FILE_TABLE *fileTable);
#endif