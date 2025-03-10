#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "sourcefile.h"
#include "lexer.h"
#include "meats.h"
#include "token.h"
#include "parser.h"
#include "ast_node.h"
#include "vm/bytecode.h"
#include "asm_parser.h"

Token *current_token(Parser *p);
void parser_advance(Parser *p);
AST_Node *parse_asm(Parser *p);
AST_Node *parse_keyword(Parser *p);
AST_Node *parse_number(Parser *p);
AST_Node *parse_identifier(Parser *p);

char *Keywords[] = {
    "printn",
    "print",
    "if",
    "else",
    "while",
    "case",
    "var",
    "const",
    "func",
    "end",
    "null",
    "asm",
};

void parser_init(Parser *parser, Lexer *lexer)
{
	parser->Lexer = lexer;
	parser->position = 0;
	parser->AST = meats_array_new(sizeof(AST_Node));
}
void parser_free(Parser *parser)
{
	lexer_free(parser->Lexer);
	parser->Lexer = NULL;
	meats_array_free(parser->AST);
	parser->AST = NULL;
}

// Parser Utility Functions Start
void parser_advance(Parser *parser)
{
	if (parser->position < parser->Lexer->Tokens->Count)
	{
		// printf("new parser position %ld\n", parser->position);
		parser->position++;
	}
}

Token *current_token(Parser *parser)
{
	return meats_array_get(parser->Lexer->Tokens, parser->position);
}

// Parser Utility Functions End
AST_Node *parse_asm(Parser *parser)
{
	printf("parsing assembly...\n");
	AST_Node *node = new_ast_node(AST_INLINE_ASM);
	MeatsArray *asm_tokens = meats_array_new(sizeof(Token));
	while (current_token(parser)->Type != TOKEN_LBRACE)
	{
		parser_advance(parser);
	};
	parser_advance(parser);
	while (current_token(parser)->Type != TOKEN_RBRACE)
	{
		if (current_token(parser)->Type == TOKEN_EOF)
		{
			printf("Error: Unexpected EOF while parsing asm block\n");
			return NULL;
		}
		meats_array_add(asm_tokens, current_token(parser));
		parser_advance(parser);
	}
	parser_advance(parser);
	// printf("parsed asm block with %ld tokens... ", asm_tokens->Count);
	ASM_Parser asm_parser;
	asm_parser_init(&asm_parser, asm_tokens);
	asm_parser_parse(&asm_parser);
	// printf("asm parser produced '%ld' bytes:\n", asm_parser.Bytecode->size);
	// print_bytes(asm_parser.Bytecode->bytes, asm_parser.Bytecode->size);
	bytecode_append(node->Bytecode, asm_parser.Bytecode->bytes, asm_parser.Bytecode->size);
	// asm_parser_free(&asm_parser);
	return node;
}

AST_Node *parse_keyword(Parser *parser)
{
	printf("parsing keyword\n");
	Token *token = current_token(parser);
	if (strcmp("asm", token->Value) == 0)
	{
		return parse_asm(parser);
	}
	return NULL;
}

AST_Node *parse_number(Parser *parser)
{
	TODO("Implement parsing of numbers");
	(void)parser;
	return NULL;
}

AST_Node *parse_identifier(Parser *parser)
{
	if (list_has_string(current_token(parser)->Value, Keywords) == 1)
	{
		return parse_keyword(parser);
	}
	return NULL;
}

AST_Node *parse_statement(Parser *parser)
{
	printf("parsing statement: '%s'...\n", current_token(parser)->Value);
	AST_Node *left = NULL;
	switch (current_token(parser)->Type)
	{
	case TOKEN_NUMBER:
		left = parse_number(parser);
		break;
	case TOKEN_IDENTIFIER:
		left = parse_identifier(parser);
		break;
	default:
		printf("Error: Unhandled token '%s' [ %s ]\n", current_token(parser)->Value, tokenType_name(current_token(parser)->Type));
		return NULL;
	}
	return left;
}

void parser_parse(Parser *parser)
{
	// printf("parsing token '%s'\n", current_token(parser)->Value);
	while (current_token(parser)->Type != TOKEN_EOF)
	{
		if (current_token(parser)->Type == TOKEN_EOL)
		{
			parser_advance(parser);
			continue;
		}
		AST_Node *statement = parse_statement(parser);
		if (statement == NULL)
		{
			printf("Error parsing statement!\n");
			break;
		}
		meats_array_add(parser->AST, statement);
		// printf("added node '%p' to ast\n", (void *)statement);
	}
	// printf(":: Parsing complete\n");
}
