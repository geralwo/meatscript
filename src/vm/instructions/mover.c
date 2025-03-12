#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_MOVER(MeatsVM *vm)
{
	uint8_t dest = fetch(vm);
	uint8_t src = fetch(vm);
	vm_set_register(vm, dest, vm_get_register(vm, src));
}

void disasm_MOVER(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	uint8_t reg2 = fetch(vm);
	printf("MOVER r%d r%u\n", reg, reg2);
}

uint8_t *bytecode_MOVER(uint8_t dest, uint8_t src)
{
	uint8_t *bytecode = malloc(MOVER_INSTR_SIZE);
	if (bytecode == NULL)
		exit(123);
	bytecode[0] = OP_MOVER;
	bytecode[1] = dest;
	bytecode[2] = src;

	return bytecode;
}
