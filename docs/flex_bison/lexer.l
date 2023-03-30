/*** Definition section ***/

%{
/* Custom comments */
#include <stdio.h>
/* #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; */
%}

/*** Rule section ***/

%option noyywrap
/* %option yylineno */
%option debug
/* %option c++ */
%option perf-report
/* %option verbose */
/* %option prefix=oak */

DIGIT [0-9]
ALPHA [a-zA-Z]
WS [ \t]+

%%

{DIGIT}+ {
    printf("Saw an integer: %s\n", yytext);
    /* return NUMBER; */
}

{WS}+ { /* ignore space */ }

"\n" { /* return END; */ }

. {
    printf("Unrecognized symbol at line %d: %s\n",
            yylineno, yytext);
    /* exit(0); */
}

%%

/*** code section ***/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s express\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char* s = argv[1];
    YY_BUFFER_STATE bs = yy_scan_string(s);
    yylex();
    yy_delete_buffer(bs);
    return 0;
}
