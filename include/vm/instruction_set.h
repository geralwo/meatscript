#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H
#include "vm.h"

#define MAX_OPCODES 256
typedef uint16_t OpCode;
enum OpCode
{
	NOP = 0x00,	 // No operation
	MOV = 0xDA,	 // MOV reg, uint8_t
	MOVI = 0xDB, // MOVI reg, uint32_t
	MOVE = 0xDC, // MOVE reg, uint64_t

	PUSH = 0x01, // PUSH value -> push value to stack
	POP = 0x02,	 // POP reg -> pop value from stack into register

	ADD = 0xAA, // ADD reg, value | reg
	SUB = 0xAB, // ADD reg, value | reg
	MUL = 0xAC, // ADD reg, value | reg
	DIV = 0xAD, // ADD reg, value | reg
	MOD = 0xAE, // ADD reg, value | reg

	ADDF = 0xBA, // ADD reg, value | reg
	SUBF = 0xBB, // ADD reg, value | reg
	MULF = 0xBC, // ADD reg, value | reg
	DIVF = 0xBD, // ADD reg, value | reg
	MODF = 0xBE, // ADD reg, value | reg

	JMP = 0xFA,	  // JMP address
	JMPZ = 0xFB,  // JMPZ dest, address
	JMPGT = 0xFD, // JMPGT dest, address
	JMPLT = 0xFE, // JMPLT dest, address
	JMPE = 0xFF,  // JMPE a, b, address

	FUNC_CALL = 0xFC, // FUNC_CALL address

	HALT = 0xE0,

};

extern void (*disasm_table[MAX_OPCODES])(MeatsVM *vm);
extern void (*execute_table[MAX_OPCODES])(MeatsVM *vm);

// Define function pointers for execution & disassembly
typedef void (*InstrExecFunc)(MeatsVM *vm);
typedef void (*InstrDisasmFunc)(MeatsVM *vm);

// Instruction structure
typedef struct
{
	uint8_t opcode;			// Opcode (e.g., 0xDA for MOV)
	const char *mnemonic;	// Assembly name (e.g., "MOV")
	InstrExecFunc execute;	// Pointer to execution function
	InstrDisasmFunc disasm; // Pointer to disassembly function
} Instruction;

#define OP_NOP 0x00
#define NOP_INSTR_SIZE 1
void execute_NOP();
void disasm_NOP();
uint8_t *bytecode_NOP();

#define OP_HALT 0xE0
#define HALT_INSTR_SIZE 1
void execute_HALT(MeatsVM *vm);
void disasm_HALT();
uint8_t *bytecode_HALT();

#define OP_MOV 0xDA
#define MOV_INSTR_SIZE (1 + 1 + 1)
void execute_MOV(MeatsVM *vm);
void disasm_MOV(MeatsVM *vm);
uint8_t *bytecode_MOV(uint8_t reg, uint8_t value);

#define OP_MOVI 0xDB
#define MOVI_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint32_t))
void execute_MOVI(MeatsVM *vm);
void disasm_MOVI(MeatsVM *vm);
uint8_t *bytecode_MOVI(uint8_t reg, uint32_t value);

#define OP_MOVE 0xDC
#define MOVE_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
void execute_MOVE(MeatsVM *vm);
void disasm_MOVE(MeatsVM *vm);
uint8_t *bytecode_MOVE(uint8_t reg, uint64_t value);

#define OP_ADD 0xAA
#define ADD_INSTR_SIZE (sizeof(OpCode) + 1 + sizeof(uint64_t))
void execute_ADD(MeatsVM *vm);
void disasm_ADD(MeatsVM *vm);
uint8_t *bytecode_ADD(uint8_t reg, uint64_t value);

#define OP_JMP 0xFA
#define JMP_INSTR_SIZE (sizeof(OpCode))
void execute_JMP(MeatsVM *vm);
void disasm_JMP();
uint8_t *bytecode_JMP();

#define OP_JMPE 0xFF
#define JMPE_INSTR_SIZE (sizeof(OpCode) + 1 + 1)
void execute_JMPE(MeatsVM *vm);
void disasm_JMPE(MeatsVM *vm);
uint8_t *bytecode_JMPE(uint8_t reg1, uint8_t reg2);

#endif // INSTRUCTIONSET_H
