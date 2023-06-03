#include "lexer.h"

/******************************************************************************
				    PRIVATE
******************************************************************************/
const Keyword _keywords[] = {
	{"let",LET},
	{"fn",FUNC},
	{"return",RETURN},
	{"if",IF},
	{"else",ELSE},
	{"true",TRUE},
	{"false",FALSE},
	{"",NO_TYPE},
};
void _nextChar(Lexer *lex) {
	if ((lex->input+lex->read_position) == NULL) {
		lex->current_char = '\0';
	} else {
		lex->current_char = lex->input[lex->read_position];
	}
	lex->position = lex->read_position;
	if (lex->input[lex->read_position] != '\0') {
		lex->read_position++;
	}
}

char _peekChar(Lexer *lex) {
	if ((lex->input+lex->read_position) == NULL) {
		return '\0';
	} else {
		return lex->input[lex->read_position];
	}
}

void _skipWhitespace(Lexer *lex) {
	while (lex->current_char == ' ' || lex->current_char == '\t' 
			|| lex->current_char == '\n'
			|| lex->current_char == '\r') {
		_nextChar(lex);
	}
}

char *_readIdentifier(Lexer *lex) {
	int i = 0;
	while (isalpha(lex->current_char) || lex->current_char == '_') {
		i++;
		_nextChar(lex);
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

char *_readInteger(Lexer *lex) {
	int i = 0;
	while (isdigit(lex->current_char)) {
		i++;
		_nextChar(lex);
	}
	int number_length = i+1;
	char *number = malloc(number_length*sizeof(char));
	while (i > 0) {
		number[number_length-1-i] = lex->input[lex->position-i];
		i--;
	}
	number[number_length-1] = '\0';
	return number;
}



/******************************************************************************
				    PUBLIC
******************************************************************************/

Token newToken(TokenType type, char* literal) {
	Token token = {type,literal};
	return token;
}
char *tokentypetostr(TokenType type) {
	char *type_string;
	switch (type) {
		case NO_TYPE: type_string = "NO_TYPE"; break;
		case ILLEGAL: type_string = "ILLEGAL"; break;
		case END_OF_FILE: type_string = "END_OF_FILE"; break;
		case IDENT: type_string = "IDENT"; break;
		case INT: type_string = "INT"; break;
		case FUNC: type_string = "FUNC"; break;
		case LET: type_string = "LET"; break;
		case RETURN: type_string = "RETURN"; break;
		case IF: type_string = "IF"; break;
		case ELSE: type_string = "ELSE"; break;
		case TRUE: type_string = "TRUE"; break;
		case FALSE: type_string = "FALSE"; break;
		case ASSIGN: type_string = "ASSIGN"; break;
		case ADD: type_string = "ADD"; break;
		case INCREMENT: type_string = "INCREMENT"; break;
		case SUB: type_string = "SUB"; break;
		case DECREMENT: type_string = "DECREMENT"; break;
		case MUL: type_string = "MUL"; break;
		case DIV: type_string = "DIV"; break;
		case MOD: type_string = "MOD"; break;
		case NOT: type_string = "NOT"; break;
		case LT: type_string = "LT"; break;
		case GT: type_string = "GT"; break;
		case EQ: type_string = "EQ"; break;
		case NEQ: type_string = "NEQ"; break;
		case LT_EQ: type_string = "LT_EQ"; break;
		case GT_EQ: type_string = "GT_EQ"; break;
		case AND_L: type_string = "AND_L"; break;
		case OR_L: type_string = "OR_L"; break;
		case AND_B: type_string = "AND_B"; break;
		case OR_B: type_string = "OR_B"; break;
		case XOR: type_string = "XOR"; break;
		case COMPLEMENT: type_string = "COMPLEMENT"; break;
		case L_SHIFT: type_string = "L_SHIFT"; break;
		case R_SHIFT: type_string = "R_SHIFT"; break;
		case COMMA: type_string = "COMMA"; break;
		case SEMICOLON: type_string = "SEMICOLON"; break;
		case L_PAREN:  type_string = "L_PAREN"; break;
		case R_PAREN:  type_string = "R_PAREN"; break;
		case L_BRACE:  type_string = "L_BRACE"; break;
		case R_BRACE:  type_string = "R_BRACE"; break;
		case L_BRACK:  type_string = "L_BRACK"; break;
		case R_BRACK: type_string = "R_BRACK"; break;
		default: type_string = "NO_TYPE"; break;
	}
	return type_string;
}

Lexer newLexer(char* input) {
	Lexer lex = {
		input,
		0,
		0,
		0,
	};
	_nextChar(&lex);
	return lex;
}

int tokencmp(Token *t1, Token *t2) {
	if (t1->type != t2->type || strcmp(t1->literal, t2->literal)) {
		return 1;
	}
	return 0;
}

Token nextToken(Lexer *lex) {
	Token token;
	_skipWhitespace(lex);
	switch (lex->current_char) {
		case '^': token.type = XOR; token.literal = "^"; break;
		case '=':
			if (_peekChar(lex) == '=') {
				_nextChar(lex);
				token.type = EQ; token.literal = "=="; break;
			} 
			token.type = ASSIGN; token.literal = "="; break;
		case '+':
			if (_peekChar(lex) == '=') {
				_nextChar(lex);
				token.type = INCREMENT; token.literal = "++";
				break;
			}
			token.type = ADD; token.literal = "+"; break;
		case '-':
			if (_peekChar(lex) == '-') {
				_nextChar(lex);
				token.type = DECREMENT; token.literal = "--";
				break;
			}
			token.type = SUB; token.literal = "-"; break;
		case '*': token.type = MUL; token.literal = "*"; break;
		case '/': token.type = DIV; token.literal = "/"; break;
		case '%': token.type = MOD; token.literal = "%"; break;
		case '<':
			if (_peekChar(lex) == '=') {
				_nextChar(lex);
				token.type = LT_EQ; token.literal = "<=";
				break;
			}
			if (_peekChar(lex) == '<') {
				_nextChar(lex);
				token.type = L_SHIFT; token.literal = "<<";
				break;
			}
			token.type = LT; token.literal = "<"; break;
		case '>':
			if (_peekChar(lex) == '=') {
				_nextChar(lex);
				token.type = GT_EQ; token.literal = ">=";
				break;
			}
			if (_peekChar(lex) == '>') {
				_nextChar(lex);
				token.type = R_SHIFT; token.literal = ">>";
				break;
			}
			token.type = GT; token.literal = ">"; break;
		case '!':
			if (_peekChar(lex) == '=') {
				_nextChar(lex);
				token.type = NEQ; token.literal = "!=";
				break;
			}
			token.type = NOT; token.literal = "!"; break;
		case '&':
			if (_peekChar(lex) == '&') {
				_nextChar(lex);
				token.type = AND_L; token.literal = "&&";
				break;
			}
			token.type = AND_B; token.literal = "&"; break;
		case '|':
			if (_peekChar(lex) == '|') {
				_nextChar(lex);
				token.type = OR_L; token.literal = "||";
				break;
			}
			token.type = OR_B; token.literal = "|"; break;
		case ',': token.type = COMMA; token.literal = ","; break;
		case ';': token.type = SEMICOLON; token.literal = ";"; break;
		case '(': token.type = L_PAREN; token.literal = "("; break;
		case ')': token.type = R_PAREN; token.literal = ")"; break;
		case '{': token.type = L_BRACE; token.literal = "{"; break;
		case '}': token.type = R_BRACE; token.literal = "}"; break;
		case '\0': token.type = END_OF_FILE; token.literal = "\0"; break;
		default:
			if (isdigit(lex->current_char)) {
				token.literal = _readInteger(lex);
				token.type = INT;
				return token;
			}
			if (isalpha(lex->current_char) == 0 && lex->current_char != '_') {
				token.type = ILLEGAL;
				token.literal = &lex->current_char;
				_nextChar(lex);
				return token;
			}
			char* identifier = _readIdentifier(lex);
			token.literal = identifier;
			int i = 0;
			while (_keywords[i].type != NO_TYPE) {
				if (strcmp(_keywords[i].string,identifier) == 0) {
					token.type = _keywords[i].type;
					return token;
				}
				i++;
			}
			token.type = IDENT;
			return token;
	}
	_nextChar(lex);
	return token;
}
