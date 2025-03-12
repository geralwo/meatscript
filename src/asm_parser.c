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
	if (asm_parser->Position < asm_parser->Tokens->Count)
		asm_parser->Position++;
	return meats_array_get(asm_parser->Tokens, asm_parser->Position);
}

void asm_parser_preprocessor(ASM_Parser *asm_parser)
{
	// printf(":: starting asm preproccessor\n");
	size_t bytecode_position = 0;

	for (size_t i = 0; i < asm_parser->Tokens->Count; i++)
	{
		Token *t = meats_array_get(asm_parser->Tokens, i);
		size_t word_size = get_instr_word_size(t->Value);

		// label declarations should only be found directly after new lines
		if (t->Type == TOKEN_SYMBOL)
		{
			ASM_Label label;
			label.name = copy_string(t->Value);
			label.position = bytecode_position;
			asm_labels[asm_label_count++] = label;
			printf("Found Label at %ld: '%s' -> %ld\n", t->line, label.name, label.position);
		}
		// if current token is ; skip all tokens up to eol
		else if (t->Type == TOKEN_SEMICOLON)
		{
			while (i < asm_parser->Tokens->Count)
			{
				Token *comment = meats_array_get(asm_parser->Tokens, i);
				if (comment->Type == TOKEN_EOL)
					break;
				i++;
			}
		}
		// else we found an assembly keyword and update the bytecode position or index we use to jump around with
		// this means the current token is for example 'JMPE'
		// a line could look like this: 'JMPE r0 r1 13'
		// or                         : 'JMPE r0 r1 :MAIN'
		// we are here:                  ^
		// an instruction has a word size which counts the whole instruction. for JMPE its 4
		// so to increment everything correctly, we need to find out how many words we can skip
		// to get the next potentially relevant token.
		// for JMPE:
		// skip (JMPE_WORD_SIZE - 1) words
		// increment bytecode position by JMPE_INSTR_SIZE
		// but not all instruction can work with labels.
		// all labels are always the last argument.
		else if (t->Type == TOKEN_IDENTIFIER)
		{
			if (word_size > 0)
				word_size -= 1;
		}
		bytecode_position += get_instr_size(t->Value);
		printf("Skipping %ld words for '%s' on line %ld | bytecode pos: %ld\n", word_size, t->Value, t->line, bytecode_position);
		i += word_size;
	}

	printf(":: finished asm preproccessor with %ld labels\n", asm_label_count);
	for (size_t i = 0; i < asm_label_count; i++)
	{
		printf("Label: %s = %ld\n", asm_labels[i].name, asm_labels[i].position);
	}
	printf(":::::::\n");
}

size_t get_addr_from_label(const char *name)
{
	for (size_t i = 0; i < asm_label_count; i++)
	{
		if (strcmp(name, asm_labels[i].name) == 0)
			return asm_labels[i].position;
	}
	return 0;
}

void asm_parser_parse(ASM_Parser *asm_parser)
{
	uint8_t raw_byte;

	asm_parser_preprocessor(asm_parser);

	while (current_asm_token(asm_parser)->Type != TOKEN_EOF)
	{
		Token *t = eat_asm_token(asm_parser);
		printf("current ASM token: '%s' [%s]idx: %ld\n", t->Value, tokenType_name(t->Type), asm_parser->Position);
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
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MUL(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("MULR", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *reg2t = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint8_t reg2 = parse_register(reg2t->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MULR(reg, reg2), MULR_INSTR_SIZE);
		}
		else if (strcmp("DIV", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_DIV(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("MOD", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			Token *valt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			uint64_t val = str_to_uint64(valt->Value);
			bytecode_append(asm_parser->Bytecode, bytecode_MOD(reg, val), MUL_INSTR_SIZE);
		}
		else if (strcmp("JMP", t->Value) == 0)
		{
			if (asm_parser->Position + 1 >= asm_parser->Tokens->Count)
				break;
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr;
			if (addrt->Type == TOKEN_SYMBOL)
			{
				addr = get_addr_from_label(addrt->Value);
			}
			else
			{
				addr = str_to_uint64(addrt->Value);
			}
			bytecode_append(asm_parser->Bytecode, bytecode_JMP(addr), JMP_INSTR_SIZE);
		}
		else if (strcmp("JMPE", t->Value) == 0)
		{
			Token *reg1t = eat_asm_token(asm_parser);
			Token *reg2t = eat_asm_token(asm_parser);
			Token *addrt = eat_asm_token(asm_parser);
			uint8_t reg1 = parse_register(reg1t->Value);
			uint8_t reg2 = parse_register(reg2t->Value);
			uint64_t addr;
			if (addrt->Type == TOKEN_SYMBOL)
			{
				// printf(":: getting label value '%s': %ld", addrt->Value, get_addr_from_label(addrt->Value));
				addr = get_addr_from_label(addrt->Value);
			}
			else
			{
				addr = str_to_uint64(addrt->Value);
			}
			bytecode_append(asm_parser->Bytecode, bytecode_JMPE(reg1, reg2, addr), JMPE_INSTR_SIZE);
		}
		else if (strcmp("JMPZ", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr;
			if (addrt->Type == TOKEN_SYMBOL)
			{
				// printf(":: getting label value '%s': %ld", addrt->Value, get_addr_from_label(addrt->Value));
				addr = get_addr_from_label(addrt->Value);
			}
			else
			{
				addr = str_to_uint64(addrt->Value);
			}
			bytecode_append(asm_parser->Bytecode, bytecode_JMPZ(reg, addr), JMPZ_INSTR_SIZE);
		}
		else if (strcmp("JMPNZ", t->Value) == 0)
		{
			Token *regt = eat_asm_token(asm_parser);
			uint8_t reg = parse_register(regt->Value);
			Token *addrt = eat_asm_token(asm_parser);
			uint64_t addr;
			if (addrt->Type == TOKEN_SYMBOL)
			{
				// printf(":: getting label value '%s': %ld", addrt->Value, get_addr_from_label(addrt->Value));
				addr = get_addr_from_label(addrt->Value);
			}
			else
			{
				addr = str_to_uint64(addrt->Value);
			}
			bytecode_append(asm_parser->Bytecode, bytecode_JMPNZ(reg, addr), JMPNZ_INSTR_SIZE);
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
		else if (strcmp(":", t->Value) == 0)
		{
			// printf("ignoring label\n");
			eat_asm_token(asm_parser);
		}
		else if (t->Type == TOKEN_SYMBOL)
		{
			// printf("ignoring label\n");
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
			printf("ERROR ASM: Unhandled Token '%s' on line %ld!\n", t->Value, t->line);
			exit(1);
		}
	}
}
