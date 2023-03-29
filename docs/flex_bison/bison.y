/*** Definition section ***/
%{
#include <stdio.h>
#include <stdlib.h>
}%

%token <symp> NAME
%token <dval> NUMBER
%left '-' '+'
%left '*' '/'
%type <dval> expression

/*** Rule section ***/
%%
statement_list: statement_list '\n'
              | statement_list statement '\n'

statement: NAME '=' expression { $1->value = $3;  }
         | expression { printf("= %g\n", $1); }

expression: NUMBER
          | NAME { $$ = $1->value; }
%%

/*** C code section ***/
int main(int argc, char* argv[]) {

    return 0;
}
