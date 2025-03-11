#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_JMP(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	// printf("jmp addr: %ld\n", address);
	if (address >= vm->ProgramLength)
	{
		printf("Invalid jump address '%zu'\n", address);
		exit(1);
		return;
	}
	vm->PC = address;
}

void disasm_JMP(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t address = vm_bytes_to_uint64(bytes);
	printf("JMP %zu\n", address);
}

uint8_t *bytecode_JMP(uint64_t address)
{
	static uint8_t bytecode[JMP_INSTR_SIZE];

	bytecode[0] = OP_JMP;		      // JMP opcode
	bytecode[1] = (address >> 56) & 0xFF; // Most significant byte
	bytecode[2] = (address >> 48) & 0xFF;
	bytecode[3] = (address >> 40) & 0xFF;
	bytecode[4] = (address >> 32) & 0xFF;
	bytecode[5] = (address >> 24) & 0xFF;
	bytecode[6] = (address >> 16) & 0xFF;
	bytecode[7] = (address >> 8) & 0xFF;
	bytecode[8] = address & 0xFF; // Least significant byte
	return bytecode;
}
