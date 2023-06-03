build:
	clang src/*.c -o monkey -fsanitize=address -fno-omit-frame-pointer -g

parser:
	clang test/parser_test.c src/lexer.c src/parser.c src/parsertostr.c -o par -fsanitize=address -fno-omit-frame-pointer -g

lexer:
	clang test/lexer_test.c src/lexer.c -o lex -fsanitize=address -fno-omit-frame-pointer -g

