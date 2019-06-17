#include "stack.h"

void push(STACK* stk, unsigned int element){
	if (stk->top == 100){
		return;
	}
	else{
		stk->arr[stk->top] = element;
		(stk->top)++;
	}
}

int pop(STACK *stk){
	if (stk->top == -1){
		return -1;
	}
	else{
		(stk->top)--;
		return stk->arr[(stk->top) + 1];
	}
}

int stackssize(STACK *stk){
	return stk->top;
}
