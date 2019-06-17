#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MILLION 1000000

char* pre_append_zeroes(char* number) {
	int i;
	int len = strlen(number);
	if (*number == '\0') {
		return NULL;
	}
	char* str = (char*)malloc(sizeof(char*) * 8);
	strcpy(str, "0000000");
	while (len < 7 && *number != '\0') {
		str[7 - len] = *number;
		len++;
		number++;
	}
	return str;
}

void initTempFile(char* fileName) {
	FILE* input = fopen(fileName, "r");
	FILE* output = fopen("tempInput.txt", "w");
	char* number = (char*)malloc(sizeof(char) * 10);
	while (!feof(input)) {
		fscanf(input, "%s", number);
		number = pre_append_zeroes(number);
		fprintf(output, "%s\n", number);
	}
	fclose(input);
	fclose(output);
}

void closeTempFiles(FILE** tempFiles) {
	int i;
	for (i = 0; i < 10; i++) {
		fclose(tempFiles[i]);
	}
}

/*
method - bit vector
time complexity - O(10 * (n + 1000000)) ~ O(n)
space complexity - O(>1000000) ~ O(1000000)
*/
void sort(char* inputFileName) {
	int i;
	long j, number;
	bool *bit_vector = (bool*)calloc(MILLION, sizeof(bool));
	FILE* inputFile = fopen(inputFileName, "r");
	for (i = 0; i < 10; i++) {
		fseek(inputFile, 0, SEEK_SET);
		while (!feof(inputFile)) {
			fscanf(inputFile, "%ld", &number);
			if (number / MILLION == i) {
				bit_vector[number % MILLION] = 1;
			}
		}
		for (j = 0; j < MILLION; j++) {
			if (bit_vector[j]) {
				printf("%ld\n", j + i * MILLION);
				bit_vector[j] = 0;
			}
		}
	}
	fclose(inputFile);
}

void init() {
	sort("input.txt");
}