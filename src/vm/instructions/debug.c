#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void execute_DEBUG(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	TODO("Implement parsing of flags in asm\n");
	vm_set_flag(vm, VM_FLAG_PRINT_DEBUG);
}

void disasm_DEBUG(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t flags = vm_bytes_to_uint64(bytes);
	printf("DEBUG %ld\n", flags);
}

uint8_t *bytecode_DEBUG(uint64_t flags)
{
	static uint8_t bytecode[DEBUG_INSTR_SIZE];
	bytecode[0] = OP_DEBUG;
	bytecode[1] = (flags >> 56) & 0xFF; // Most significant byte
	bytecode[2] = (flags >> 48) & 0xFF;
	bytecode[3] = (flags >> 40) & 0xFF;
	bytecode[4] = (flags >> 32) & 0xFF;
	bytecode[5] = (flags >> 24) & 0xFF;
	bytecode[6] = (flags >> 16) & 0xFF;
	bytecode[7] = (flags >> 8) & 0xFF;
	bytecode[8] = flags & 0xFF; // Least significant byte
	return bytecode;
}
