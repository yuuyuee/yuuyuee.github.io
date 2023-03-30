# Flex

```flex

/* The flex input file consists of three sections, separated by
   a line with just %% in it. */

/*** definitions section ***/

/*  */

%{
#include <stdio.h>
%}

/* option definition from */
%option noyywrap
%option yylineno
%option debug
/* %option c++ */
%option perf-report
/* %option verbose */
/* %option prefix=oak */

/* name definition form
  <name> <definition>
  The "name" is a word beginning with a letter or an underscore (_) followed
  by zero or more letters, digits, underscore  or dash.
  The "definition" is taken to begin at the firest non-whitespace character
  following the name and continuing to the end of the line.
  The definition can subsequently be referred to using "{name}", which will
  expand to "(definition)".
*/

DIGIT [0-9]
ALPHA [a-zA-Z]
WS [ \t]+


/*** rules section ***/
%%

%%

/*** user code section ***/


```