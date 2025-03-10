#include "vm.h"
#include "vm/instruction_set.h"
#include "stdlib/std_print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_SUB(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t value = vm_bytes_to_uint64(bytes);
	vm_set_register(vm, reg, vm_get_register(vm, reg) - value);
}

void disasm_SUB(MeatsVM *vm)
{
	uint8_t reg = fetch(vm);
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t value = vm_bytes_to_uint64(bytes);
	meats_print("SUB r%u %ld\n", reg, value);
}

uint8_t *bytecode_SUB(uint8_t reg, uint64_t value)
{
	static uint8_t bytecode[SUB_INSTR_SIZE];
	bytecode[0] = OP_SUB;
	bytecode[1] = reg;

	// Split the 64-bit value into 8 bytes and store them in the bytecode
	bytecode[2] = (value >> 56) & 0xFF; // Most significant byte
	bytecode[3] = (value >> 48) & 0xFF;
	bytecode[4] = (value >> 40) & 0xFF;
	bytecode[5] = (value >> 32) & 0xFF;
	bytecode[6] = (value >> 24) & 0xFF;
	bytecode[7] = (value >> 16) & 0xFF;
	bytecode[8] = (value >> 8) & 0xFF;
	bytecode[9] = value & 0xFF; // Least significant byte

	return bytecode;
}
