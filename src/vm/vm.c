#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vm/instruction_set.h"
#include "util.h"

void (*execute_table[MAX_OPCODES])(MeatsVM *vm) = {
	[0x00] = execute_NOP,
	[0xDA] = execute_MOV,
	[0xDB] = execute_MOVI,
	[0xDC] = execute_MOVE,
	[0xAA] = execute_ADD,
	[0xFF] = execute_JMPE,
	[0xFA] = execute_JMP,
	[0xE0] = execute_HALT,
};

void (*disasm_table[MAX_OPCODES])(MeatsVM *vm) = {
	[0x00] = disasm_NOP,
	[0xDA] = disasm_MOV,
	[0xDB] = disasm_MOVI,
	[0xDC] = disasm_MOVE,
	[0xAA] = disasm_ADD,
	[0xFF] = disasm_JMPE,
	[0xFA] = disasm_JMP,
	[0xE0] = disasm_HALT,
};

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
	vm->Registers[RR] = 1; // enable the vm, RR = register 31
	for (int i = 0; i < VM_STACK_SIZE; i++)
		vm->Stack[i] = 0;
	vm->Heap = malloc(VM_HEAP_START_SIZE);
	vm->Program = NULL;
}

uint8_t fetch(MeatsVM *vm)
{
	if (vm->PC < vm->ProgramLength)
	{
		return vm->Program[vm->PC++];
	}
	printf("%ld is past Program\n", vm->PC);
	return 0x00;
}

void fetch_bytes(MeatsVM *vm, uint8_t *dest, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		dest[i] = fetch(vm);
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
			// printf("Executing 0x%.2X:\n",opcode);
			execute_table[opcode](vm);
		}
		else
		{
			printf("RUNTIME ERROR: Unknown instruction: 0x%.2X at PC=%ld\n", opcode, vm->PC - 1);
			meats_vm_print_asm(vm);
			meats_vm_dump_registers(vm);
			exit('r' + 't');
		}
	}
}

void meats_vm_dump_registers(MeatsVM *vm)
{
	for (int i = 0; i < VM_REGISTER_COUNT; i++)
	{

		printf("  [  r%.02d: ", i);
		print_bits(vm->Registers[i]);
		printf(" = %ld  ", vm->Registers[i]);
		printf("] \n");
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
}

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
			printf("DISASM: Unknown instruction: 0x%.2X at PC=%ld\n", opcode, vm->PC - 1);
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
		printf("VM Program Size: %ld\n", vm->ProgramLength);
		for (size_t i = 0; i < vm->ProgramLength; i++)
		{
			printf("  %02x ", vm->Program[i]);
			if ((i + 1) % 8 == 0)
				printf("\n");
		}
		printf("\n");
	}
}
