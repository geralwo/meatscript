#include "vm/bytecode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
void bytecode_init(Bytecode *bc)
{
    bc->size = 0;
    bc->capacity = 16;
    bc->bytes = malloc(16 * sizeof(uint8_t));
    memset(bc->bytes, 0, 16);
}

Bytecode *new_bytecode()
{
    Bytecode *bc = malloc(sizeof(Bytecode));
    bytecode_init(bc);
    return bc;
}

void free_bytecode(Bytecode *bc)
{
    free(bc->bytes);
    free(bc);
}

void bytecode_append(Bytecode *bc, uint8_t *data, size_t size)
{
    // Check if the current bytecode can accommodate the new data
    if (bc->size + size > bc->capacity)
    {
        // Reallocate memory to make space for the new data
        while (bc->size + size > bc->capacity)
        {
            bc->capacity *= 2; // Double the capacity until it fits
        }
        bc->bytes = realloc(bc->bytes, bc->capacity * sizeof(uint8_t));
        if (bc->bytes == NULL)
        {
            fprintf(stderr, "Memory reallocation failed!\n");
            exit(1);
        }
    }

    // Append the new data to the bytecode array
    memcpy(bc->bytes + bc->size, data, size);
    bc->size += size; // Update the size of the bytecode
}

void bytecode_dump(Bytecode *bc)
{
    printf("Bytecode size: %zu\n", bc->size);
    printf("Bytecode capacity: %zu\n", bc->capacity);
    if (bc->size > 0)
    {
        printf("Bytecode bytes:\n");
        for (size_t i = 0; i < bc->size; i++)
        {
            printf(" %02x ", bc->bytes[i]);
            if ((i + 1) % 16 == 0)
            {
                printf("\n");
            }
        }
        printf("\n");
    }
}

void write_bytecode_to_file(const char *filename, const uint8_t *bytecode, size_t length)
{
    FILE *file = fopen(filename, "wb"); // Open file in binary write mode
    if (!file)
    {
        perror("Failed to open file for writing");
        return;
    }

    size_t written = fwrite(bytecode, sizeof(uint8_t), length, file);
    if (written != length)
    {
        perror("Failed to write all bytecode data");
    }

    fclose(file);
}