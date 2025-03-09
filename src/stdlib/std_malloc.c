#include "stdlib/std_malloc.h"
#include <stdlib.h>
#include <string.h>

MeatArena MEAT_MEM_POOL;

void _meat_mem_init()
{
	// MEAT_MEM_POOL = malloc(sizeof(MeatArena));

	MEAT_MEM_POOL.Size = 64;
	MEAT_MEM_POOL.FreeSize = 64;
	MEAT_MEM_POOL.Data = malloc(MEAT_MEM_POOL.Size);
	if (MEAT_MEM_POOL.Data == NULL)
	{
		printf("Memory initialization failed!\n");
		return;
	}
	MEAT_MEM_POOL.Next = MEAT_MEM_POOL.Data;
}

MeatPiece meat_alloc(size_t size)
{
	if (size < MEAT_MEM_POOL.FreeSize)
	{
		return (MeatPiece){MEAT_MEM_POOL.Next, size};
	}
	return (MeatPiece){NULL, 0};
}
void meat_free(MeatPiece *mptr)
{
	memset(mptr->ptr, 0, mptr->size);
	MEAT_MEM_POOL.FreeSize += mptr->size;
	// free(mptr->ptr);
}