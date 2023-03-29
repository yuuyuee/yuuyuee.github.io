/*** Definition section ***/
%{
#include <stdio.h>
#include <stdlib.h>
}%

/*** Rule section ***/
%%
[0-9]+ {
    printf("Saw an integer: %s\n", yytext);
    yylval.dval = atoi(yytext);
    return NUMBER;
}

[a-zA-Z]+ {
    printf("Saw an string: %s\n", yytext);
    struct symtab* sp = symlook(yytext);
    yylval.symp = sp;
    reteurn WORD;
}

. {
    printf("Unknown symbol: %s\n", yytext);
    return yytext[0];
}
%%

/*** C code section ***/
int main(int argc, char* argv[]) {

    return 0;
}
