# The syntax of CONF in Backus-Naur form

```EBNF
(* The syntax of Conf in Backus-Naur Form. *)

(*

function-declaration
  assignment-expression
  compound-statement
  if-else-statement
  for-statement
  while-statement
  switch-statement
class-declaration
  class-statement

*)

(* Operator precedence and associativity
Operators                   | Type of operation         | Associativity
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
(exprs...)                  | parenthesized expression  |
[exprs...]                  | list                      |
{key: value...}             | dictionary                |
{exprs}                     | set                       |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
x[index]                    | subscription              | left to right
x[index:index]              | slicing                   | left to right
x(arguments...)             | function call             | left to right
x.attribute                 | attribute reference       | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**                          | exponentiation            | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+x                          | positive                  | left to right
-x                          | negative                  | left to right
~x                          | bitwise NOT               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*                           | multiplication            | left to right
/                           | division                  | left to right
//                          | floor division            | left to right
%                           | remainder                 | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                           | addition                  | left to right
-                           | substraction              | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
<<                          | left shift                | left to right
>>                          | right shift               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
&                           | bitwise AND               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
^                           | bitwise XOR               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
|                           | bitwise OR                | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
in                          | including membership      | left to right
not in                      | not including membership  | left to right
is                          | identity                  | left to right
is not                      | not identity              | left to right
<                           | less than                 | left to right
<=                          | less or equal than        | left to right
>                           | greater than              | left to right
>=                          | greater or equal than     | left to right
==                          | equal                     | left to right
!=                          | not equal                 | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
not x                       | boolean NOT               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
and                         | boolean AND               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
or                          | boolean OR                | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if - else                   | conditional expression    |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
lambda                      | lambda expression         |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
=                           | assignment expression     |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*)

(* Class definition *)
class-definition ::= 'class' identifier '{' block '}'

(* Function definition *)
function-definition ::= 'func' identifier '(' [params] ')' '{' block '}'




(* Bitwise operators *)
inclusive-or-expression ::= inclusive-or-expression '|' exclusive-or-expression
                          | exclusive-or-expression

exclusive-or-expression ::= exclusive-or-expression '^' and-expression
                          | and-expression

and-expression ::= and-expression '&' shift-expression
                 | shift-expression

shift-expression ::= shift-expression '<<' additive-expression
                   | shift-expression '>>' additive-expression
                   | additive-expression

(* Arithmetic operators *)
additive-expression ::= additive-expression '+' multiplicative-expression
                      | additive-expression '-' multiplicative-expression
                      | multiplicative-expression

multiplicative-expression ::= multiplicative-expression '*' unary-expression
                            | multiplicative-expression '/' unary-expression
                            | multiplicative-expression '//' unary-expression
                            | multiplicative-expression '%' unary-expression
                            | unary-expression

unary-expression ::= '+' unary-expression
                   | '-' unary-expression
                   | '~' unary-expression
                   | postfix-expression

postfix-expression ::= postfix-expression '[' expression ']'
                     | postfix-expression '(' {assignment-expression}* ')'
                     | postfix-expression '.' identifier
                     | primary-expression

(* Primary elements *)
primary-expression ::= identifier
                     | constant
                     | string
                     | '(' expression ')'
```
