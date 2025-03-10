#ifndef ASMPARSER_H
#define ASMPARSER_H

#include "meats.h"
#include "vm/bytecode.h"
typedef struct s_ASM_Parser
{
	size_t Position;
	size_t Line;
	Bytecode *Bytecode;
	MeatsArray *Tokens;
} ASM_Parser;

typedef struct s_ASM_Label
{
	char *name;
	size_t position; // Bytecode position of the label
} ASM_Label;

void asm_parser_init(ASM_Parser *asm_parser, MeatsArray *tokens);
void asm_parser_free(ASM_Parser *asm_parser);
void asm_parser_parse(ASM_Parser *asm_parser);
#endif // ASMPARSER_H
