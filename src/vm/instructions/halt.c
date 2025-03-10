#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>

void execute_HALT(MeatsVM *vm)
{
	uint64_t ret = vm_get_register(vm, 31);
	vm_set_flag(vm, VM_FLAG_HALT);
	//  vm_set_flag(vm, VM_FLAG_PRINT_DEBUG);
	if (vm_flag_is_set(vm, VM_FLAG_PRINT_DEBUG))
	{
		meats_vm_print_asm(vm);
		meats_vm_dump_bytecode(vm);
		meats_vm_dump_registers(vm);
	}
	else if (vm_flag_is_set(vm, VM_FLAG_HALT))
	{
		exit(ret);
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
