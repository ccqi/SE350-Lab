#ifndef _HEAP_H
#define _HEAP_H

#include "rtx.h"

void heap_push(U32 *p_heap, MEMORY_BLOCK *block);
void *heap_pop(U32 *p_heap);
int heap_empty(U32 *p_heap);

#endif
