#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>

void execute_POP(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	uint64_t sp = vm_get_register(vm, 29);

	if (sp == 0) // Prevent stack underflow
	{
		printf(":: ERROR: Stack underflow!\n");
		return;
	}

	sp--; // Decrement stack pointer first
	uint64_t result = vm->Stack[sp];
	vm_set_register(vm, 29, sp);
	vm_set_register(vm, reg, result);

	printf(":: POP: sp:%ld, reg: r%u, value: %ld\n", sp, reg, result);
}

void disasm_POP(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	printf("POP r%u\n", reg);
}

uint8_t *bytecode_POP(uint8_t reg)
{
	static uint8_t bytecode[POP_INSTR_SIZE];
	bytecode[0] = OP_POP;
	bytecode[1] = reg;
	return bytecode;
}
