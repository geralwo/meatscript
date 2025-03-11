#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "meats.h"
#include "asm_parser.h"
#include "token.h"
#include "vm/bytecode.h"
#include "vm/instruction_set.h"
#include "stdlib/std_print.h"

#define ASM_LABEL_CAP 1024
ASM_Label asm_labels[ASM_LABEL_CAP] = {0};
size_t asm_label_count = 0; // Track number of labels

void asm_parser_init(ASM_Parser *asm_parser, MeatsArray *asm_tokens)
{
	asm_parser->Position = 0;
	asm_parser->Line = 1;
	asm_parser->Bytecode = new_bytecode();
	asm_parser->Tokens = asm_tokens;
}
void asm_parser_free(ASM_Parser *asm_parser)
{
	if (!asm_parser)
		return; // Avoid freeing NULL pointer

	free_bytecode(asm_parser->Bytecode);
	asm_parser->Bytecode = NULL; // Prevent dangling pointer

	meats_array_free(asm_parser->Tokens);
	asm_parser->Tokens = NULL; // Prevent dangling pointer
}

uint8_t parse_register(const char *reg)
{
	if (reg == NULL || reg[0] != 'r')
	{
		fprintf(stderr, "Error: Invalid register format\n");
		return 0;
	}
	return str_to_uint8(reg + 1); // Skip 'r' and convert the number
}

int is_valid_byte(const char *str, uint8_t *out_value)
{
	char *endptr;
	long value = strtol(str, &endptr, 10);

	// Check if the entire string is consumed and the value is in range
	if (*endptr == '\0' && value >= 0 && value <= UINT8_MAX)
	{
		*out_value = (uint8_t)value; // Store the result
		return 1;		     // Valid
	}
	return 0; // Invalid
}

void asm_parser_advance(ASM_Parser *asm_parser)
{
	if (asm_parser->Position < asm_parser->Tokens->Count)
		asm_parser->Position++;
}

Token *current_asm_token(ASM_Parser *asm_parser)
{
	return meats_array_get(asm_parser->Tokens, asm_parser->Position);
}

Token *eat_asm_token(ASM_Parser *asm_parser)
{
	return meats_array_get(asm_parser->Tokens, asm_parser->Position++);
}

void asm_parser_preprocessor(ASM_Parser *asm_parser)
{
	for (size_t i = 0; i < asm_parser->Tokens->Count; i++)
	{
		Token *t = meats_array_get(asm_parser->Tokens, i);
		if (t->Type == TOKEN_COLON && (i + 1) < asm_parser->Tokens->Count)
		{
			ASM_Label label;
			Token *label_name = meats_array_get(asm_parser->Tokens, i + 1);
			label.name = copy_string(label_name->Value);
			asm_labels[asm_label_count++] = label;
			i++;
		}
	}
}

void asm_parser_finalize(ASM_Parser *asm_parser)
{
	(void)asm_parser;
}

void asm_parser_parse(ASM_Parser *asm_parser)
{
	uint8_t raw_byte;
	// meats_print("ASM PARSER:\n");
	// for (size_t i = 0; i < asm_parser->Tokens->Count; i++)
	// {
	// 	Token *t = meats_array_get(asm_parser->Tokens, i);
	// 	meats_print("asm processing :: %s %s\n", t->Value, tokenType_name(t->Type));
	// }

	asm_parser_preprocessor(asm_parser);

	while (asm_parser->Position < asm_parser->Tokens->Count)
	{
		Token *t = eat_asm_token(asm_parser);
		// printf("current ASM token: '%s' idx: %ld\n", t->Value, i);
		if (strcmp("MOV", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint8_t val = str_to_uint8(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOV(reg, val), MOV_INSTR_SIZE);
		}
		else if (strcmp("MOVI", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint32_t val = str_to_uint32(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOVI(reg, val), MOVI_INSTR_SIZE);
		}
		else if (strcmp("MOVE", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOVE(reg, val), MOVE_INSTR_SIZE);
		}
		else if (strcmp("ADD", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_ADD(reg, val), ADD_INSTR_SIZE);
		}
		else if (strcmp("SUB", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_SUB(reg, val), SUB_INSTR_SIZE);
		}
		else if (strcmp("MUL", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MUL(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("DIV", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_DIV(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("MOD", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOD(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("JMPE", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *reg1t = eat_asm_token(asm_parser);
			Token *reg2t = eat_asm_token(asm_parser);
			Token *addrt = eat_asm_token(asm_parser);
			uint8_t reg1 = parse_register(reg1t->Value);
			uint8_t reg2 = parse_register(reg2t->Value);
			uint64_t addr = str_to_uint64(addrt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_JMPE(reg1, reg2, addr), JMPE_INSTR_SIZE);
		}
		else if (strcmp("JMP", t->Value) == 0)
		{
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr;
			if (strcmp(addrt->Value, ":") == 0)
			{
				eat_asm_token(asm_parser);
				addr = 0;
				bytecode_append(asm_parser->Bytecode, bytecode_JMP(addr), JMP_INSTR_SIZE);
			}
			else
			{
				addr = str_to_uint64(addrt->Value);
				bytecode_append(asm_parser->Bytecode, bytecode_JMP(addr), JMP_INSTR_SIZE);
			}
		}
		else if (strcmp("JMPZ", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr = str_to_uint64(addrt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_JMPZ(parse_register(regt->Value), addr), JMPZ_INSTR_SIZE);
		}
		else if (strcmp("JMPNZ", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr = str_to_uint64(addrt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_JMPNZ(parse_register(regt->Value), addr), JMPNZ_INSTR_SIZE);
		}
		else if (strcmp("DEBUG", t->Value) == 0)
		{
			Token *flagt = eat_asm_token(asm_parser);
			uint64_t flags = str_to_uint64(flagt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_DEBUG(flags), DEBUG_INSTR_SIZE);
		}
		else if (strcmp("HALT", t->Value) == 0)
		{
			bytecode_append(asm_parser->Bytecode, bytecode_HALT(), HALT_INSTR_SIZE);
		}
		else if (strcmp("NOP", t->Value) == 0)
		{
			bytecode_append(asm_parser->Bytecode, bytecode_NOP(), NOP_INSTR_SIZE);
		}
		else if (strcmp("PUSH", t->Value) == 0)
		{
			Token *valt = eat_asm_token(asm_parser);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_PUSH(val), PUSH_INSTR_SIZE);
		}
		else if (strcmp("POP", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_POP(reg), POP_INSTR_SIZE);
		}
		else if (strcmp(":", t->Value) == 0)
		{
			eat_asm_token(asm_parser);
		}
		else if (strcmp(";", t->Value) == 0)
		{
			Token *current_token = eat_asm_token(asm_parser);
			while (current_token->Type != TOKEN_EOL)
			{
				if (current_token->Type == TOKEN_EOF)
				{
					printf("Unexpected EOF Token while parsing Comment!\n");
					exit(1);
				}
				current_token = eat_asm_token(asm_parser);
			}
		}
		else if (is_valid_byte(t->Value, &raw_byte))
		{
			bytecode_append(asm_parser->Bytecode, &raw_byte, 1);
		}
		else if (strcmp("EOL", t->Value) == 0)
		{
			continue;
		}
		else if (strcmp("EOF", t->Value) == 0)
		{
			return;
		}
		else
		{
			printf("ERROR ASM: Unhandled Token '%s'!\n", t->Value);
			exit(1);
		}
	}
	asm_parser_finalize(asm_parser);
}
