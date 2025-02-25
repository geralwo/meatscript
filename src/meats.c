#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sourcefile.h"
#include "lexer.h"
#include "vm.h"
#include "vm/bytecode.h"
#include "vm/instruction_set.h"
#include "util.h"
#include "asm_parser.h"

int main(int argc, char *argv[])
{
	MeatsVM vm;

	if (argc < 2)
	{
		printf("mobile, easy and typed script compiler\n");
		printf("Usage: %s file.meats\n", argv[0]);
		return 1;
	}
	if (strcmp("repl", argv[1]) == 0)
	{
		Bytecode repl_init;
		bytecode_init(&repl_init);

		// bytecode_append(&repl_init, bytecode_MOV(31, 0), MOV_INSTR_SIZE);
		// bytecode_append(&repl_init, bytecode_HALT(), HALT_INSTR_SIZE);
		char *repl_input = read_line(EOF);
		Lexer repl_lexer;
		lexer_init(&repl_lexer, repl_input, strlen(repl_input));
		// printf("%s\n", repl_input);
		lexer_tokenize(&repl_lexer);
		ASM_Parser asm_parser;
		asm_parser_init(&asm_parser, repl_lexer.Tokens);
		asm_parser_parse(&asm_parser);
		bytecode_append(&repl_init, asm_parser.Bytecode->bytes, asm_parser.Bytecode->size);
		meats_vm_init(&vm);
		vm.ProgramLength = repl_init.size;
		vm.Program = repl_init.bytes;
		meats_vm_dump_bytecode(&vm);
		meats_vm_run(&vm);
		meats_vm_dump_registers(&vm);
		// meats_vm_dump_bytecode(&vm);
		meats_vm_print_asm(&vm);
	}
	else
	{
		TODO("implement parsing from file\n");
		SourceFile *source_file = read_file(argv[1]);
		printf("content of '%s':\n----\n%s\n----\n", source_file->file_name, source_file->source_code);
	}

	return 0;
}
