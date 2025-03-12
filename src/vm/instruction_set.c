#include <string.h>
#include "vm/instruction_set.h"
#include "util.h"

void (*execute_table[UINT8_MAX + 1])(MeatsVM *vm) = {
    [OP_NOP] = execute_NOP,
    [OP_MOV] = execute_MOV,
    [OP_MOVI] = execute_MOVI,
    [OP_MOVE] = execute_MOVE,
    [OP_MOVER] = execute_MOVER,
    [OP_ADD] = execute_ADD,
    [OP_SUB] = execute_SUB,
    [OP_MUL] = execute_MUL,
    [OP_DIV] = execute_DIV,
    [OP_MOD] = execute_MOD,
    [OP_ADDR] = execute_ADDR,
    [OP_SUBR] = execute_SUBR,
    [OP_MULR] = execute_MULR,
    [OP_DIVR] = execute_DIVR,
    [OP_MODR] = execute_MODR,
    [OP_JMP] = execute_JMP,
    [OP_JMPZ] = execute_JMPZ,
    [OP_JMPNZ] = execute_JMPNZ,
    [OP_JMPE] = execute_JMPE,
    [OP_HALT] = execute_HALT,
    [OP_DEBUG] = execute_DEBUG,
};

void (*disasm_table[UINT8_MAX + 1])(MeatsVM *vm) = {
    [OP_NOP] = disasm_NOP,
    [OP_DEBUG] = disasm_DEBUG,
    [OP_MOV] = disasm_MOV,
    [OP_MOVI] = disasm_MOVI,
    [OP_MOVE] = disasm_MOVE,
    [OP_MOVER] = disasm_MOVER,
    [OP_ADD] = disasm_ADD,
    [OP_SUB] = disasm_SUB,
    [OP_MUL] = disasm_MUL,
    [OP_DIV] = disasm_DIV,
    [OP_MOD] = disasm_MOD,
    [OP_ADDR] = disasm_ADDR,
    [OP_SUBR] = disasm_SUBR,
    [OP_MULR] = disasm_MULR,
    [OP_DIVR] = disasm_DIVR,
    [OP_MODR] = disasm_MODR,
    [OP_JMP] = disasm_JMP,
    [OP_JMPZ] = disasm_JMPZ,
    [OP_JMPNZ] = disasm_JMPNZ,
    [OP_JMPE] = disasm_JMPE,
    [OP_HALT] = disasm_HALT,
};

size_t get_instr_word_size(const char *instr)
{
	if (strcmp("MOV", instr) == 0)
	{
		return MOV_INSTR_WORD_SIZE;
	}
	else if (strcmp("MOVI", instr) == 0)
	{
		return MOVI_INSTR_WORD_SIZE;
	}
	else if (strcmp("MOVE", instr) == 0)
	{
		return MOVE_INSTR_WORD_SIZE;
	}
	else if (strcmp("MOVER", instr) == 0)
	{
		return MOVER_INSTR_WORD_SIZE;
	}
	else if (strcmp("ADD", instr) == 0)
	{
		return ADD_INSTR_WORD_SIZE;
	}
	else if (strcmp("ADDR", instr) == 0)
	{
		return ADDR_INSTR_WORD_SIZE;
	}
	else if (strcmp("SUB", instr) == 0)
	{
		return SUB_INSTR_WORD_SIZE;
	}
	else if (strcmp("SUBR", instr) == 0)
	{
		return SUBR_INSTR_WORD_SIZE;
	}
	else if (strcmp("MUL", instr) == 0)
	{
		return MUL_INSTR_WORD_SIZE;
	}
	else if (strcmp("MULR", instr) == 0)
	{
		return MULR_INSTR_WORD_SIZE;
	}
	else if (strcmp("DIV", instr) == 0)
	{
		return DIV_INSTR_WORD_SIZE;
	}
	else if (strcmp("DIVR", instr) == 0)
	{
		return DIVR_INSTR_WORD_SIZE;
	}
	else if (strcmp("MOD", instr) == 0)
	{
		return MOD_INSTR_WORD_SIZE;
	}
	else if (strcmp("MODR", instr) == 0)
	{
		return MODR_INSTR_WORD_SIZE;
	}
	else if (strcmp("JMP", instr) == 0)
	{
		return JMP_INSTR_WORD_SIZE;
	}
	else if (strcmp("JMPE", instr) == 0)
	{
		return JMPE_INSTR_WORD_SIZE;
	}
	else if (strcmp("JMPZ", instr) == 0)
	{
		return JMPZ_INSTR_WORD_SIZE;
	}
	else if (strcmp("JMPNZ", instr) == 0)
	{
		return JMPNZ_INSTR_WORD_SIZE;
	}
	else if (strcmp("DEBUG", instr) == 0)
	{
		return DEBUG_INSTR_WORD_SIZE;
	}
	else if (strcmp("HALT", instr) == 0)
	{
		return HALT_INSTR_WORD_SIZE;
	}
	else if (strcmp("NOP", instr) == 0)
	{
		return NOP_INSTR_WORD_SIZE;
	}
	else
		return 0;
}

size_t get_instr_size(const char *instr)
{
	if (strcmp("MOV", instr) == 0)
	{
		return MOV_INSTR_SIZE;
	}
	else if (strcmp("MOVI", instr) == 0)
	{
		return MOVI_INSTR_SIZE;
	}
	else if (strcmp("MOVE", instr) == 0)
	{
		return MOVE_INSTR_SIZE;
	}
	else if (strcmp("MOVER", instr) == 0)
	{
		return MOVER_INSTR_SIZE;
	}
	else if (strcmp("ADD", instr) == 0)
	{
		return ADD_INSTR_SIZE;
	}
	else if (strcmp("ADDR", instr) == 0)
	{
		return ADDR_INSTR_SIZE;
	}
	else if (strcmp("SUB", instr) == 0)
	{
		return SUB_INSTR_SIZE;
	}
	else if (strcmp("SUBR", instr) == 0)
	{
		return SUBR_INSTR_SIZE;
	}
	else if (strcmp("MUL", instr) == 0)
	{
		return MUL_INSTR_SIZE;
	}
	else if (strcmp("MULR", instr) == 0)
	{
		return MULR_INSTR_SIZE;
	}
	else if (strcmp("DIV", instr) == 0)
	{
		return DIV_INSTR_SIZE;
	}
	else if (strcmp("DIVR", instr) == 0)
	{
		return DIVR_INSTR_SIZE;
	}
	else if (strcmp("MOD", instr) == 0)
	{
		return MOD_INSTR_SIZE;
	}
	else if (strcmp("MODR", instr) == 0)
	{
		return MODR_INSTR_SIZE;
	}
	else if (strcmp("JMP", instr) == 0)
	{
		return JMP_INSTR_SIZE;
	}
	else if (strcmp("JMPE", instr) == 0)
	{
		return JMPE_INSTR_SIZE;
	}
	else if (strcmp("JMPZ", instr) == 0)
	{
		return JMPZ_INSTR_SIZE;
	}
	else if (strcmp("JMPNZ", instr) == 0)
	{
		return JMPNZ_INSTR_SIZE;
	}
	else if (strcmp("DEBUG", instr) == 0)
	{
		return DEBUG_INSTR_SIZE;
	}
	else if (strcmp("HALT", instr) == 0)
	{
		return HALT_INSTR_SIZE;
	}
	else if (strcmp("NOP", instr) == 0)
	{
		return NOP_INSTR_SIZE;
	}
	else
		return 0;
}