#include "vm.h"
#include "vm/instruction_set.h"
#include "stdlib/std_print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_DIVR(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	vm_set_register(vm, reg1, vm_get_register(vm, reg1) / vm_get_register(vm, reg2));
}

void disasm_DIVR(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	meats_print("DIVR r%u r%u\n", reg1, reg2);
}

uint8_t *bytecode_DIVR(uint8_t reg, uint8_t reg2)
{
	static uint8_t bytecode[DIVR_INSTR_SIZE];
	bytecode[0] = OP_DIVR;
	bytecode[1] = reg;
	bytecode[2] = reg2;
	return bytecode;
}
