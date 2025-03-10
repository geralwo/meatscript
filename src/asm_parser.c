#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "meats.h"
#include "asm_parser.h"
#include "token.h"
#include "vm/bytecode.h"
#include "vm/instruction_set.h"

void asm_parser_process_labels(ASM_Parser *asm_parser);

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

	asm_parser = NULL;
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
#define ASM_LABEL_CAP 1024
ASM_Label asm_labels[ASM_LABEL_CAP];
size_t asm_label_count = 0; // Track number of labels

void asm_parser_resolve_label(ASM_Parser *asm_parser)
{
	Token *label_name = meats_array_get(asm_parser->Tokens, ++asm_parser->Position);
	int label_defined = -1;
	for (size_t i = 0; i < ASM_LABEL_CAP; i++)
	{
		if (strcmp(asm_labels[i].name, label_name->Value) == 0)
		{
			TODO("Get the label position of %s", asm_labels[i].name);
			label_defined = 1;
		}
	}
	if (label_defined == -1)
	{
		ASM_Label label;
		label.position = asm_parser->Bytecode->size;
		label.name = label_name->Value;
		asm_labels[asm_label_count++] = label;
	}
	else if (label_defined == 1)
	{
		return;
	}
}

void asm_parser_parse(ASM_Parser *asm_parser)
{
	uint8_t raw_byte;

	while (asm_parser->Position < asm_parser->Tokens->Count)
	{
		Token *t = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
		// printf("current ASM token: '%s' idx: %ld\n", t->Value, i);
		if (strcmp("MOV", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			Token *valt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg = parse_register(regt->Value);
			uint8_t val = str_to_uint8(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOV(reg, val), MOV_INSTR_SIZE);
		}
		else if (strcmp("MOVI", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			Token *valt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg = parse_register(regt->Value);
			uint32_t val = str_to_uint32(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOVI(reg, val), MOVI_INSTR_SIZE);
		}
		else if (strcmp("MOVE", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			Token *valt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOVE(reg, val), MOVE_INSTR_SIZE);
		}
		else if (strcmp("ADD", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *regt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			Token *valt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_ADD(reg, val), ADD_INSTR_SIZE);
		}
		else if (strcmp("JMPE", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *reg1t = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			Token *reg2t = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg1 = parse_register(reg1t->Value);
			uint8_t reg2 = parse_register(reg2t->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_JMPE(reg1, reg2), JMPE_INSTR_SIZE);
		}
		else if (strcmp("JMP", t->Value) == 0)
		{
			// Token *regT = meats_array_get(asm_parser->Tokens, i++);
			// uint8_t reg = str_to_uint8(regT->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_JMP(7), JMP_INSTR_SIZE);
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
			Token *valt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_PUSH(val), PUSH_INSTR_SIZE);
		}
		else if (strcmp("POP", t->Value) == 0)
		{
			Token *regt = meats_array_get(asm_parser->Tokens, asm_parser->Position++);
			uint8_t reg = parse_register(regt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_POP(reg), POP_INSTR_SIZE);
		}
		else if (strcmp("EOL", t->Value) == 0)
		{
			continue;
		}
		else if (strcmp("EOF", t->Value) == 0)
		{
			return;
		}
		else if (strcmp(":", t->Value) == 0)
		{

			asm_parser_resolve_label(asm_parser);
		}
		else if (is_valid_byte(t->Value, &raw_byte))
		{
			bytecode_append(asm_parser->Bytecode, &raw_byte, 1);
		}
		else
		{
			printf("ERROR ASM: Unhandled Token '%s'!\n", t->Value);
			exit(1);
		}
	}
}
