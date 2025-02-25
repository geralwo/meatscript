#ifndef ASTNODE_H
#define ASTNODE_H

#include <stdlib.h>
#include "vm/bytecode.h"

typedef enum
{
	AST_STATEMENT,
	AST_IDENTIFIER,
	AST_VAR_DEF,
	AST_CONST_DEF,
	AST_NUMBER,
	AST_STRING,
	AST_FUNC_DEF,
	AST_FUNC_CALL,
	AST_BLOCK,
	AST_INLINE_ASM,
} AST_NodeType;

typedef struct
{
	AST_NodeType Type;
	Bytecode *Bytecode;
} AST_Node;

static inline AST_Node *new_ast_node(AST_NodeType type)
{
	AST_Node *node = (AST_Node *)malloc(sizeof(AST_Node));
	node->Type = type;
	node->Bytecode = new_bytecode();
	return node;
}

#endif // ASTNODE_H
