#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode_generator.h"
#include "stack.h"

#define META_SIZE 100

typedef struct _symbol {
	char name[5];
	int address;
	int size;
} SYMBOL;

typedef struct _label {
	char block_name[5];
	unsigned int address;
} LABEL;

typedef struct _instruction {
	int inst_no;
	int op_code;
	int arg1, arg2, arg3;
	int jump;
} INSTRUCTION;

typedef struct _meta_data {
	int symbol_index, memory_index, label_index;
	SYMBOL symbolTable[META_SIZE];
	int memory[META_SIZE];
	LABEL labelTable[META_SIZE];
} META_DATA;

typedef struct _opcode_table {
	int op_index;
	INSTRUCTION opcode[META_SIZE];
} OPCODE_TABLE;

char** scanInstruction(char* instruction) {
	int argNo = 0, index;
	char** argv = (char**)malloc(sizeof(char*) * 5);
	char* trim_new_line = instruction;
	while (*trim_new_line != '\n') {
		trim_new_line++;
	}
	*trim_new_line = '\0';
	while (*instruction) {
		argv[argNo] = (char*)malloc(sizeof(char) * 20);
		index = 0;
		while (*instruction == ' ' || *instruction == ',') {
			instruction++;
		}
		while (*instruction != ' ' && *instruction != '\0' && *instruction != ',') {
			argv[argNo][index] = *instruction;
			instruction++;
			index++;
		}
		argv[argNo][index] = '\0';
		argNo++;
	}
	return argv;
}

int get_symbol_bytes(char* symbol_name) {
	bool isArray = false;
	int number = 0;
	while (*symbol_name != '\0') {
		if (*symbol_name == '[') {
			isArray = true;
			symbol_name++;
			break;
		}
		symbol_name++;
	}
	if (isArray) {
		while (*symbol_name != ']') {
			number = number * 10 + *symbol_name - '0';
			symbol_name++;
		}
		return number;
	}
	return 1;
}

char* get_symbol_name(char* symbol_name) {
	char* str = (char*)malloc(sizeof(char) * 10), *res;
	strcpy(str, symbol_name);
	res = str;
	while (*str != '[' && *str != ':' && *str != '\n' && *str != '\0') {
		str++;
	}
	*str = '\0';
	return res;
}

int get_symbol_value(char* symbol) {
	int number = 0;
	while (*symbol != '\0' && *symbol != '\n') {
		number = number * 10 + *symbol - '0';
		symbol++;
	}
	return number;
}

int get_reg_value(char* symbol) {
	if (*symbol >= 'A' && *symbol <= 'H') {
		return *symbol - 'A';
	}
}

void buildSymbolTable(char** argv, META_DATA* metaData) {
	int i;
	int symbol_index = metaData->symbol_index;
	int memory_index = metaData->memory_index;
	char *symbol_name = get_symbol_name(argv[1]);
	if (strcmp(argv[0], "DATA") == 0) {
		metaData->symbolTable[symbol_index].size = get_symbol_bytes(argv[1]);
		strcpy(metaData->symbolTable[symbol_index].name, symbol_name);
		metaData->symbolTable[symbol_index].address = metaData->memory_index;
		memory_index += metaData->symbolTable[symbol_index].size;
		symbol_index++;
	}
	else if (strcmp(argv[0], "CONST") == 0) {
		metaData->symbolTable[symbol_index].size = get_symbol_bytes(argv[1]);
		strcpy(metaData->symbolTable[symbol_index].name, symbol_name);
		metaData->symbolTable[symbol_index].address = memory_index;
		for (i = 0; i < metaData->symbolTable[symbol_index].size; i++) {
			metaData->memory[i] = get_symbol_value(argv[3]);
		}
		memory_index += metaData->symbolTable[symbol_index].size;
		symbol_index++;
	}
	metaData->symbol_index = symbol_index;
	metaData->memory_index = memory_index;
}

int find_symbol_address(META_DATA* metaData, char* symbol) {
	int i;
	for (i = 0; i < metaData->symbol_index; i++) {
		if (strcmp(metaData->symbolTable[i].name, symbol) == 0) {
			return metaData->symbolTable[i].address;
		}
	}
}

