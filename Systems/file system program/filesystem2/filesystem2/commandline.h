#ifndef _COMMANDLINE_H
#define _COMMANDLINE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copy_to_fs(char* src_file_name, char* dest_file_name);
void copy_from_fs(char* src_file_name, char* dest_file_name);
void list_files();
void delete_file(char* file_name);
void debug();

#endif