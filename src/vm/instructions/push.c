#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>

void execute_PUSH(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	uint64_t sp = vm_get_register(vm, 29);

	if (sp >= VM_STACK_SIZE) // Prevent stack overflow
	{
		printf(":: ERROR: Stack overflow!\n");
		return;
	}

	uint64_t reg_val = vm_get_register(vm, reg);
	vm->Stack[sp] = reg_val;	 // Store value at current SP
	vm_set_register(vm, 29, sp + 1); // Increment SP *after* storing

	printf(":: PUSH: sp:%ld, reg: r%u, value: %ld\n", sp, reg, reg_val);
}

void disasm_PUSH(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	printf("PUSH r%u\n", reg);
}

uint8_t *bytecode_PUSH(uint8_t reg)
{
	static uint8_t bytecode[PUSH_INSTR_SIZE];
	bytecode[0] = OP_PUSH;
	bytecode[1] = reg;
	return bytecode;
}
