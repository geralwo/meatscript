#ifndef VM_H
#define VM_H
#include <stdint.h>
#include <stddef.h>

#define VM_REGISTER_COUNT 32
#define VM_STACK_SIZE 16384
#define VM_HEAP_START_SIZE 4096

#define VM_FETCH_BUFFER_SIZE sizeof(size_t)
#include "vm/bytecode.h"

typedef struct
{
	uint64_t Registers[VM_REGISTER_COUNT]; // General-purpose registers
	uint8_t Stack[VM_STACK_SIZE];	       // Stack
	size_t PC;			       // Program Counter
	size_t ProgramLength;
	uint8_t *Program;
	size_t HeapSize;
	uint8_t *Heap;

} MeatsVM;

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R22 22
#define RR 31

uint8_t fetch(MeatsVM *vm);
void fetch_bytes(MeatsVM *vm, uint8_t *dest, size_t size);

size_t vm_bytes_to_size_t(const uint8_t bytes[], size_t byte_count);
uint16_t vm_bytes_to_uint16(const uint8_t bytes[2]);
uint32_t vm_bytes_to_uint32(const uint8_t bytes[4]);
uint64_t vm_bytes_to_uint64(const uint8_t bytes[8]);

uint8_t *vm_size_t_to_bytes(size_t value);
uint8_t *vm_uint64_to_bytes(uint64_t value);
uint8_t *vm_uint32_to_bytes(uint32_t value);
uint8_t *vm_uint16_to_bytes(uint16_t value);

// Function to set a register's value
void vm_set_register(MeatsVM *vm, uint8_t reg, uint64_t value);

// Function to get a register's value
uint64_t vm_get_register(MeatsVM *vm, uint8_t reg);

// Function to initialize the VM
void meats_vm_init(MeatsVM *vm);
MeatsVM *meats_vm_new(Bytecode *bc);
void meats_vm_run(MeatsVM *vm);
void meats_vm_dump_registers(MeatsVM *vm);
void meats_vm_dump_bytecode(MeatsVM *vm);
void meats_vm_print_asm(MeatsVM *vm);
void meats_vm_print_stats(MeatsVM *vm);
#endif // VM_H
