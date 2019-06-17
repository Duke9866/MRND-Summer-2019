#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>

typedef struct node {
	int data;
	node* next;
	node* random;
} node;

node* new_node(int data) {
	node* elem = (node*)malloc(sizeof(node));
	elem->data = data;
	elem->next = elem->random = NULL;
	return elem;
}

node* input() {
	node* head = new_node(1);
	node* s2 = new_node(2);
	node* s3 = new_node(3);
	node* s4 = new_node(4);
	node* s5 = new_node(5);
	node* s6 = new_node(6);

	head->next = s2;
	s2->next = s3;
	s3->next = s4;
	s4->next = s5;
	s5->next = s6;

	head->random = s3;
	s2->random = s5;
	s4->random = s5;
	s6->random = s4;

	return head;
}

node* clone_using_map(node* head) {
	node* elem = NULL, *iter = head, *new_head = NULL, *new_iter = NULL;
	std::unordered_map<node*, node*> map;
	while (iter) {
		if (new_head == NULL) {
			new_head = elem = new_node(iter->data);
			map[iter] = elem;
		}
		else {
			elem->next = new_node(iter->data);
			elem = elem->next;
			map[iter] = elem;
		}
		iter = iter->next;
	}
	iter = head;
	new_iter = new_head;
	while (iter) {
		if (iter->random != NULL) {
			new_iter->random = map[iter->random];
		}
		iter = iter->next;
		new_iter = new_iter->next;
	}
	return new_head;
}

node* clone(node* head) {
	node* elem = NULL, *iter = head, *temp_next;
	while (iter) {
		elem = new_node(iter->data);
		elem->next = iter->next;
		iter->next = elem;
		iter = iter->next->next;
	}
	iter = head;
	while (iter) {
		if (iter->random) {
			iter->next->random = iter->random->next;
		}
		iter = iter->next->next;
	}
	iter = head;
	head = iter->next;
	while (iter) {
		temp_next = iter->next;
		if (iter->next) {
			iter->next = iter->next->next;
		}
		iter = temp_next;
	}
	return head;
}

void printLLR(node* head) {
	node* iter = head;
	while (iter) {
		printf("(%d, %d) ", iter->data, iter->random != NULL ? iter->random->data : -1);
		iter = iter->next;
	}
	printf("\n");
}

void init() {
	node* head = input();
	//node* new_head = clone(head);
	node* new_head = clone_using_map(head);
	printLLR(head);
	printLLR(new_head);
}