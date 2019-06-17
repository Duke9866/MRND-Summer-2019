// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs_io.h"

// TODO: reference additional headers your program requires here

//commands
void copy_to_fs(FILE_TABLE* fileTable, char** argv);
void copy_from_fs(FILE_TABLE* fileTable, char** argv);
void debug(FILE_TABLE* fileTable);
void format(FILE_TABLE *fileTable);
void delete_file(FILE_TABLE* fileTable, char** argv);
void list_files(FILE_TABLE* fileTable);

//helpers
char** scanArguments(char* instruction);
bool findEntry(FILE_TABLE* fileTable, char* file_name);
void deallocate_blocks(FILE_TABLE* fileTable, FILE_RECORD* record);
bool allocate_blocks(FILE_TABLE* fileTable, FILE_RECORD* record);