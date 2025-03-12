#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H
#include "vm.h"

typedef uint8_t OpCode;

// A = Arithmetic
// B =
// C =
// D = Moving Data
// E = Exiting, starting, running
// F = Jumping, Functions

#define OP_NOP 0x00
#define OP_HALT 0xE0
#define OP_DEBUG 0xED

#define OP_MOV 0xDA
#define OP_MOVI 0xDB
#define OP_MOVE 0xDC

#define OP_ADD 0xA0
#define OP_ADDR 0xA1
#define OP_SUB 0xA2
#define OP_SUBR 0xA3
#define OP_MUL 0xA4
#define OP_MULR 0xA5
#define OP_DIV 0xA6
#define OP_DIVR 0xA7
#define OP_MOD 0xA8
#define OP_MODR 0xA9

#define OP_JMP 0xFA
#define OP_JMPE 0xFF
#define OP_JMPZ 0xF0
#define OP_JMPNZ 0xF1

#define OP_PUSH 0xC0
#define OP_POP 0xC1

extern void (*disasm_table[UINT8_MAX + 1])(MeatsVM *vm);
extern void (*execute_table[UINT8_MAX + 1])(MeatsVM *vm);

size_t get_instr_size(const char *instr);
size_t get_instr_word_size(const char *instr);

#define NOP_INSTR_SIZE sizeof(OpCode)
#define NOP_INSTR_WORD_SIZE 1
void execute_NOP(MeatsVM *vm);
void disasm_NOP();
uint8_t *bytecode_NOP();

#define DEBUG_INSTR_SIZE (sizeof(OpCode) + sizeof(uint64_t))
#define DEBUG_INSTR_WORD_SIZE 2
void execute_DEBUG(MeatsVM *vm);
void disasm_DEBUG(MeatsVM *vm);
uint8_t *bytecode_DEBUG(uint64_t debug_flags);

#define HALT_INSTR_SIZE sizeof(OpCode)
#define HALT_INSTR_WORD_SIZE 1
void execute_HALT(MeatsVM *vm);
void disasm_HALT();
uint8_t *bytecode_HALT();

#define MOV_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
void execute_MOV(MeatsVM *vm);
void disasm_MOV(MeatsVM *vm);
uint8_t *bytecode_MOV(uint8_t reg, uint8_t value);
#define MOV_INSTR_WORD_SIZE 3

#define MOVI_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint32_t))
#define MOVI_INSTR_WORD_SIZE 3
void execute_MOVI(MeatsVM *vm);
void disasm_MOVI(MeatsVM *vm);
uint8_t *bytecode_MOVI(uint8_t reg, uint32_t value);

#define MOVE_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define MOVE_INSTR_WORD_SIZE 3
void execute_MOVE(MeatsVM *vm);
void disasm_MOVE(MeatsVM *vm);
uint8_t *bytecode_MOVE(uint8_t reg, uint64_t value);

#define ADD_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define ADD_INSTR_WORD_SIZE 3
void execute_ADD(MeatsVM *vm);
void disasm_ADD(MeatsVM *vm);
uint8_t *bytecode_ADD(uint8_t reg, uint64_t value);

#define ADDR_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
#define ADDR_INSTR_WORD_SIZE 3
void execute_ADDR(MeatsVM *vm);
void disasm_ADDR(MeatsVM *vm);
uint8_t *bytecode_ADDR(uint8_t reg, uint8_t reg2);

#define SUB_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define SUB_INSTR_WORD_SIZE 3
void execute_SUB(MeatsVM *vm);
void disasm_SUB(MeatsVM *vm);
uint8_t *bytecode_SUB(uint8_t reg, uint64_t value);

#define SUBR_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
#define SUBR_INSTR_WORD_SIZE 3
void execute_SUBR(MeatsVM *vm);
void disasm_SUBR(MeatsVM *vm);
uint8_t *bytecode_SUBR(uint8_t reg, uint8_t reg2);

#define MUL_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define MUL_INSTR_WORD_SIZE 3
void execute_MUL(MeatsVM *vm);
void disasm_MUL(MeatsVM *vm);
uint8_t *bytecode_MUL(uint8_t reg, uint64_t value);

#define MULR_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
#define MULR_INSTR_WORD_SIZE 3
void execute_MULR(MeatsVM *vm);
void disasm_MULR(MeatsVM *vm);
uint8_t *bytecode_MULR(uint8_t reg, uint8_t reg2);

#define DIV_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define DIV_INSTR_WORD_SIZE 3
void execute_DIV(MeatsVM *vm);
void disasm_DIV(MeatsVM *vm);
uint8_t *bytecode_DIV(uint8_t reg, uint64_t value);

#define DIVR_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
#define DIVR_INSTR_WORD_SIZE 3
void execute_DIVR(MeatsVM *vm);
void disasm_DIVR(MeatsVM *vm);
uint8_t *bytecode_DIVR(uint8_t reg, uint8_t reg2);

#define MOD_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define MOD_INSTR_WORD_SIZE 3
void execute_MOD(MeatsVM *vm);
void disasm_MOD(MeatsVM *vm);
uint8_t *bytecode_MOD(uint8_t reg, uint64_t value);

#define MODR_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
#define MODR_INSTR_WORD_SIZE 3
void execute_MODR(MeatsVM *vm);
void disasm_MODR(MeatsVM *vm);
uint8_t *bytecode_MODR(uint8_t reg, uint8_t reg2);

#define JMP_INSTR_SIZE (sizeof(OpCode) + sizeof(uint64_t))
#define JMP_INSTR_WORD_SIZE 2
void execute_JMP(MeatsVM *vm);
void disasm_JMP(MeatsVM *vm);
uint8_t *bytecode_JMP(uint64_t address);

#define JMPZ_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define JMPZ_INSTR_WORD_SIZE 3
void execute_JMPZ(MeatsVM *vm);
void disasm_JMPZ(MeatsVM *vm);
uint8_t *bytecode_JMPZ(uint8_t reg, uint64_t address);

#define JMPNZ_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
#define JMPNZ_INSTR_WORD_SIZE 3
void execute_JMPNZ(MeatsVM *vm);
void disasm_JMPNZ(MeatsVM *vm);
uint8_t *bytecode_JMPNZ(uint8_t reg, uint64_t address);

#define JMPE_INSTR_SIZE (sizeof(OpCode) + 1 + 1 + +sizeof(uint64_t))
#define JMPE_INSTR_WORD_SIZE 4
void execute_JMPE(MeatsVM *vm);
void disasm_JMPE(MeatsVM *vm);
uint8_t *bytecode_JMPE(uint8_t reg1, uint8_t reg2, uint64_t address);

#define PUSH_INSTR_SIZE (sizeof(OpCode) + sizeof(uint64_t))
#define PUSH_INSTR_WORD_SIZE 2
void execute_PUSH(MeatsVM *vm);
void disasm_PUSH(MeatsVM *vm);
uint8_t *bytecode_PUSH(uint64_t value);

#define POP_INSTR_SIZE (sizeof(OpCode) + sizeof(uint8_t))
#define POP_INSTR_WORD_SIZE 2
void execute_POP(MeatsVM *vm);
void disasm_POP(MeatsVM *vm);
uint8_t *bytecode_POP(uint8_t reg);

#endif // INSTRUCTIONSET_H
