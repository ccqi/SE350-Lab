#include "heap.h"

void heap_push(U32 **p_heap, MEMORY_BLOCK *block) {
	block->next = *p_heap;
	*p_heap = (U32*) block;
}

void *heap_pop(U32 **p_heap) {
	MEMORY_BLOCK* block = (MEMORY_BLOCK*) *p_heap;
	*p_heap = block->next;
	block->next = NULL;
	return block;
}

int heap_empty(U32 **p_heap) {
	return *p_heap == NULL;
}
