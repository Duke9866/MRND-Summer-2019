#ifndef _STACK_H
#define _STACK_H
#include<stdio.h>
#include<stdlib.h>

typedef struct _stack {
	unsigned int arr[100];
	int top;
} STACK;

STACK* intialize();
void push(STACK  *arr, unsigned int element);
int pop(STACK *arr);
int stackssize(STACK *arr);
STACK * intialize();
#endif