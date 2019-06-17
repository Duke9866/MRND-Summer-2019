#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	node* next;
	node* prev;
} node;

node* new_node(int data) {
	node* ele = (node*)malloc(sizeof(node));
	ele->data = data;
	ele->next = ele->prev = NULL;
	return ele;
}

node* merge(node* head1, node* head2) {
	if (head1 == NULL && head2 == NULL) {
		return NULL;
	}
	else if (head1 == NULL) {
		return head2;
	}
	else if (head2 == NULL) {
		return head1;
	}
	if (head1->data < head2->data) {
		head1->next = merge(head1->next, head2);
		if (head1->next != NULL) {
			head1->next->prev = head1;
		}
		return head1;
	}
	else {
		head2->next = merge(head1, head2->next);
		if (head2->next != NULL) {
			head2->next->prev = head2;
		}
		return head2;
	}
}

node* merge_sort(node* head) {
	node *mid, *tail;
	node *h1, *h2;
	mid = tail = head;
	if (head == NULL || head->next == NULL) {
		return head;
	}
	while (mid->next && tail->next && tail->next->next) {
		mid = mid->next;
		tail = tail->next->next;
	}
	h1 = head;
	h2 = mid->next;
	mid->next->prev = NULL;
	mid->next = NULL;
	h1 = merge_sort(h1);
	h2 = merge_sort(h2);
	head = merge(h1, h2);
	return head;
}

void printDLL(node* head) {
	node* iter = head;
	//print forward
	while (iter) {
		printf("%d ", iter->data);
		iter = iter->next;
	}
	printf("\n");
	iter = head;
	//print reverse
	while (iter->next) {
		iter = iter->next;
	}
	while (iter) {
		printf("%d ", iter->data);
		iter = iter->prev;
	}
	printf("\n\n");
}

node* input() {
	node* head1 = new_node(6);
	node* s2 = new_node(4);
	node* s3 = new_node(2);
	node* s4 = new_node(1);
	node* s5 = new_node(3);
	node* s6 = new_node(5);

	head1->next = s2;
	s2->next = s3;
	s3->next = s4;
	s4->next = s5;
	s5->next = s6;

	s6->prev = s5;
	s5->prev = s4;
	s4->prev = s3;
	s3->prev = s2;
	s2->prev = head1;
	
	return head1;
}

void init() {
	node* head = input();

	head = merge_sort(head);

	printDLL(head);
}