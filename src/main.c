#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sourcefile.h"
#include "lexer.h"
#include "vm.h"
#include "vm/bytecode.h"
#include "vm/instruction_set.h"
#include "util.h"
#include "parser.h"
#include "ast_node.h"
#include "asm_parser.h"
#include "stdlib/std_array.h"
#include "stdlib/std_malloc.h"

int main(int argc, char *argv[])
{

	int dont_run_vm = 0;
	int verbose = 0;
	MeatsVM vm;
	_meat_mem_init();
	meats_vm_init(&vm);
	vm.HeapSize = MEAT_MEM_POOL.Size;
	vm.Heap = MEAT_MEM_POOL.Data;
	// meats_vm_print_stats(&vm);
	char *binary_file_name = NULL;
	if (argc < 2)
	{
		printf("mobile, easy and typed script compiler\n");
		printf("Usage: %s [options] <file>\n", argv[0]);
		printf("Options:\n");
		printf("-o <file>:\twrites bytecode to file\n");
		printf("repl_asm\tstart a repl in asm mode (file arg is not needed)\n");
		return 1;
	}
	else if (argc > 2)
	{
		for (int i = 0; i < argc; i++)
		{
			if (strcmp("-o", argv[i]) == 0)
			{
				if ((i + 1) >= argc)
					return 1;
				binary_file_name = argv[i + 1];
			}
			else if (strcmp("-disasm", argv[i]) == 0)
			{
				Bytecode program;
				bytecode_init(&program);

				SourceFile *source_code = read_file(argv[1]);
				Lexer lexer;
				lexer_init(&lexer, source_code->source_code, source_code->file_size);
				lexer_tokenize(&lexer);
				Parser parser;
				parser_init(&parser, &lexer);
				parser_parse(&parser);
				for (size_t i = 0; i < parser.AST->Count; i++)
				{
					AST_Node *node = meats_array_get(parser.AST, i);
					if (node->Bytecode)
						bytecode_append(&program, node->Bytecode->bytes, node->Bytecode->size);
					else
						exit(13);
				}
				vm.Program = program.bytes;
				vm.ProgramLength = program.size;
				print_bytes(vm.Program, vm.ProgramLength);
				meats_vm_print_asm(&vm);
				return 0;
			}
			else if (strcmp("-dont-run-vm", argv[i]) == 0)
			{
				dont_run_vm = 1;
			}
			else if (strcmp("-v", argv[i]) == 0)
			{
				verbose = 1;
			}
		}
	}
	if (strcmp("repl_asm", argv[1]) == 0)
	{
		Bytecode repl_init;
		bytecode_init(&repl_init);
		MeatsArray *repl_labels = meats_array_new(sizeof(ASM_Label));
		// bytecode_append(&repl_init, bytecode_MOV(30, 0), MOV_INSTR_SIZE);
		// bytecode_append(&repl_init, bytecode_HALT(), HALT_INSTR_SIZE);
		while (1)
		{
			printf(">");
			char *repl_input = read_line(EOF);
			Lexer repl_lexer;
			lexer_init(&repl_lexer, repl_input, strlen(repl_input));
			// printf("%s\n", repl_input);
			lexer_tokenize(&repl_lexer);
			ASM_Parser asm_parser;
			asm_parser_init(&asm_parser, repl_lexer.Tokens, repl_labels);
			asm_parser_parse(&asm_parser);
			bytecode_append(&repl_init, asm_parser.Bytecode->bytes, asm_parser.Bytecode->size);

			vm.ProgramLength = repl_init.size;
			vm.Program = repl_init.bytes;
			// meats_vm_dump_bytecode(&vm);
			if (dont_run_vm == 0)
				meats_vm_run(&vm);
		}
		return vm.Registers[30];
	}
	Bytecode program;
	bytecode_init(&program);

	SourceFile *source_code = read_file(argv[1]);
	if (verbose == 1)
	{
		printf("Source Code '%s':\n------\n%s\n------\n",
		       source_code->file_name,
		       source_code->source_code);
	}
	Lexer lexer;
	lexer_init(&lexer, source_code->source_code, source_code->file_size);
	lexer_tokenize(&lexer);
	if (verbose == 1)
	{
		printf("Token count: %ld\n", lexer.Tokens->Count);
		for (size_t i = 0; i < lexer.Tokens->Count; i++)
		{
			Token *t = meats_array_get(lexer.Tokens, i);
			printf("Token: %s %s\n", t->Value, tokenType_name(t->Type));
		}
	}

	Parser parser;
	parser_init(&parser, &lexer);
	parser_parse(&parser);
	for (size_t i = 0; i < parser.AST->Count; i++)
	{
		AST_Node *node = meats_array_get(parser.AST, i);
		if (node->Bytecode)
			bytecode_append(&program, node->Bytecode->bytes, node->Bytecode->size);
	}
	vm.Program = program.bytes;
	vm.ProgramLength = program.size;
	if (verbose == 1)
	{
		printf("Parser AST Node count: %ld\n", parser.AST->Count);
		for (size_t i = 0; i < parser.AST->Count; i++)
		{
			AST_Node *t = meats_array_get(parser.AST, i);
			printf("%s\n", astNodeType_name(t->Type));
		}
		printf("Parser AST Label count: %ld\n", parser.Labels->Count);
		for (size_t i = 0; i < parser.Labels->Count; i++)
		{
			ASM_Label *t = meats_array_get(parser.Labels, i);
			printf("%s->%lu\n", t->name, t->position);
		}
		printf("Program Bytecode (len: %ld):\n", program.size);
		print_bytes(program.bytes, program.size);
	}
	if (binary_file_name != NULL)
	{
		if (verbose == 1)
			printf("dumping bytecode file to '%s'\n", binary_file_name);
		write_bytecode_to_file(binary_file_name, vm.Program, vm.ProgramLength);
	}
	if (dont_run_vm == 0)
		meats_vm_run(&vm);
	return vm.Registers[31];
}
