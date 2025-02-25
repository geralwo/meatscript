#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_JMP(MeatsVM *vm)
{
	size_t address = R7;
	if (address >= vm->ProgramLength && address == 0)
	{
		printf("Invalid jump address '%zu'\n", address);
		exit(1);
		return;
	}
	vm->PC = address;
}

void disasm_JMP()
{

	printf("JMP\n");
}

uint8_t *bytecode_JMP(size_t address)
{
	static uint8_t bytecode[JMP_INSTR_SIZE];

	bytecode[0] = 0xFA; // JMPE opcode

	uint8_t *byte_value_addr = vm_size_t_to_bytes(address);

	// Copy the bytes to the instruction buffer
	for (size_t i = 0; i < sizeof(size_t); i++)
	{
		bytecode[1 + i] = byte_value_addr[i]; // Copy bytes from vm_size_t_to_bytes()
	}

	return bytecode;
}
