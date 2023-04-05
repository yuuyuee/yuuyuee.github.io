
all: lexer;

lexer.h lexer.c: lexer.l
	flex --header-file=lexer.h -o lexer.c $<

lexer: lexer.h lexer.c
	gcc -Wall -Wextra -g -I. -o $@ $^ -lfl

clean:
	rm -rf lexer lexer.h lexer.c