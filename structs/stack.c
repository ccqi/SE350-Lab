#include <stdlib.h>
#include <stdio.h>
#include "node.c"

typedef struct {
	int nodeSize;
	node *root;
} stack;

stack* stackCreate(int size) {
	stack *s = malloc(sizeof(stack));
	s->nodeSize = size;
	s->root = NULL;
	return s;
}

void stackPush(stack *s, void *value) {
	node *newNode = malloc(sizeof(node));
	newNode->value = value;
	newNode->next = s->root;
	s->root = newNode;
}

void* stackPop(stack *s) {
	node *root = s->root;
	s->root = s->root->next;
	void *value = root->value;
	free(root);
	return value;
}

void* stackPeek(stack *s) {
	return s->root->value;
}

int stackEmpty(stack *s) {
	return !s->root;
}

int main(void) {
	stack *s = stackCreate(128);
	int *a = malloc(sizeof(int));
	*a = 1;
	int *b = malloc(sizeof(int));;
	*b = 2;
	int *c = malloc(sizeof(int));;
	*c = 3;
	stackPush(s, a);
	printf ("%d \n", *((int*)s->root->value));
	printf("%d \n", *((int*)stackPeek(s)));
	stackPush(s, b);
	printf ("%d \n", *((int*)s->root->value));
	printf("%d \n", *((int*)stackPeek(s)));
	stackPush(s, c);
	printf ("%d \n", *((int*)s->root->value));
	printf("%d \n", *((int*)stackPeek(s)));
	printf("%d \n", *((int*)stackPop(s)));
	printf("%d \n", *((int*)stackPeek(s)));
	printf("%d \n", *((int*)stackPop(s)));
	printf("Empty: %d \n", stackEmpty(s));
	printf("%d \n", *((int*)stackPeek(s)));
	printf("%d \n", *((int*)stackPop(s)));
	printf("Empty: %d \n", stackEmpty(s));
	free(s);
	free(a);
	free(b);
	free(c);
	return 0;
}
