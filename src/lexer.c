#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "sourcefile.h"
#include "lexer.h"

void lexer_init(Lexer *lexer, char *source_code, size_t length)
{
	lexer->source = source_code;
	lexer->length = length;
	lexer->position = 0;
	lexer->line = 0;
	lexer->column = 0;
	lexer->Tokens = meats_array_new(sizeof(Token));
	// printf("Lexer initialized\n");
}

void lexer_advance(Lexer *lexer)
{
	if (lexer->source[lexer->position] == '\n')
	{
		lexer->line++;
		lexer->column = 0;
	}
	else
	{
		lexer->column++;
	}
	lexer->position++;
}

char lexer_peek(Lexer *lexer)
{
	if (lexer->position + 1 < lexer->length)
		return lexer->source[lexer->position + 1];
	else
		return lexer->source[lexer->position];
}

char current_char(Lexer *lexer)
{
	return lexer->source[lexer->position];
}

void lexer_tokenize(Lexer *lexer)
{
	while (current_char(lexer) != '\0')
	{
		Token token;
		char c = current_char(lexer);
		// printf(":: lexing character '%u'\n",c);
		if (isdigit(c))
		{
			token = new_token(TOKEN_NUMBER);
			int start = lexer->position;
			while (isdigit(c) || c == '.' || c == '_' || c == 'x')
			{
				lexer_advance(lexer);
				c = lexer->source[lexer->position];
			}
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			token.line = lexer->line;
			token.column = lexer->column;
			// printf("lexed digit '%s' at index: %d\n",token.Value,start);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (isalpha(c))
		{
			int start = lexer->position;
			while (isalnum(c) || c == '_')
			{
				lexer_advance(lexer);
				c = lexer->source[lexer->position];
			}
			token = new_token(TOKEN_IDENTIFIER);
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			token.line = lexer->line;
			token.column = lexer->column;
			meats_array_add(lexer->Tokens, &token);
			// printf("lexed identifier '%s' at index: %d\n",token.Value,start);
			// lexer_advance(lexer);
		}
		else if (c == '\n')
		{
			lexer->line++;
			lexer->column = 0;
			token = new_token(TOKEN_EOL);
			token.Value = "EOL";
			token.line = lexer->line;
			token.column = lexer->column;
			meats_array_add(lexer->Tokens, &token);
			lexer_advance(lexer);
		}
		else if (c == '\r')
		{
			lexer_advance(lexer);
		}
		else if (c == '\'' || c == '"')
		{
			token = new_token(TOKEN_STRING);
			char quote = c;
			int start = lexer->position;
			lexer_advance(lexer); // eat starting quote
			while (current_char(lexer) != quote)
			{
				lexer_advance(lexer); // eat everything between quotes
			}
			lexer_advance(lexer); // eat closing quote
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			token.line = lexer->line;
			token.column = lexer->column;
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == ':')
		{
			token = new_token(TOKEN_OPERATOR);
			token.Value = cut_string(lexer->source, lexer->position, 1);
			token.line = lexer->line;
			token.column = lexer->column;
			if (c == '*' || c == '/')
			{
				token.Precedence = 1;
			}
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == '{')
		{
			token = new_token(TOKEN_LBRACE);
			token.Value = "{";
			token.line = lexer->line;
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == '}')
		{
			token = new_token(TOKEN_RBRACE);
			token.Value = "}";
			token.line = lexer->line;
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == '(' || c == ')')
		{
			TODO("implement parentheses");
			lexer_advance(lexer);
		}
		else if (c == '[' || c == ']')
		{
			TODO("implement square brackets");
			lexer_advance(lexer);
		}
		else if (c == '#')
		{
			token = new_token(TOKEN_HASH);
			token.Value = "#";
			token.line = lexer->line;
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == ';')
		{
			token = new_token(TOKEN_SEMICOLON);
			token.Value = ";";
			token.line = lexer->line;
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (c == ':')
		{
			token = new_token(TOKEN_COLON);
			token.Value = ":";
			token.line = lexer->line;
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (isspace(c))
		{
			lexer_advance(lexer);
		}
		else
		{
			printf("Unhandled char: '%c' (%u) \n", c, c);
			exit(2);
		}
	}
	Token eof_token = new_token(TOKEN_EOF);
	eof_token.Value = "EOF";
	eof_token.line = lexer->line;
	eof_token.column = lexer->column;
	meats_array_add(lexer->Tokens, &eof_token);
}

void lexer_free(Lexer *lexer)
{
	printf("Freeing lexer\n");
	free(lexer);
}
