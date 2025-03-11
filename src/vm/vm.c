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
	#ifndef _WIN32
	printf("PC '%ld' is past Program!!!\n", vm->PC);
	#else
	printf("PC '%lld' is past Program!!!\n", vm->PC);
	#endif
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
			#ifndef _WIN32
			printf("RUNTIME ERROR: Unknown instruction: 0x%.2X at PC=%ld\n", opcode, vm->PC - 1);
			#else
			printf("RUNTIME ERROR: Unknown instruction: 0x%.2X at PC=%lld\n", opcode, vm->PC - 1);
			#endif
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

void meats_vm_dump_registers(MeatsVM *vm)
{
	for (int i = 0; i < VM_REGISTER_COUNT; i++)
	{
		printf("  [  r%.02d: ", i);
		print_bits(vm->Registers[i]);

		#ifndef _WIN32
		printf(" = %ld  ", vm->Registers[i]);
		#else
		printf(" = %lld  ", vm->Registers[i]);
		#endif
		printf("] \n");
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
}
void meats_vm_dump_mem(MeatsVM *vm);

void meats_vm_dump_mem(MeatsVM *vm)
{
	for (int i = 0; i < (int)vm->HeapSize; i++)
	{
		if (i % 8 == 0)
			printf("\n");
		printf("m%u", vm->Heap[i]);
	}
	printf("\n");
	for (int i = 0; i < VM_STACK_SIZE; i++)
	{
		if (i % 8 == 0)
			printf("\n");
		printf("%u", vm->Stack[i]);
	}
	printf("\n");
}

void meats_vm_print_asm(MeatsVM *vm)
{
	printf("::: ASM BEGIN :::\n");
	vm->PC = 0;
	while (vm->PC < vm->ProgramLength)
	{
		uint8_t opcode = fetch(vm);
		if (disasm_table[opcode])
			disasm_table[opcode](vm);
		else
		{

			#ifndef _WIN32
			printf("DISASM: Unknown instruction: 0x%.2X at PC=%ld\n", opcode, vm->PC - 1);
			#else
			printf("DISASM: Unknown instruction: 0x%.2X at PC=%lld\n", opcode, vm->PC - 1);
			#endif
			break;
		}
	}
	printf("::: ASM END :::\n");
	vm->PC = 0;
}

void meats_vm_dump_bytecode(MeatsVM *vm)
{
	if (vm->ProgramLength == 0)
	{
		printf(":: VM has no bytecode program\n");
	}
	else
	{

		#ifndef _WIN32
		printf("VM Program Size: %ld\n", vm->ProgramLength);
		#else
		printf("VM Program Size: %lld\n", vm->ProgramLength);
		#endif
		for (size_t i = 0; i < vm->ProgramLength; i++)
		{
			printf("  %02x ", vm->Program[i]);
			if ((i + 1) % 8 == 0)
				printf("\n");
		}
		printf("\n");
	}
}

void meats_vm_print_stats(MeatsVM *vm)
{
	#ifndef _WIN32
	printf("Bytecode Size: %ld\tR31: %ld\nHeapPtr: %p\tHeapSize: %ld\n", vm->ProgramLength, vm_get_register(vm, 31), (void *)vm->Heap, vm->HeapSize);
	#else
	printf("Bytecode Size: %lld\tR31: %lld\nHeapPtr: %p\tHeapSize: %lld\n", vm->ProgramLength, vm_get_register(vm, 31), (void *)vm->Heap, vm->HeapSize);
	#endif

	meats_vm_print_asm(vm);
}
