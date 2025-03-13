#ifndef VM_H
#define VM_H
#include <stdint.h>
#include <stddef.h>

#define VM_REGISTER_COUNT 32
#define VM_STACK_SIZE 1024
#define VM_HEAP_START_SIZE 4096

#define VM_FLAG_ZERO (1ULL << 0)
#define VM_FLAG_OVERFLOW (1ULL << 1)
#define VM_FLAG_HALT (1ULL << 63)
#define VM_FLAG_PRINT_DEBUG (1ULL << 42)

#define VM_FETCH_BUFFER_SIZE sizeof(size_t)
#include "vm/bytecode.h"

// Special purpose registers
// r0  program counter
// r29 stack pointer
// r30 return code
// r31 flags
typedef struct
{
	uint64_t Registers[VM_REGISTER_COUNT]; // General-purpose registers
	uint64_t Stack[VM_STACK_SIZE];	       // Stack
	size_t PC;			       // Program Counter
	size_t ProgramLength;
	size_t HeapSize;
	uint8_t *Program;
	uint8_t *Heap;

} MeatsVM;

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

// Function to manipulate registers
void vm_set_register(MeatsVM *vm, uint8_t reg, uint64_t value);
uint64_t vm_get_register(MeatsVM *vm, uint8_t reg);

// Function to set set Flags

void vm_set_flag(MeatsVM *vm, uint64_t flag);
void vm_unset_flag(MeatsVM *vm, uint64_t flag);
int vm_flag_is_set(MeatsVM *vm, uint64_t flag);

// Function to initialize the VM
void meats_vm_init(MeatsVM *vm);
MeatsVM *meats_vm_new(Bytecode *bc);

// Functions to run VM
void meats_vm_run(MeatsVM *vm);

// Functions to debug VM
void meats_vm_dump_registers(MeatsVM *vm);
void meats_vm_dump_bytecode(MeatsVM *vm);
void meats_vm_print_asm(MeatsVM *vm);
void meats_vm_print_stats(MeatsVM *vm);
void meats_vm_dump_mem(MeatsVM *vm);

#endif // VM_H
