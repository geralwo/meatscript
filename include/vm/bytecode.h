#ifndef BYTECODE_H
#define BYTECODE_H
#include <stdint.h>
#include <stddef.h>

typedef struct
{
	size_t size;
	size_t capacity;
	uint8_t *bytes;
} Bytecode;

void bytecode_init(Bytecode *bc);
Bytecode *new_bytecode(void);
void free_bytecode(Bytecode *bc);
void bytecode_append(Bytecode *bc, uint8_t *data, size_t size);
void bytecode_dump(Bytecode *bc);

#endif // BYTECODE_H
