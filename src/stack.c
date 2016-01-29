#include "node.h"

typedef struct {
	int nodeSize;
	NODE *root;
} STACK;

STACK* stack_create(int size) {
	STACK *s = malloc(sizeof(STACK));
	s->nodeSize = size;
	s->root = NULL;
	return s;
}

void stack_push(STACK *s, void *value) {
	NODE *newNode = malloc(sizeof(NODE));
	newNode->value = value;
	newNode->next = s->root;
	s->root = newNode;
}

void* stack_pop(STACK *s) {
	NODE *root = s->root;
	s->root = s->root->next;
	void *value = root->value;
	free(root);
	return value;
}

void* stack_peek(STACK *s) {
	return s->root->value;
}

int stack_empty(STACK *s) {
	return !s->root;
}
