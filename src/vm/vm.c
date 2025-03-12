#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vm.h"
#include "vm/instruction_set.h"
#include "util.h"

MeatsVM *meats_vm_new(Bytecode *bc)
{
	MeatsVM *vm = malloc(sizeof(MeatsVM));
	meats_vm_init(vm);
	vm->Program = bc->bytes;
	vm->ProgramLength = bc->size;
	return vm;
}

void meats_vm_init(MeatsVM *vm)
{
	for (int i = 0; i < VM_REGISTER_COUNT; i++)
		vm->Registers[i] = 0;
	vm->Registers[30] = 1;
	for (int i = 0; i < VM_STACK_SIZE; i++)
		vm->Stack[i] = 0;
	vm->Heap = malloc(VM_HEAP_START_SIZE);
	vm->Program = NULL;
	vm->ProgramLength = 0;
}

uint8_t fetch(MeatsVM *vm)
{
	if (vm->PC < vm->ProgramLength)
	{
		return vm->Program[vm->PC++];
	}
	printf("PC '%ld' is past Program!!!\n", vm->PC);
	return 0x00;
}

void fetch_bytes(MeatsVM *vm, uint8_t *dest, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (dest != NULL)
			dest[i] = fetch(vm);
		else
			fetch(vm);
		// printf(": 0x%.2X :\n",dest[i]);
	}
	// printf(":: fetched %ld bytes\n",size);
}

void meats_vm_run(MeatsVM *vm)
{
	while (vm->PC < vm->ProgramLength)
	{
		uint8_t opcode = fetch(vm);

		if (execute_table[opcode])
		{
			// printf(" Executing 0x%.2X [%ld]:\n", opcode, vm->PC);
			execute_table[opcode](vm);
		}
		else
		{
			printf("RUNTIME ERROR: Unknown instruction: 0x%.2X at PC=%ld\n", opcode, vm->PC - 1);
			vm_set_flag(vm, VM_FLAG_HALT);
			vm_set_flag(vm, VM_FLAG_PRINT_DEBUG);
			execute_table[OP_HALT](vm);
		}
	}
	printf("!!! EXITED WITH NO HALT !!!\n");
	printf("!!! EXITED WITH NO HALT !!!\n");
	printf("!!! EXITED WITH NO HALT !!!\n");
	printf("!!! EXITED WITH NO HALT !!!\n");
	printf("!!! EXITED WITH NO HALT !!!\n");
	vm_set_flag(vm, VM_FLAG_HALT);
	vm_set_flag(vm, VM_FLAG_PRINT_DEBUG);
	execute_table[OP_HALT](vm);
}