#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_JMP(MeatsVM *vm)
{
	size_t address = vm_get_register(vm, 7);
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

uint8_t *bytecode_JMP()
{
	static uint8_t bytecode[JMP_INSTR_SIZE];

	bytecode[0] = 0xFA; // JMP opcode

	return bytecode;
}