int find_label_address(META_DATA* metaData, char* symbol) {
	int i;
	for (i = 0; i < metaData->label_index; i++) {
		if (strcmp(metaData->labelTable[i].block_name, symbol) == 0) {
			return metaData->labelTable[i].address;
		}
	}
}

bool isLabel(char* symbol) {
	while (*symbol != ':' && *symbol != '\0') {
		symbol++;
	}
	return *symbol == ':';
}

bool isRegister(char* symbol) {
	if (*symbol >= 'A' && *symbol <= 'H' && *(symbol + 1) == 'X') {
		return true;
	}
	return false;
}

void processArith(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable) {
	int op_index = opcodeTable->op_index;
	if (strcmp(argv[0], "ADD") == 0) {
		opcodeTable->opcode[op_index].op_code = 3; // OPCODE for ADD
	}
	else if (strcmp(argv[0], "SUB") == 0) {
		opcodeTable->opcode[op_index].op_code = 4; // OPCODE for SUB
	}
	else if (strcmp(argv[0], "MUL") == 0) {
		opcodeTable->opcode[op_index].op_code = 5; // OPCODE for MUL
	}
	opcodeTable->opcode[op_index].inst_no = op_index + 1;
	opcodeTable->opcode[op_index].arg1 = get_reg_value(argv[1]);
	opcodeTable->opcode[op_index].arg2 = get_reg_value(argv[2]);
	opcodeTable->opcode[op_index].arg3 = get_reg_value(argv[3]);
	opcodeTable->op_index++;
}

void processLabel(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable) {
	strcpy(metaData->labelTable[metaData->label_index].block_name, get_symbol_name(argv[0]));
	metaData->labelTable[metaData->label_index].address = opcodeTable->op_index;
	metaData->label_index++;
}

void processIf(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable, STACK* stack) {
	opcodeTable->opcode[opcodeTable->op_index].inst_no = opcodeTable->op_index + 1;
	opcodeTable->opcode[opcodeTable->op_index].arg1 = get_reg_value(argv[1]);
	opcodeTable->opcode[opcodeTable->op_index].op_code = 7; // OPCODE for IF
	if (strcmp(argv[2], "EQ") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].arg2 = 8; // OPCODE for EQ
	}
	else if (strcmp(argv[2], "LT") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].arg3 = 9; // OPCODE for LT
	}
	else if (strcmp(argv[2], "GT") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].arg3 = 10; // OPCODE for GT
	}
	else if (strcmp(argv[2], "LTEQ") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].arg3 = 11; // OPCODE for LTEQ
	}
	else if (strcmp(argv[2], "GTEQ") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].arg3 = 12; // OPCODE for GTEQ
	}
	opcodeTable->opcode[opcodeTable->op_index].arg3 = get_reg_value(argv[1]);
	push(stack, opcodeTable->op_index + 1);
	opcodeTable->op_index++;
}

void processIO(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable) {
	if (strcmp(argv[0], "READ") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].op_code = 14; // OPCODE for READ
		opcodeTable->opcode[opcodeTable->op_index].arg1 = get_reg_value(argv[1]);
	}
	else if (strcmp(argv[0], "PRINT") == 0) {
		opcodeTable->opcode[opcodeTable->op_index].op_code = 13; // OPCODE for PRINT
		opcodeTable->opcode[opcodeTable->op_index].arg1 = find_symbol_address(metaData, argv[1]);
	}
	opcodeTable->opcode[opcodeTable->op_index].inst_no = opcodeTable->op_index + 1;
	opcodeTable->op_index++;
}

void processMov(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable) {
	if (isRegister(argv[1])) {
		opcodeTable->opcode[opcodeTable->op_index].op_code = 2; // OPCODE for MOV from memory to register
		opcodeTable->opcode[opcodeTable->op_index].arg1 = get_reg_value(argv[1]);
		opcodeTable->opcode[opcodeTable->op_index].arg2 = find_symbol_address(metaData, argv[2]);
	}
	else {
		opcodeTable->opcode[opcodeTable->op_index].op_code = 1; // OPCODE for MOV from register to memory
		opcodeTable->opcode[opcodeTable->op_index].arg1 = find_symbol_address(metaData, argv[1]);
		opcodeTable->opcode[opcodeTable->op_index].arg2 = get_reg_value(argv[2]);
	}
	opcodeTable->opcode[opcodeTable->op_index].inst_no = opcodeTable->op_index + 1;
	opcodeTable->op_index++;
}

