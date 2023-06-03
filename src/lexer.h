#ifndef LEXER
#define LEXER

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define AMOUNT_OF_TOKEN_TYPES 51


typedef enum {
	NO_TYPE = 0,
	ILLEGAL,
	END_OF_FILE,
	
	IDENT,
	INT,

	FUNC,
	LET,
	RETURN,
	IF,
	ELSE,
	TRUE,
	FALSE,

	ASSIGN,
	ADD,
	INCREMENT,
	SUB,
	DECREMENT,
	MUL,
	DIV,
	MOD,
	NOT,

	LT,
	GT,
	EQ,
	NEQ,
	LT_EQ,
	GT_EQ,

	AND_L,
	OR_L,

	AND_B,
	OR_B,
	XOR,
	COMPLEMENT,
	L_SHIFT,
	R_SHIFT,

	COMMA,
	SEMICOLON,

	L_PAREN, // (
	R_PAREN, // )
	L_BRACE, // {
	R_BRACE, // }
	L_BRACK, // [
	R_BRACK, // ]

} TokenType;

typedef struct {
	char* string;
	TokenType type;
} Keyword;

typedef struct {
	TokenType type;
	char *literal;
} Token;

typedef struct {
	char *input;
	int position;
	int read_position;
	char current_char;
} Lexer;


extern Token newToken(TokenType, char*);
extern char *tokentypetostr(TokenType);
extern int tokencmp(Token*, Token*);
extern Lexer newLexer(char*);
extern Token nextToken(Lexer*);
/*extern void nextChar(Lexer *lex);
extern char peekChar(Lexer *lex);
extern void skipWhitespace(Lexer *lex);
extern char *readIdentifier(Lexer *lex);
extern char *readInteger(Lexer *lex);*/

#endif
