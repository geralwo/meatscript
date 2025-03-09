#ifndef MEATLIB_MALLOC_H
#define MEATLIB_MALLOC_H
#include <stddef.h>
#include <stdio.h>
#include "stdlib/std_malloc.h"

typedef struct s_MeatArena
{
	void *Data;
	size_t Size;
	size_t FreeSize;
	void *Next;
} MeatArena;

extern MeatArena MEAT_MEM_POOL;

typedef struct s_MeatPiece
{
	void *ptr;
	size_t size;
} MeatPiece;

void _meat_mem_init();
MeatPiece meat_alloc(size_t size);
void meat_free(MeatPiece *ptr);

#endif // MEATLIB_MALLOC_H