#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main() {
	char input_buffer[1024];
	char input_buffer_check;
	Lexer lexer;
	Token token;
	printf("\n    Monkey Interpreter\n__________________________\n");
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
		if (input_buffer[0] == 'e' &&
			input_buffer[1] == 'x' &&
			input_buffer[2] == 'i' &&
			input_buffer[3] == 't'
		) {
			break;
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
