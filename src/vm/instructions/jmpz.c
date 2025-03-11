#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_JMPZ(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	if (address >= vm->ProgramLength)
	{
		printf("Invalid jump address '%zu'\n", address);
		exit(1);
		return;
	}
	if (vm_get_register(vm, reg1) == 0)
	{
		vm->PC = address;
	}
}

void disasm_JMPZ(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	printf("JMPZ r%u == 0 -> %zu\n", reg1, address);
}

uint8_t *bytecode_JMPZ(uint8_t reg1, uint64_t address)
{
	static uint8_t bytecode[JMPZ_INSTR_SIZE];

	bytecode[0] = OP_JMPZ;
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
