#include "parsertostr.h"

/******************************************************************************
				    PRIVATE
******************************************************************************/

int _identtostr(char** str, Identifier *ident) {
	strcat(*str, "Identifier { token: { ");
	strcat(*str, tokentypetostr(ident->token.type));
	strcat(*str, ", \""); 
	strcat(*str, ident->token.literal);
	strcat(*str, "\" } | value: { \""); 
	strcat(*str, ident->value);
	strcat(*str, "\" }"); 
	return 0;
}
int _expressiontostr(char** str, int buf_size, Expression *expression) {
	if (strlen(*str)+200 > buf_size) {
		buf_size += 1000;
		*str = realloc(*str, buf_size);
		*str[buf_size-1] = '\0';
	}
	switch (expression->type) {
		case EXPRESSION_IDENTIFIER:
			if (expression == NULL || expression->node == NULL) {
				strcat(*str, "Identifier {   }");
				break;
			}
			strcat(*str, "Identifier { token: ");
			strcat(*str, tokentypetostr(
					expression->node->identifier.token.type
			));
			strcat(*str, ", ");
			//strcat(*str, expression->node->identifier.token.literal);
			strcat(*str, " | value: ");
			strcat(*str, expression->node->identifier.value);
			strcat(*str, " }");
			break;
		default:
			strcat(*str, "Expression {   }");
	}
	return 0;
}

int _tokentotstr(char** str, Token* token) {
			strcat(*str, "token: { "); 
			strcat(*str, tokentypetostr(token->type));
			strcat(*str, ", \""); 
			strcat(*str, token->literal);
			strcat(*str, "\" }"); 
			return 0;
}

int _lettostr(char** str, int buf_size, Statement *statement) {
			if (statement->node == NULL) {
				strcat(*str, "Let {   }"); 
				return 0;
			}
			strcat(*str, "Let { "); 
			_tokentotstr(str, &statement->node->let.token);
			strcat(*str, " | ");
			if (statement->node->let.identifier == NULL) {
			} else {
				_identtostr(str, statement->node->let.identifier);
			}
			strcat(*str, " | ");
			if (_expressiontostr(str, buf_size, statement->node->
								let.expression)) {
				return 1;		
			}
			strcat(*str, " }"); 
			return 0;
}
int _returntostr(char** str, int buf_size, Statement *statement) {
	if (statement->node == NULL) {
		strcat(*str, "Return {   }");
		return 0;
	}
	strcat(*str, "Return { ");
	_tokentotstr(str, &statement->node->ret.token);
	strcat(*str, " | ");
	_expressiontostr(str, buf_size, statement->node->ret.expression);
	strcat(*str, " }");
	return 0;
}
int _statementtostr(char** str, int buf_size, Statement *statement) {
	if (strlen(*str)+200 > buf_size) {
		buf_size += 1000;
		*str = realloc(*str, buf_size);
		*str[buf_size-1] = '\0';
	}
	switch (statement->type) {
		case STATEMENT_LET:
			if(_lettostr(str, buf_size, statement)) return 1;
			break;
		case STATEMENT_RETURN:
			if (_returntostr(str, buf_size,  statement)) return 2;
			break;
		case STATEMENT_END:
			strcat(*str, "End");
			break;
	}
	return 0;
}


/******************************************************************************
				    PUBLIC
******************************************************************************/

char* programtostr(Program *program) {
	char* _s; 
	char** str = &_s;
	*str = malloc(1000*sizeof(char));
	*str[0] = '\0';
	int buf_size = 1000;
	for (int i = 0; i < program->statement_amount; i++) {
		char counter[10];
		snprintf(counter, 10,"%d: ", i+1);
		counter[9] = '\0';
		if ((strlen(*str)+10*sizeof(char)) > buf_size) {
			buf_size *= 2;
			str = realloc(*str, buf_size);
		}
		strcat(*str, counter);
		_statementtostr(str, buf_size, program->statements+i);
		strcat(*str, "\n");
	}

	return *str;
}
