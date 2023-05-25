#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
	NO_TYPE,
	ILLEGAL,
	END_OF_FILE,
	
	IDENT,
	INT,

	FUNC,
	LET,

	ASSIGN,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,

	COMMA,
	SEMICOLON,

	L_PAREN, // (
	R_PAREN, // )
	L_BRACE, // {
	R_BRACE, // }
	L_BRACK, // [
	R_BRACK, // ]

};

struct Keyword {
	char* string;
	enum TokenType type;
};
const struct Keyword keywords[] = {
	{"let",LET},
	{"fn",FUNC},
	{"",NO_TYPE},
};

struct Token {
	enum TokenType type;
	char *literal;
};

struct Lexer {
	char *input;
	int position;
	int read_position;
	char current_char;
};

void nextChar(struct Lexer *lex) {
	if ((lex->input+lex->read_position) == NULL) {
		lex->current_char = '\0';
	} else {
		lex->current_char = lex->input[lex->read_position];
	}
	lex->position = lex->read_position;
	lex->read_position++;
}

struct Lexer newLexer(char* input) {
	struct Lexer lex = {
		input,
		0,
		0,
		0,
	};
	nextChar(&lex);
	return lex;
}

char *readIdentifier(struct Lexer *lex) {
	int i = 0;
	while (isalpha(lex->current_char) || lex->current_char == '_') {
		i++;
		nextChar(lex);
	}
	int identifier_length = i+1;
	char *identifier = malloc(identifier_length*sizeof(char));
	while (i > 0) {
		identifier[identifier_length-1-i] = lex->input[lex->position-i];
		i--;
	}
	identifier[identifier_length-1] = '\0';
	return identifier;
}

struct Token nextToken(struct Lexer *lex) {
	struct Token token;
	switch (lex->current_char) {
		case '=': token.type = ASSIGN; token.literal = "="; break;
		case '+': token.type = ADD; token.literal = "+"; break;
		case ',': token.type = COMMA; token.literal = ","; break;
		case ';': token.type = SEMICOLON; token.literal = ";"; break;
		case '(': token.type = L_PAREN; token.literal = "("; break;
		case ')': token.type = R_PAREN; token.literal = ")"; break;
		case '{': token.type = L_BRACE; token.literal = "{"; break;
		case '}': token.type = R_BRACE; token.literal = "}"; break;
		case '\0': token.type = END_OF_FILE; token.literal = "\0"; break;
		default:
			if (isalpha(lex->current_char) != 0 && lex->current_char != '_') {
				token.type = ILLEGAL;
				token.literal = &lex->current_char;
			}
			char* identifier = readIdentifier(lex);
			token.literal = identifier;
			int i = 0;
			int isKeyword;
			while (strcmp(keywords[i].string,"")) {
				if (strcmp(keywords[i].string,identifier) == 0) {
					token.type = keywords[i].type;
					return token;
				}
				i++;
			}
	}
	nextChar(lex);
	return token;
}
