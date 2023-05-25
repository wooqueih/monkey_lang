#include "../src/lexer.c"
#include <stdio.h>
#include <string.h>


int tokencmp(struct Token *t1, struct Token *t2) {
	if (t1->type != t2->type || strcmp(t1->literal, t2->literal)) {
		return 1;
	}
	return 0;
}
int test_nextToken() {
	struct Lexer lexer = newLexer(
		"let five = 5;\n"
		"let ten = 10;\n\n"
		"let add = fn(a, b) {\n"
		"	a+b\n"
		"};\n\n"
		"let result = add(five, ten);"
	);
	printf("input is \"%s\"\n\n", lexer.input);
	struct Token token = nextToken(&lexer);
	struct Token expected_tokens[] = {
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
		{END_OF_FILE,""},
	};
	struct Token *current_expected_token = expected_tokens;
	while (current_expected_token->type != END_OF_FILE) {
		if (tokencmp(&token, current_expected_token)) {
			printf("test fail: expected \"%s\" read \"%s\"\n", current_expected_token->literal, token.literal);
			return 1;
		}
		token = nextToken(&lexer);
		current_expected_token++;
	}
	printf("test successful\n");
	return 0;
}

int main() {
	test_nextToken();
	return 0;
}
