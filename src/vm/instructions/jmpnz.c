#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_JMPNZ(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	if (vm_get_register(vm, reg1) != 0)
	{
		vm->PC = address;
	}
}

void disasm_JMPNZ(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	printf("JMPNZ r%u == 0 -> %zu\n", reg1, address);
}

uint8_t *bytecode_JMPNZ(uint8_t reg1, uint64_t address)
{
	static uint8_t bytecode[JMPNZ_INSTR_SIZE];

	bytecode[0] = OP_JMPNZ;
	bytecode[1] = reg1;
	bytecode[2] = (address >> 56) & 0xFF; // Most significant byte
	bytecode[3] = (address >> 48) & 0xFF;
	bytecode[4] = (address >> 40) & 0xFF;
	bytecode[5] = (address >> 32) & 0xFF;
	bytecode[6] = (address >> 24) & 0xFF;
	bytecode[7] = (address >> 16) & 0xFF;
	bytecode[8] = (address >> 8) & 0xFF;
	bytecode[9] = address & 0xFF; // Least significant byte
	return bytecode;
}
