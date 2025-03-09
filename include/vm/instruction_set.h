#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H
#include "vm.h"

typedef uint8_t OpCode;

#define OP_NOP 0x00
#define OP_HALT 0xE0
#define OP_MOV 0xDA
#define OP_MOVI 0xDB
#define OP_MOVE 0xDC
#define OP_ADD 0xAA
#define OP_JMP 0xFA
#define OP_JMPE 0xFF

extern void (*disasm_table[UINT8_MAX + 1])(MeatsVM *vm);
extern void (*execute_table[UINT8_MAX + 1])(MeatsVM *vm);

#define NOP_INSTR_SIZE 1
void execute_NOP(MeatsVM *vm);
void disasm_NOP();
uint8_t *bytecode_NOP();

#define HALT_INSTR_SIZE 1
void execute_HALT(MeatsVM *vm);
void disasm_HALT();
uint8_t *bytecode_HALT();

#define MOV_INSTR_SIZE (1 + 1 + 1)
void execute_MOV(MeatsVM *vm);
void disasm_MOV(MeatsVM *vm);
uint8_t *bytecode_MOV(uint8_t reg, uint8_t value);

#define MOVI_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint32_t))
void execute_MOVI(MeatsVM *vm);
void disasm_MOVI(MeatsVM *vm);
uint8_t *bytecode_MOVI(uint8_t reg, uint32_t value);

#define MOVE_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
void execute_MOVE(MeatsVM *vm);
void disasm_MOVE(MeatsVM *vm);
uint8_t *bytecode_MOVE(uint8_t reg, uint64_t value);

#define ADD_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
void execute_ADD(MeatsVM *vm);
void disasm_ADD(MeatsVM *vm);
uint8_t *bytecode_ADD(uint8_t reg, uint64_t value);

#define JMP_INSTR_SIZE (sizeof(OpCode))
void execute_JMP(MeatsVM *vm);
void disasm_JMP();
uint8_t *bytecode_JMP();

#define JMPE_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
void execute_JMPE(MeatsVM *vm);
void disasm_JMPE(MeatsVM *vm);
uint8_t *bytecode_JMPE(uint8_t reg1, uint8_t reg2);

#endif // INSTRUCTIONSET_H
