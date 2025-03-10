#include "vm.h"
#include "vm/instruction_set.h"
#include "util.h"
#include <stdint.h>
void execute_POP(MeatsVM *vm)
{
	TODO("POP r%u\n", fetch(vm));
}
void disasm_POP(MeatsVM *vm)
{
	printf("POP r%u\n", fetch(vm));
}
uint8_t *bytecode_POP(uint8_t reg)
{
	static uint8_t bytes[POP_INSTR_SIZE];
	bytes[0] = OP_POP;
	bytes[1] = reg;

	return bytes;
}