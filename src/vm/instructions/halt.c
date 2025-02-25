#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_HALT(MeatsVM *vm)
{
	switch (vm_get_register(vm, 31))
	{
	case 0:
	{
		exit(0);
		break;
	}
	default:
	{
		meats_vm_dump_bytecode(vm);
		meats_vm_dump_registers(vm);
		meats_vm_print_asm(vm);
		exit(vm_get_register(vm, 31));
	}
	}
}

void disasm_HALT()
{
	printf("HALT\n");
}

uint8_t *bytecode_HALT()
{
	static uint8_t bytecode[HALT_INSTR_SIZE];
	bytecode[0] = 0xE0;
	return bytecode;
}
