#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast_node.h"
#include "asm_parser.h"

typedef struct
{
	size_t position;
	Lexer *Lexer;
	MeatsArray *AST;
	MeatsArray *Labels;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);
void parser_free(Parser *parser);
void parser_parse(Parser *parser);
AST_Node *parse_statement(Parser *parser);
// AST_Node* parse_identifier(Parser* parser);
AST_Node *parse_var_decl(Parser *parser);
AST_Node *parse_const_decl(Parser *parser);
#endif // PARSER_H
