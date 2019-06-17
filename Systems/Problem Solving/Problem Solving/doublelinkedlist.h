#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct node *next, *prev;
} node;

typedef struct doublelinkedlist{
	struct node *head;
	struct node *tail;
} dll;

dll* newList() {
	dll* temp = (dll*)malloc(sizeof(dll));
	temp->head = NULL;
	temp->tail = NULL;
	return temp;
}

node* newNode(int data) {
	node* temp = (node*)malloc(sizeof(node));
	temp->data = data;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}

dll* insertBegin(dll *list, int data) {
	node* temp = newNode(data);
	if (list->head == NULL) {
		list->head = temp;
		list->tail = temp;
	}
	else {
		temp->next = list->head;
		list->head->prev = temp;
		list->head = temp;
	}
	return list;
}

dll* insertEnd(dll *list, int data) {
	node* temp = newNode(data);
	if (list->tail == NULL) {
		list->tail = temp;
		list->head = temp;
	}
	else {
		temp->prev = list->tail;
		list->tail->next = temp;
		list->tail = temp;
	}
	return list;
}
