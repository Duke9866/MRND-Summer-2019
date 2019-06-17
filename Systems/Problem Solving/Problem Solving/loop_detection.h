#include <stdio.h>
#include <stdlib.h>

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

//detects the loop and removes the cycle
node* detect_loop(node* head) {
	node *slow = head, *fast = head;
	if (head == NULL) {
		return NULL;
	}
	else if (slow == slow->next) {
		return slow;
	}
	do {
		slow = slow->next;
		fast = fast->next->next;
	} while (slow != fast && fast && fast->next);
	if (fast == NULL) {
		return NULL;
	}
	slow = head;
	do {
		slow = slow->next;
		fast = fast->next;
	} while (slow->next != fast->next);
	return fast->next;
}

void printList(node* head) {
	if (head == NULL) {
		printf("NULL");
	}
	while (head) {
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

node* input() {
	node* head = new_node(1);
	node* s2 = new_node(2);
	node* s3 = new_node(3);
	node* s4 = new_node(4);
	node* s5 = new_node(5);
	node* s6 = new_node(6);
	node* s7 = new_node(7);
	node* s8 = new_node(8);

	head->next = s2;
	s2->next = s3;
	s3->next = s4;
	s4->next = s5;
	s5->next = s6;
	s6->next = s7;
	s7->next = s8;

	//meeting at node 4
	s8->next = s4;

	return head;
	//return NULL;
}

void init() {
	node* head = input();
	node* meet;
	meet = detect_loop(head);
	if (meet == NULL) {
		printf("No cycle detected\n");
	}
	else {
		printf("%d\n", meet->data);
	}
	//printList(head);
}