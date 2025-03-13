#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "vm.h"
#include "vm/instruction_set.h"

void execute_PRINT(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t fd = vm_bytes_to_uint64(bytes);
	uint8_t bytes2[sizeof(uint64_t)];
	fetch_bytes(vm, bytes2, sizeof(uint64_t));
	uint64_t addr = vm_bytes_to_uint64(bytes2);
	uint8_t bytes3[sizeof(uint64_t)];
	fetch_bytes(vm, bytes3, sizeof(uint64_t));
	uint64_t size = vm_bytes_to_uint64(bytes3);
	// printf("write %lu, %lu, %lu\n", fd, size, addr);
	write(fd, &vm->Program[addr], size);
	return;
}

void disasm_PRINT(MeatsVM *vm)
{
	uint8_t bytes[sizeof(uint64_t)];
	fetch_bytes(vm, bytes, sizeof(uint64_t));
	uint64_t fd = vm_bytes_to_uint64(bytes);

	uint8_t bytes2[sizeof(uint64_t)];
	fetch_bytes(vm, bytes2, sizeof(uint64_t));
	uint64_t size = vm_bytes_to_uint64(bytes2);

	uint8_t bytes3[sizeof(uint64_t)];
	fetch_bytes(vm, bytes3, sizeof(uint64_t));
	uint64_t addr = vm_bytes_to_uint64(bytes3);

	printf("PRINT %lu %lu %lu\n", fd, size, addr);
}

uint8_t *bytecode_PRINT(uint64_t fd, uint64_t size, uint64_t addr)
{
	static uint8_t bytecode[PRINT_INSTR_SIZE];
	bytecode[0] = OP_PRINT;
	bytecode[1] = (fd >> 56) & 0xFF; // Most significant byte
	bytecode[2] = (fd >> 48) & 0xFF;
	bytecode[3] = (fd >> 40) & 0xFF;
	bytecode[4] = (fd >> 32) & 0xFF;
	bytecode[5] = (fd >> 24) & 0xFF;
	bytecode[6] = (fd >> 16) & 0xFF;
	bytecode[7] = (fd >> 8) & 0xFF;
	bytecode[8] = fd & 0xFF;	   // Least significant byte
	bytecode[9] = (size >> 56) & 0xFF; // Most significant byte
	bytecode[10] = (size >> 48) & 0xFF;
	bytecode[11] = (size >> 40) & 0xFF;
	bytecode[12] = (size >> 32) & 0xFF;
	bytecode[13] = (size >> 24) & 0xFF;
	bytecode[14] = (size >> 16) & 0xFF;
	bytecode[15] = (size >> 8) & 0xFF;
	bytecode[16] = size & 0xFF;	    // Least significant byte
	bytecode[17] = (addr >> 56) & 0xFF; // Most significant byte
	bytecode[18] = (addr >> 48) & 0xFF;
	bytecode[19] = (addr >> 40) & 0xFF;
	bytecode[20] = (addr >> 32) & 0xFF;
	bytecode[21] = (addr >> 24) & 0xFF;
	bytecode[22] = (addr >> 16) & 0xFF;
	bytecode[23] = (addr >> 8) & 0xFF;
	bytecode[24] = addr & 0xFF; // Least significant byte

	return bytecode;
}

// uint8_t *bytecode_STRING(const char *str)
// {
// 	size_t str_len = strlen(str);
// 	uint8_t str_bytes[str_len];

// 	return str_bytes;
// }
