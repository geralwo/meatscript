#include "vm/instruction_set.h"
#include "util.h"

void (*execute_table[UINT8_MAX + 1])(MeatsVM *vm) = {
    [OP_NOP] = execute_NOP,
    [OP_MOV] = execute_MOV,
    [OP_MOVI] = execute_MOVI,
    [OP_MOVE] = execute_MOVE,
    [OP_ADD] = execute_ADD,
    [OP_SUB] = execute_SUB,
    [OP_MUL] = execute_MUL,
    [OP_DIV] = execute_DIV,
    [OP_MOD] = execute_MOD,
    [OP_JMP] = execute_JMP,
    [OP_JMPZ] = execute_JMPZ,
    [OP_JMPNZ] = execute_JMPNZ,
    [OP_JMPE] = execute_JMPE,
    [OP_HALT] = execute_HALT,
    [OP_PUSH] = execute_PUSH,
    [OP_POP] = execute_POP,
};

void (*disasm_table[UINT8_MAX + 1])(MeatsVM *vm) = {
    [OP_NOP] = disasm_NOP,
    [OP_MOV] = disasm_MOV,
    [OP_MOVI] = disasm_MOVI,
    [OP_MOVE] = disasm_MOVE,
    [OP_ADD] = disasm_ADD,
    [OP_SUB] = disasm_SUB,
    [OP_MUL] = disasm_MUL,
    [OP_DIV] = disasm_DIV,
    [OP_MOD] = disasm_MOD,
    [OP_JMP] = disasm_JMP,
    [OP_JMPZ] = disasm_JMPZ,
    [OP_JMPNZ] = disasm_JMPNZ,
    [OP_JMPE] = disasm_JMPE,
    [OP_HALT] = disasm_HALT,
    [OP_PUSH] = disasm_PUSH,
    [OP_POP] = disasm_POP,
};