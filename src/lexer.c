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
	lexer->line = 1;
	lexer->column = 1;
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
		// char c = current_char(lexer);
		//  printf(":: lexing character '%u'\n",c);
		if (isdigit(current_char(lexer)))
		{
			token = new_token(TOKEN_NUMBER, lexer->line);
			int start = lexer->position;
			while (isdigit(current_char(lexer)) || current_char(lexer) == '.' || current_char(lexer) == '_' || current_char(lexer) == 'x')
			{
				lexer_advance(lexer);
			}
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			token.column = lexer->column;
			// printf("lexed digit '%s' at index: %d\n",token.Value,start);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (isalpha(current_char(lexer)))
		{
			token = new_token(TOKEN_IDENTIFIER, lexer->line);
			int start = lexer->position;
			token.column = lexer->column;
			while (isalnum(current_char(lexer)) || current_char(lexer) == '_')
			{
				lexer_advance(lexer);
			}
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			meats_array_add(lexer->Tokens, &token);
			// printf("lexed identifier '%s' at index: %d\n",token.Value,start);
			// lexer_advance(lexer);
		}
		else if (current_char(lexer) == '\n')
		{
			token = new_token(TOKEN_EOL, lexer->line);
			token.Value = "EOL";
			token.column = lexer->column;
			lexer->column = 0;
			meats_array_add(lexer->Tokens, &token);
			lexer_advance(lexer);
		}
		else if (current_char(lexer) == '\r')
		{
			lexer_advance(lexer);
		}
		else if (current_char(lexer) == '\'' || current_char(lexer) == '"')
		{
			token = new_token(TOKEN_STRING, lexer->line);
			char quote = current_char(lexer);
			int start = lexer->position;
			lexer_advance(lexer); // eat starting quote
			while (current_char(lexer) != quote)
			{
				lexer_advance(lexer); // eat everything between quotes
			}
			lexer_advance(lexer); // eat closing quote
			token.Value = cut_string(lexer->source, start, lexer->position - start);
			token.column = lexer->column;
			meats_array_add(lexer->Tokens, &token);
		}
		// math operators
		else if (current_char(lexer) == '+' || current_char(lexer) == '-' || current_char(lexer) == '*' || current_char(lexer) == '/' || current_char(lexer) == '%')
		{
			token = new_token(TOKEN_OPERATOR, lexer->line);
			token.Value = cut_string(lexer->source, lexer->position, 1);
			token.column = lexer->column;
			if (current_char(lexer) == '*' || current_char(lexer) == '/')
			{
				token.Precedence = 1;
			}
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == '{')
		{
			token = new_token(TOKEN_LBRACE, lexer->line);
			token.Value = "{";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == '=')
		{
			token = new_token(TOKEN_EQUAL, lexer->line);
			token.Value = "=";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == ',')
		{
			token = new_token(TOKEN_COMMA, lexer->line);
			token.Value = ",";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == '}')
		{
			token = new_token(TOKEN_RBRACE, lexer->line);
			token.Value = "}";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == '(')
		{
			token = new_token(TOKEN_LPAREN, lexer->line);
			token.Value = "(";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == ')')
		{
			token = new_token(TOKEN_RPAREN, lexer->line);
			token.Value = ")";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == '[' || current_char(lexer) == ']')
		{
			TODO("implement square brackets");
			lexer_advance(lexer);
		}
		else if (current_char(lexer) == '#')
		{
			token = new_token(TOKEN_HASH, lexer->line);
			token.Value = "#";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == ';')
		{
			token = new_token(TOKEN_SEMICOLON, lexer->line);
			token.Value = ";";
			token.column = lexer->column;
			lexer_advance(lexer);
			meats_array_add(lexer->Tokens, &token);
		}
		else if (current_char(lexer) == ':')
		{
			lexer_advance(lexer);
			if (isalpha(current_char(lexer)))
			{
				token = new_token(TOKEN_SYMBOL, lexer->line);
				token.column = lexer->column;
				int start = lexer->position - 1;
				token.column = lexer->column;
				while (isalnum(current_char(lexer)) || current_char(lexer) == '_')
				{
					lexer_advance(lexer);
				}
				token.Value = cut_string(lexer->source, start, lexer->position - start);
				meats_array_add(lexer->Tokens, &token);
			}
			else
			{
				token = new_token(TOKEN_COLON, lexer->line);
				token.Value = ":";
				token.column = lexer->column;
				lexer_advance(lexer);
				meats_array_add(lexer->Tokens, &token);
			}
		}
		else if (isspace(current_char(lexer)))
		{
			lexer_advance(lexer);
		}
		else
		{
			printf("Unhandled char: '%c' (%u) \n", current_char(lexer), current_char(lexer));
			exit(2);
		}
	}
	Token eof_token = new_token(TOKEN_EOF, lexer->line);
	eof_token.Value = "EOF";
	eof_token.column = lexer->column;
	meats_array_add(lexer->Tokens, &eof_token);
}

void lexer_free(Lexer *lexer)
{
	printf("Freeing lexer\n");
	free(lexer);
}
