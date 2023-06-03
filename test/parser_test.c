#include "../src/parser.h"
#include "../src/parsertostr.h"
#include <stdio.h>

int main() {
	setbuf(stdout, NULL);
	char* input = 
		"let a = 10;\n"
		"let = 1+1;\n"
		"return 321;\n";
		//"a;";
	printf(
		"--------------------------\n"
		"        PARSER TEST\n"
		"--------------------------\n"
		"\n"
		"%s\n"
		"\n"
		, input);
	Lexer lexer = newLexer(input);
	Parser parser = newParser(&lexer);
	Program program = parseProgram(&parser);
	char* str = programtostr(&program);
	printf("%s\n", str);
	for (int i = 0; i < parser.error_amount; i++) {
		printf("E %d: %s\n",i+1 ,parser.errors[i]);
	}
	printf("\n");
	printf(
		"--------------------------\n"
		"        TEST PASSED\n"
		"--------------------------\n"
		);

	return 0;
}

/*
 * statement {
 * 	LET;
 * 	let {
 *		"let";
 *
 * 	};
 * };
*/
