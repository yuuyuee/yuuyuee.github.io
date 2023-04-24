# The syntax of Conf in Backus-Naur form

## Definition

```EBNF
(* The syntax of Conf in Backus-Naur Form. *)

program ::= block+

block ::= compound_stmt | (simple_stmt ';')

(* simple statement*)
(* assignment must precede expression, else parsing a simple assignment
   will throw a syntax error *)
simple_stmt ::= assign_stmt
              | expression
              | return_stmt
              | raise_stmt
              | assert_stmt
              | 'break'
              | 'continue'

(* assignment statement *)
assign_stmt ::= (targets '=') expressions
targets ::= target (',' target)*
target ::= identify | attr_ref | slicing

(* expression list *)
expressions ::= expression (',' expression)*

expression ::= cond_expr

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
not                         | boolean NOT               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
and                         | boolean AND               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
or                          | boolean OR                | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if - else                   | conditional expression    |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
=                           | assignment expression     |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Note:
  all comparison operators have the same priority, which is lower
than that of arithmetic, shifting or bitwise operation.
  comparison can be chained arbitrarily and have the interpretation
that is conventional in mathematics. like as expression "a < b < c"
is equivalent to "a < b and b < c".
*)

(* conditional expression *)
cond_expr ::= logic_or ['if' logic_or 'else' expression]

(* logical expression *)
logic_or ::= logic_and | logic_or 'or' logic_and
logic_and ::= logic_not logic_and 'and' logic_not
logic_not ::= comp_ops | 'not' logic_not

(* relational expression *)
comp_ops ::= bitwise_or (comp_operator bitwise_or)*
comp_operator ::= '<' | '<=' | '>' | '>=' | '==' | '!='
                | 'is' ['not'] | ['not'] 'in'

(* bitwise operators *)
bitwise_or ::= bitwise_or '|' bitwise_xor | bitwise_xor
bitwise_xor ::= bitwise_xor '^' bitwise_and | bitwise_and
bitwise_and ::= bitwise_and '&' shift_expr | shift_expr

(* shifting expression *)
shift_expr ::= shift_expr ('<<' | '>>') add_expr | add_expr

(* arithmetic operators *)
add_expr ::= add_expr '+' mul_expr
           | add_expr '-' mul_expr
           | mul_expr

mul_expr ::= mul_expr '*' unary_expr
           | mul_expr '/' unary_expr
           | mul_expr '//' unary_expr
           | mul_expr '%' unary_expr
           | unary_expr

unary_expr ::= '+' unary_expr
         | '-' unary_expr
         | '~' unary_expr
         | power_expr

power_expr ::= primary ['**' unary_expr]

(* Primary elements *)
primary ::= attr_ref | func_call | slicing | atom

(* attribute reference *)
attr_ref ::= primary '.' identify

(* function call *)
func_call ::= primary '(' [arg_list] ')'
arg_list ::= arg (',' arg)*
arg ::= identify | expression

(* slicing *)
slicing ::= primary '[' slice_list ']'
slice_list ::= expression | proper_slice
proper_slice ::= [lower_bound] ':' [upper_bound] [':' [stride]]
lower_bound ::= expression
upper_bound ::= expression
stride ::=  expression

(* atomic expression *)
atom ::= identify
       | 'True'
       | 'False'
       | 'None'
       | str
       | number
       | group_expr
       | list
       | (dict | set)

(* identifier *)
identifier ::= ['_'] alpa alpa_num*
alpa_num ::= alpa | dec_digit
alpa ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g'
       | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n'
       | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u'
       | 'v' | 'w' | 'x' | 'y' | 'z'
       | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G'
       | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N'
       | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U'
       | 'V' | 'W' | 'X' | 'Y' | 'Z'
       | '_'

(* string literal *)
str ::= "'" utf8* "'" | '"' utf8* '"'
asc ::= \x00...\x7f
u1 ::= \x80...\xbf
u2 ::= \xc2...\xdf u1
u3 ::= \xe0...\xef u2 u1
u4 ::= \xf0...\xf4 u3 u2 u1
utf8 ::= asc | u2 | u3 | u4

(* number literal *)
number ::= integer | float_point

(* integer literal *)
integer ::= bin_int | oct_int | dec_int | hex_int

bin_int ::= '0' ('b' | 'B')? bin_digit+
bin_digit ::= '0' | '1'

oct_int ::= '0' ('o' | 'O')? oct_digit+
oct_digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7'

dec_int ::= nonzero_dec_digit? dec_digit*
dec_digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
nonzero_dec_digit ::= '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

hex_int ::= '0' ('x' | 'X') hex_digit+
hex_digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
          | 'a' | 'b' | 'c' | 'd' | 'e' | 'f'
          | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'

(* floating point literal *)
float_point ::= exponent_float | point_float

exponent_float ::= (point_float | dec_int) exponent
exponent ::= ('e' | 'E') ['+' | '-'] dec_int
point_float ::= nonzero_dec_digit+ "." dec_digit+

(* group expression *)
group_expr ::= '(' expression ')'

(* list literal *)
list ::= '[' [expressions] ']'
set ::= '{' [expressions] '}'

(* dictionay literal *)
dict ::= '{' [kvs] '}'
kvs ::= kv (',' kv)*
kv ::= expression ':' expression

(* return statement *)
return_stmt ::= 'return' [expression]

(* raise statement *)
raise_stmt ::= 'raise' expression

(* assert statement *)
assert_stmt ::= 'assert' expression [',' expression]

(* compound statement *)
compound_stmt ::= func_def
                | class_def
                | if_stmt
                | for_stmt
                | match_stmt

(* function definition *)
func_def ::= 'func' identifier '(' [params] ')' '{' block* '}'
params ::= param (',' param)*
param ::= identifier ['=' expression]

(* class definition *)
class_def ::= 'class' identifier '{' block* '}'

(* if-elif-else statement *)
if_stmt ::= 'if' expression '{' block* '}' elif_stmt* [else_stmt]
elif_stmt ::= 'elif' expression '{' block* }'
else_stmt ::= 'else' '{' block* '}'

(* for statement *)
for_stmt ::= 'for' targets 'in' expressions '{' block* '}'

(* match statement *)
match_stmt ::= 'match' expression '{' case_block+ [def_block] '}'
case_block ::= 'case' expression '{' block* '}'
def_block ::= 'default' '{' block* '}'

```

## Example

```python
# example for DNS parser

PROTOCOL_LIST = "998[DNS]"

func main() {

}
```

## Loader

```c++



```
