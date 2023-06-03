#include "../src/lexer.h"
#include <stdio.h>
#include <string.h>


int test_nextToken() {
	Lexer lexer = newLexer(
		"let five = 5;\n"
		"let ten = 10;\n\n"
		"let add = fn(a, b) {\n"
		"	a+b\n"
		"};\n\n"
		"let result = add(five, ten);\n"
		"!-*/13289;\n"
		"5< 10 ==5\n"
		"if(5 <= 10) {\n"
		"	return true<<2\n"
		"}else{\n"
		"	return false\n"
		"}\n"
	);
	printf("--------------------------\n\tLEXER TEST\n--------------------------\n\n%s\n\n", lexer.input);
	Token token = nextToken(&lexer);
	Token expected_tokens[] = {
		{LET,"let"},
		{IDENT,"five"},
		{ASSIGN,"="},
		{INT,"5"},
		{SEMICOLON,";"},
		
		{LET,"let"},
		{IDENT,"ten"},
		{ASSIGN,"="},
		{INT,"10"},
		{SEMICOLON,";"},

		{LET,"let"},
		{IDENT,"add"},
		{ASSIGN,"="},
		{FUNC,"fn"},
		{L_PAREN,"("},
		{IDENT,"a"},
		{COMMA,","},
		{IDENT,"b"},
		{R_PAREN,")"},
		{L_BRACE,"{"},
		{IDENT,"a"},
		{ADD,"+"},
		{IDENT,"b"},
		{R_BRACE,"}"},
		{SEMICOLON,";"},
		
		{LET,"let"},
		{IDENT,"result"},
		{ASSIGN,"="},
		{IDENT,"add"},
		{L_PAREN,"("},
		{IDENT,"five"},
		{COMMA,","},
		{IDENT,"ten"},
		{R_PAREN,")"},
		{SEMICOLON,";"},
	
		{NOT,"!"},
		{SUB,"-"},
		{MUL,"*"},
		{DIV,"/"},
		{INT,"13289"},
		{SEMICOLON,";"},

		{INT,"5"},
		{LT,"<"},
		{INT,"10"},
		{EQ,"=="},
		{INT,"5"},

		{IF,"if"},
		{L_PAREN,"("},
		{INT,"5"},
		{LT_EQ,"<="},
		{INT,"10"},
		{R_PAREN,")"},
		{L_BRACE,"{"},

		{RETURN,"return"},
		{TRUE,"true"},
		{L_SHIFT,"<<"},
		{INT,"2"},

		{R_BRACE,"}"},
		{ELSE,"else"},
		{L_BRACE,"{"},

		{RETURN,"return"},
		{FALSE,"false"},

		{R_BRACE,"}"},	
		
		{END_OF_FILE,""},
	};
	Token *current_expected_token = expected_tokens;
	int i = 0;
	while (current_expected_token->type != END_OF_FILE) {
		if (tokencmp(&token, current_expected_token)) {
			printf("test fail at word  %d : expected \"%s\" (%d) read \"%s\" (%d)\n", i, current_expected_token->literal, current_expected_token->type, token.literal, token.type);
			return 1;
		}
		token = nextToken(&lexer);
		printf("{ %s, \"%s\" }\n", tokentypetostr(token.type), token.literal);
		i++;
		current_expected_token++;
	}
	printf("--------------------------\n\tTEST PASSED\n--------------------------\n\n");
	return 0;
}

int main() {
	test_nextToken();
	char input_buffer[1024];
	char input_buffer_check;
	Lexer lexer;
	Token token;
	printf(" Monkey Interactive Lexer\n__________________________\n");
	while (1) {
		printf(">>");
		token = newToken(NO_TYPE,"");
		for (int i = 0; i < 1024; i++) {
			input_buffer[i] = '\0';
		}
		fgets(input_buffer, 1024, stdin);
		if (input_buffer[1022] != '\0' && input_buffer[1022] != '\n') {
			do {
				input_buffer_check = getchar();
			} while (input_buffer_check != EOF && input_buffer_check != '\n');
			printf("input to long\n\n");
			continue;
		}
		lexer = newLexer(input_buffer);
		int i = 0;
		while (token.type != END_OF_FILE) {
			token = nextToken(&lexer);
			i++;
			printf("%d: { %s, \"%s\" }\n", i, tokentypetostr(token.type), token.literal);
		}
		printf("\n");
	}
	return 0;
}
