#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_JMPE(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	if (vm_get_register(vm, reg1) == vm_get_register(vm, reg2))
	{
		vm->PC = address;
	}
}

void disasm_JMPE(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	printf("JMPE r%u == r%u -> %zu\n", reg1, reg2, address);
}

uint8_t *bytecode_JMPE(uint8_t reg1, uint8_t reg2, uint64_t address)
{
	static uint8_t bytecode[JMPE_INSTR_SIZE];

	bytecode[0] = OP_JMPE;
	bytecode[1] = reg1;
	bytecode[2] = reg2;
	bytecode[3] = (address >> 56) & 0xFF; // Most significant byte
	bytecode[4] = (address >> 48) & 0xFF;
	bytecode[5] = (address >> 40) & 0xFF;
	bytecode[6] = (address >> 32) & 0xFF;
	bytecode[7] = (address >> 24) & 0xFF;
	bytecode[8] = (address >> 16) & 0xFF;
	bytecode[9] = (address >> 8) & 0xFF;
	bytecode[10] = address & 0xFF; // Least significant byte
	return bytecode;
}
