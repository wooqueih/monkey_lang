//top down operator precedence parser
#include "parser.h"
#include "lexer.h"


/******************************************************************************
				    PRIVATE
******************************************************************************/

static void ((*_prefixParseFns[AMOUNT_OF_TOKEN_TYPES])(Token)) = {0};
static void ((*_infixParseFns[AMOUNT_OF_TOKEN_TYPES])(Token)) = {0};
//static void ((*_postfixParseFns[AMOUNT_OF_TOKEN_TYPES])(Token)) = {0};

/*int _hasPrefixParseFn(TokenType type) {
	if (_prefixParseFns[type] == NULL) {
		return 0;
	}
	return 1;
}
int _hasInfixParseFn(TokenType type) {
	if (_infixParseFns[type] == NULL) {
		return 0;
	}
	return 1;
}
int _hasPostfixParseFn(TokenType type) {
	if (_postfixParseFns[type] == NULL) {
		return 0;
	}
	return 1;
}*/
void _registerPrefixParseFn(TokenType type, void fn (Token) ) {
	_prefixParseFns[type] = fn;
}
void _registerInfixParseFn(TokenType type, void fn (Token) ) {
	_infixParseFns[type] = fn;
}
/*void _registerPostfixParseFn(TokenType type, void fn (Token) ) {
	_postfixParseFns[type] = fn;
}*/


void _nextToken_Parser(Parser *parser) {
	parser->current_token = parser->next_token;
	parser->next_token = nextToken(parser->lexer);
}

int _expectNext(Parser *par, TokenType type) {
	if (par->next_token.type == type) {
		return 0;
	}
	if (par->error_amount+1 > par->error_capacity) {
		if (par->error_capacity > 100000) {
			return 2;
		}
		par->error_capacity *= 2;
		char** tmp = realloc(par->errors, par->error_capacity*sizeof(char*));
		if (tmp == NULL) {
			return 3;
		}
		par->errors = tmp;
	}
	par->errors[par->error_amount] = malloc(100*sizeof(char));
	snprintf(par->errors[par->error_amount], 100,"expected %s got %s (\"%s\")", 
					tokentypetostr(type),
					tokentypetostr(par->next_token.type),
					par->next_token.literal);
	par->error_amount++;
	return 1;
}


int _parseStatement(Parser* par, Statement* statement);

int _parseExpression(Parser* par, Expression* expression) {
	return 0;
}

int _parseLetStatement(Parser* par, Statement* statement) {
	statement->type = STATEMENT_LET;
	statement->node = malloc(sizeof(StatementUnion));
	statement->node->let.identifier = NULL;
	statement->node->let.expression = NULL;

	statement->node->let.token.type = par->current_token.type;
	statement->node->let.token.literal = malloc(4*sizeof(char));
	strcpy(statement->node->let.token.literal, par->current_token.literal);

	if (_expectNext(par, IDENT) == 0) {
		_nextToken_Parser(par);
		statement->node->let.identifier = malloc(sizeof(Identifier));
		statement->node->let.identifier->token.literal = malloc(15*sizeof(char));
		statement->node->let.identifier->token.type = par->current_token.type;
		strncpy(statement->node->let.identifier->token.literal,
						par->current_token.literal,
						15);
		statement->node->let.identifier->value = "TODO";
	}

	
	if (_expectNext(par, ASSIGN) == 0) {
		_nextToken_Parser(par);
		_nextToken_Parser(par);
		statement->node->let.expression = malloc(sizeof(Expression));
	}


	
	while (par->current_token.type != SEMICOLON) {
		if (par->current_token.type == END_OF_FILE) {
			return 2;
		}
		_nextToken_Parser(par);
	}
	_nextToken_Parser(par);
	return 0;
}
int _parseReturnStatement(Parser* par, Statement* statement) {
	statement->type = STATEMENT_RETURN;
	statement->node = malloc(sizeof(StatementUnion));
	statement->node->ret.token = par->current_token;

	_nextToken_Parser(par);
	statement->node->ret.expression = malloc(sizeof(Expression));

	while (par->current_token.type != SEMICOLON) {
		if (par->current_token.type == END_OF_FILE) {
			return 2;
		}
		_nextToken_Parser(par);
	}
	_nextToken_Parser(par);
	return 0;
}
int _parseStatement(Parser* par, Statement* statement) {
	//printf("%s\n", tokentypetostr(par->current_token.type));
	switch (par->current_token.type) {
		case LET:
			printf("LET\n");
			if (_parseLetStatement(par, statement))
				return 1;
			break;
		case RETURN:
			printf("RETURN\n");
			if (_parseReturnStatement(par, statement))
				return 2;
			break;
		default:
			/*printf("TOKEN NOT IMPLEMENTED\n");
			_nextToken_Parser(par);*/
			break;
	}
	return 0;
}


/******************************************************************************
				    PUBLIC
******************************************************************************/

Parser newParser(Lexer* lex) {
	Parser par;
	par.lexer = lex;
	par.errors = malloc(sizeof(char*)*5);
	par.error_amount = 0;
	par.error_capacity = 500;

	_nextToken_Parser(&par);
	_nextToken_Parser(&par);

	return par;
}

Program parseProgram(Parser *par) {
	Program p;
	p.statements = malloc(sizeof(Statement)*200); //bufferoverflow
	int buf_size = 200;
	int i = 0;
	while (par->current_token.type != END_OF_FILE) {
		if (i > 100000) {
			break;
		}
		if (i+1 >= buf_size) {
			buf_size *= 2;
			printf("\nbuf_size is %d\n\n", buf_size);
			Statement* ptr;
			ptr = realloc(p.statements, buf_size*sizeof(Statement));
			if (ptr == NULL) {
				break;
			}
			//free(p.statements);
			p.statements = ptr;
		}
		_parseStatement(par, p.statements+i);
		i++;
	}
	p.statements[i].type = STATEMENT_END;
	p.statement_amount = i+1;
	return p;
}
