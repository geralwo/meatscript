#include "vm/instruction_set.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
void execute_PUSH(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	uint64_t value;
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	value = vm_bytes_to_uint64(bytes);
	TODO("PUSH %zu\n", value);
}
void disasm_PUSH(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	uint64_t value;
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	value = vm_bytes_to_uint64(bytes);
	printf("PUSH %zu\n", value);
}
uint8_t *bytecode_PUSH(uint64_t value)
{
	uint8_t *bytecode = malloc(PUSH_INSTR_SIZE);
	if (bytecode == NULL)
		exit(123);
	bytecode[0] = OP_PUSH;
	// Split the 64-bit value into 8 bytes and store them in the bytecode
	bytecode[1] = (value >> 56) & 0xFF; // Most significant byte
	bytecode[2] = (value >> 48) & 0xFF;
	bytecode[3] = (value >> 40) & 0xFF;
	bytecode[4] = (value >> 32) & 0xFF;
	bytecode[5] = (value >> 24) & 0xFF;
	bytecode[6] = (value >> 16) & 0xFF;
	bytecode[7] = (value >> 8) & 0xFF;
	bytecode[8] = value & 0xFF; // Least significant byte

	return bytecode;
}