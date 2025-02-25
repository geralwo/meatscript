#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_MOVE(MeatsVM *vm)
{

	uint8_t reg = fetch(vm);
	static uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t value = vm_bytes_to_uint64(bytes);
	// static uint8_t bytes[sizeof(uint64_t)];
	// fetch_bytes(vm,bytes,sizeof(uint64_t));
	vm_set_register(vm, reg, value);
}

void disasm_MOVE(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	static uint8_t bytes[sizeof(uint64_t)];
	uint64_t value;
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	value = vm_bytes_to_uint64(bytes);
	printf("MOVE r%d %zu\n", reg, value);
}

uint8_t *bytecode_MOVE(uint8_t reg, uint64_t value)
{
	static uint8_t bytecode[MOVE_INSTR_SIZE];

	bytecode[0] = 0xDC;
	bytecode[1] = reg;
	for (size_t i = 0; i < sizeof(uint64_t); i++)
	{
		bytecode[2 + i] = (value >> (8 * (sizeof(uint64_t) - 1 - i))) & 0xFF;
	}

	return bytecode;
}
