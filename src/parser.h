#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

typedef struct {
	Token token;
	char *value;
} Identifier;

typedef enum {
	PRECEDENCE_LOWEST = 0,
	PRECEDENCE_EQUALS = 1,
	PRECEDENCE_LESSGREATER = 2,
	PRECEDENCE_ADD = 3,
	PRECEDENCE_MUL = 4,
	PRECEDENCE_PREFIX = 5,
	PRECEDENCE_CALL = 6,
} Precedence;

typedef enum {
	EXPRESSION_IDENTIFIER,
} ExpressionType;

typedef union {
	Identifier identifier;
} ExpressionUnion;

typedef struct {
	ExpressionType type;
	ExpressionUnion *node;
} Expression;

typedef enum {
	STATEMENT_END,
	STATEMENT_LET,
	STATEMENT_RETURN,
} StatementType;

typedef struct {
	Token token;
	Identifier *identifier;
	Expression *expression;
} Let;

typedef struct {
	Token token;
	Expression *expression;
} Return;

typedef struct {
	//Token first_token;
	Expression *expression;
} ExpressionStatement;

typedef struct {

} ProgramEnd;

typedef union {
	ProgramEnd end;
	Let let;
	ExpressionStatement expr;
	Return ret;
} StatementUnion;

typedef struct {
	StatementType type;
	StatementUnion *node;
} Statement;

typedef struct {
	Statement *statements;
	int statement_amount;
} Program;

typedef struct {
	Lexer *lexer;
	Token current_token;
	Token next_token;
	char **errors;
	int error_amount;
	int error_capacity;
} Parser;



extern Parser newParser(Lexer* lex);
extern Program parseProgram(Parser* par);
extern char* programtostr(Program* program);

#endif
