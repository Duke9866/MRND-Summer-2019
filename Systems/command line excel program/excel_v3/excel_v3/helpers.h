#ifndef _HELPERS_H
#define _HELPERS_H

#define _CRT_SECURE_NO_WARNINGS
#define CSV_SIZE 10

typedef enum { Integer, String, Cell, Expression, Operator } TYPE;
typedef enum { CSV, TXT } FILE_TYPE;

typedef struct cells_list {
	char* cell_name;
	struct cells_list* next;
} CELL_LIST;

typedef struct cell {
	int row;
	int col;
	int value;
	char* exp;
	CELL_LIST* parents;
} CELL;

typedef struct {
	TYPE type;
	union {
		int value;
		char str[30];
		CELL *cell;
	};
} TOKEN;

typedef struct tokens_list {
	TOKEN* token;
	struct tokens_list* next;
} TOKEN_LIST;

typedef struct stack
{
	int top;
	unsigned capacity;
	void** array;
} STACK;

// function to create a stack of given capacity. It initializes size of 
// stack as 0 
STACK* createStack(unsigned capacity, TYPE type)
{
	STACK* stack = (STACK*)malloc(sizeof(STACK));
	stack->capacity = capacity;
	stack->top = -1;
	if (type == Operator) {
		stack->array = (int*malloc(stack->capacity * sizeof(int));
	}
	else {
		stack->array
	}
	return stack;
}

// Stack is full when top is equal to the last index 
int isFull(struct Stack* stack)
{
	return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1 
int isEmpty(struct Stack* stack)
{
	return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1 
void push(struct Stack* stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
	printf("%d pushed to stack\n", item);
}

// Function to remove an item from stack.  It decreases top by 1 
int pop(struct Stack* stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

char* getFileName(char* fname, FILE_TYPE ft);
char* scanTokenString(FILE*, char=' ');
TOKEN_LIST* scanTokenValue(char* str, CELL*** sheet);
CELL* getCell(char* str);
int getCommand(int**, TOKEN*);
void setCommand(int**, TOKEN*);
TOKEN* new_token();
CELL_LIST* new_cell_list();
void add_token(TOKEN_LIST* token_list, TOKEN* token);
TOKEN_LIST* read_command();
CELL*** initialiseSheet();
void execute(CELL*** sheet, TOKEN_LIST* tokens);
void importCommand(TOKEN* fp, CELL*** sheet);
void exportCommand(TOKEN* fp, CELL*** sheet);
int evaluate(CELL* cell, char* expression, CELL*** sheet);
char* removeSpaces(char* str);
bool isOperator(char op);

#endif