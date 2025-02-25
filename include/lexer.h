#ifndef LEXER_H
#define LEXER_H
#include "meats.h"
#include "token.h"
typedef struct
{
	char *source;
	int length;
	int position;
	int line;
	int column;
	MeatsArray *Tokens;
} Lexer;

void lexer_init(Lexer *lexer, char *source, size_t length);
void lexer_free(Lexer *lexer);
void lexer_tokenize(Lexer *lexer);

#endif // LEXER_H
