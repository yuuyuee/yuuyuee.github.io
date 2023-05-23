
all: lexer;

lexer.h lexer.c: lexer.l
	flex --header-file=lexer.h -o lexer.c $<

lexer: lexer.h lexer.c
	gcc -Wall -Wextra -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -g -I. -o $@ $^ -lasan

clean:
	rm -rf lexer lexer.h lexer.c