void processJump(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable) {
	opcodeTable->opcode[opcodeTable->op_index].inst_no = opcodeTable->op_index + 1;
	opcodeTable->opcode[opcodeTable->op_index].op_code = 6; // OPCODE for JMP
	opcodeTable->opcode[opcodeTable->op_index].arg1 = find_label_address(metaData, argv[1]);
}

void processEndIf(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable, STACK* stack) {
	int i, address, op_index;
	op_index = opcodeTable->op_index + 1;
	opcodeTable->opcode[opcodeTable->op_index].inst_no = opcodeTable->op_index + 1;
	while ((address = pop(stack)) != -1) {
		opcodeTable->opcode[address].jump = op_index;
		op_index = opcodeTable->opcode[address].inst_no;
	}
}

void debug(OPCODE_TABLE* opcodeTable) {
	int i;
	for (i = 0; i < opcodeTable->op_index; i++) {
		if (opcodeTable->opcode[i].inst_no != -1)
			printf("%5d)", opcodeTable->opcode[i].inst_no);
		if (opcodeTable->opcode[i].op_code != -1)
			printf("%5d", opcodeTable->opcode[i].op_code);
		if (opcodeTable->opcode[i].arg1 != -1)
			printf("%5d", opcodeTable->opcode[i].arg1);
		if (opcodeTable->opcode[i].arg2 != -1)
			printf("%5d", opcodeTable->opcode[i].arg2);
		if (opcodeTable->opcode[i].arg3 != -1)
			printf("%5d", opcodeTable->opcode[i].arg3);
		if (opcodeTable->opcode[i].jump != -1)
			printf("%5d", opcodeTable->opcode[i].jump);
		printf("\n");
	}
}

void execute(char** argv, META_DATA* metaData, OPCODE_TABLE* opcodeTable, STACK* stack) {
	if (strcmp(argv[0], "DATA") == 0 || strcmp(argv[0], "CONST") == 0) {
		buildSymbolTable(argv, metaData);
	}
	else if (strcmp(argv[0], "ADD") == 0 || strcmp(argv[0], "SUB") == 0 || strcmp(argv[0], "MUL") == 0) {
		processArith(argv, metaData, opcodeTable);
	}
	else if (strcmp(argv[0], "IF") == 0) {
		processIf(argv, metaData, opcodeTable, stack);
	}
	else if (strcmp(argv[0], "ENDIF") == 0) {
		processEndIf(argv, metaData, opcodeTable, stack);
	}
	else if (strcmp(argv[0], "READ") == 0 || strcmp(argv[0], "PRINT") == 0) {
		processIO(argv, metaData, opcodeTable);
	}
	else if (strcmp(argv[0], "MOV") == 0) {
		processMov(argv, metaData, opcodeTable);
	}
	else if (strcmp(argv[0], "JMP") == 0) {
		processJump(argv, metaData, opcodeTable);
	}
	else if (isLabel(argv[0])) {
		processLabel(argv, metaData, opcodeTable);
	}
}

int main() {
	int i;
	char* fin_name = "input.txt";
	char* fout_name = "output.txt";
	char* instruction = (char*)malloc(sizeof(char) * 100);
	char** argv;
	
	FILE* fin, *fout;
	
	META_DATA metaTable;
	metaTable.symbol_index = 0;
	metaTable.memory_index = 0;
	metaTable.label_index = 0;
	
	OPCODE_TABLE opcodeTable;
	opcodeTable.op_index = 0;
	memset(opcodeTable.opcode, -1, sizeof(opcodeTable.opcode));

	STACK stack;
	stack.top = -1;

	if ((fin = fopen(fin_name, "r")) == 0) {
		exit(0);
	}
	if ((fout = fopen(fout_name, "w")) == 0) {
		exit(0);
	}
	while (!feof(fin)) {
		fgets(instruction, 100, fin);
		argv = scanInstruction(instruction);
		execute(argv, &metaTable, &opcodeTable, &stack);
	}
	debug(&opcodeTable);
	fclose(fin);
	fclose(fout);
	return 0;
}