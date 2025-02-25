#ifndef TOKEN_H
#define TOKEN_H
#include <stdlib.h>
// Token type enumeration
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_BINARY_OPERATOR,
    TOKEN_EQUAL,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_EOL,
    TOKEN_EOF,
    TOKEN_ANY,
} TokenType;


// Token structure
typedef struct {
    TokenType Type;
    char* Value;
    int line;
    int column;
    int Precedence;
} Token;

static inline Token new_token(TokenType type)
{
    Token token;
    token.Type = type;
    token.Value = NULL;
    token.line = -1;
    token.column = -1;
    token.Precedence = 0;
    return token;
}

static inline const char* tokenType_name(TokenType type)
{
    switch(type)
    {
	case TOKEN_IDENTIFIER:
		return "IDENTIFIER";
	case TOKEN_NUMBER:
		return "NUMBER";
	case TOKEN_STRING:
		return "STRING";
	case TOKEN_OPERATOR:
		return "OPERATOR";
	case TOKEN_BINARY_OPERATOR:
		return "BINARY OPERATOR";
	case TOKEN_EQUAL:
		return "EQUAL";
	case TOKEN_EOL:
		return "EOL";
	case TOKEN_EOF:
		return "EOF";
	case TOKEN_ANY:
		return "ANY";
	case TOKEN_RBRACE:
		return "RBRACE";
	case TOKEN_LBRACE:
		return "LBRACE";
	default:
		return "UNKNOWN";
    }
}

void free_token(void* token);

#endif // TOKEN_H
