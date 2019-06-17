#include <stdlib.h>
#include <stdio.h>

typedef struct node {
	int data;
	struct node* next;
} node;

node* new_node(int data) {
	node* ele = (node*)malloc(sizeof(node));
	ele->data = data;
	ele->next = NULL;
	return ele;
}

node* createList(int* arr, int size) {
	int i;
	node* head = new_node(arr[0]);
	node* s = head;
	for (i = 1; i < size; i++) {
		s->next = new_node(arr[i]);
		s = s->next;
	}
	return head;
}

void reverse(node* head, node* tail) {
	node* h = head;
	node* tnext = tail->next;
	tail->next = NULL;
	node* curr = head, *prev = NULL, *next = NULL;
	while (curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	head = prev;
	h->next = tnext;
}

node* reverseKNodes(node* head, int k) {
	int n;
	node* s, *h, *t, *p;
	s = head;
	h = t = p = NULL;
	while (s) {
		p = h;
		h = t = s;
		n = 1;
		while (n < k && s) {
			s = s->next;
			n++;
		}
		if (n < k || s == NULL) {
			break;
		}
		t = s;
		s = s->next;
		reverse(h, t);
		if (p == NULL) {
			p = h;
			head = t;
		}
		else {
			p->next = t;
		}
	}
	return head;
}

void printLL(node* head) {
	while (head) {
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

node* input(int *k) {
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int n = 8;
	node* head = createList(arr, n);
	*k = 3;
	return head;
}

void init() {
	int *k = (int*)malloc(sizeof(int));
	node* head = input(k);
	printLL(head);
	head = reverseKNodes(head, *k);
	printLL(head);
}