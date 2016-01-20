#include "node.c"
#include <string.h>
#include <stdlib.h>

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
	// newNode->value = malloc(s->nodeSize);
	// memcpy(newNode->value, value, s->nodeSize);
	newNode->value = value;
	
	newNode->next = s->root;
	s->root = newNode;
}

void* stackPop(stack *s) {
	node *root = s->root;
	s->root = s->root->next;
	return root->value;
}


int main(void) {
	stack *s = stackCreate(128);
	int *a;
	*a = 1;
	int *b;
	*b = 2;
	int *c;
	*c = 3;;
	stackPush(s, a);
	printf ("%d \n", *((int*)s->root->value));
	stackPush(s, b);
	printf ("%d \n", *((int*)s->root->value));
	stackPush(s, c);
	printf ("%d \n", *((int*)s->root->value));

	printf ("%d \n", *((int*)(stackPop(s)->value)));
	printf ("%d \n", *((int*)(stackPop(s)->value)));
	printf ("%d \n", *((int*)(stackPop(s)->value)));
	return 0;
}
