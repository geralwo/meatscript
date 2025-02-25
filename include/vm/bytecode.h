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
Bytecode *new_bytecode();
void free_bytecode();
void bytecode_append(Bytecode *bc, uint8_t *data, size_t size);
void bytecode_append_bc(Bytecode *dest, Bytecode *src);
void bytecode_create_label(Bytecode *bc, const char *name);
size_t bytecode_get_label(Bytecode *bc, const char *name);
void bytecode_dump(Bytecode *bc);

#endif // BYTECODE_H
