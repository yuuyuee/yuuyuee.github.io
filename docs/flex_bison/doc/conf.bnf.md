# The syntax of CONF in Backus-Naur form

```EBNF
(* The syntax of Conf in Backus-Naur Form. *)
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
that is conventional in mathematics. like as expression "a < b < c" is
equivalent to "a < b and b < c"
*)

program ::= [statements]

statements ::= statement+

statement ::= compound_statement | simple_statements

simple_statements ::= simple_statement+ ';'

(* Simple Statement*)

(* assignment must precede expression, else parsing a simple assignment
   will throw a syntax error *)
simple_statement ::= assignment_statement
                   | expression
                   | return_statement
                   | raise_statement
                   | assert_statement
                   | 'break'
                   | 'continue'

assignment_statement ::= (target_list '=')+ (expressions)
target_list ::= target (',' target)* [',']
target ::= NAME
         | '(' [target_list] ')'
         | '[' [target_list] ']'
         | attributeref
         | subscription
         | slicing

expressions ::= expression (',' expression)+ [',']
              | expression ','
              | expression

expression ::= disjunction 'if' disjunction 'else' expression
             | disjunction

disjunction ::= conjunction ('or' conjunction)+
              | conjunction

conjunction ::= inversion ('and' inversion)+

inversion ::= 'not' inversion | comparison

(* Comparison operators *)
comparison ::= bitwise_or compare_op_bitwise_or_pair+ | bitwise_or
compare_op_bitwise_or_pair ::= eq_bitwise_or
                             | noteq_bitwise_or
                             | lte_bitwise_or
                             | lt_bitwise_or
                             | gte_bitwise_or
                             | gt_bitwise_or
                             | notin_bitwise_or
                             | in_bitwise_or
                             | isnot_bitwise_or
                             | is_bitwise_or

eq_bitwise_or ::= '==' bitwise_or
noteq_bitwise_or ::= ('!=') bitwise_or
lte_bitwise_or ::= ('<=') bitwise_or
lt_bitwise_or ::= ('<') bitwise_or
gte_bitwise_or ::= ('>=') bitwise_or
gt_bitwise_or ::= ('>') bitwise_or
notin_bitwise_or ::= ('not in') bitwise_or
in_bitwise_or ::= ('in') bitwise_or
isnot_bitwise_or ::= ('is not') bitwise_or
is_bitwise_or ::= ('is') bitwise_or

(* Bitwise operators *)
bitwise_or ::= bitwise_or '|' bitwise_xor
             | bitwise_xor

bitwise_xor ::= bitwise_xor '^' bitwise_and
              | bitwise_and

bitwise_and ::= bitwise_and '&' shift_expr
              | shift_expr

shift_expr ::= shift_expr '<<' sum
             | shift_expr '>>' sum
             | sum

(* Arithmetic operators *)
sum ::= sum '+' term
      | sum '-' term
      | term

term ::= term '*' factor
       | term '/' factor
       | term '//' factor
       | term '%' factor
       | factor

factor ::= '+' factor
         | '-' factor
         | '~' factor
         | power

power ::= primary '**' factor
        | primary

(* Primary elements *)
primary ::= primary '.' NAME
          | primary '(' [arguments] ')'
          | primary '[' slices ']'
          | atom

slices ::= [expression] ':' [expression] [':' [expression]]
         | expression

atom ::= NAME
       | 'True'
       | 'False'
       | 'None'
       | strings
       | NUMBER
       | tuple
       | list
       | dict
       | set

(* Literals *)
string ::= STRING
strings ::= string+

list ::= '[' [expressions] ']'
tuple ::= '(' [expressions] ')'
set ::= '{' [expressions] '}'

(* Dicts *)
dict ::= '{' [kvpairs] '}'
kvpairs ::= kvpair | kvpairs ',' kvpair
kvpair ::= expression ':' expression

(* function call arguments *)
arguments ::= argumetns ',' expression | expression

(* Return statement *)
return_statement ::= 'return' [expression]

(* Raise statement *)
raise_statement ::= 'raise' expression

(* Assert statement *)
assert_statement ::= 'assert' expression [',' expression]

(* Compound Statement *)
compound_stmt ::= function_def
                | if_statement
                | class_def
                | for_statement
                | match_statement

(* Class definition *)
class-definition ::= 'class' NAME '{' block '}'

(* Function definition *)
function-definition ::= 'func' NAME '(' [NAME] ')' '{' block '}'

block ::= statements | simple_statements

(* If statement *)
if_statement ::= 'if' expression '{' block '}' elif_statement
               | 'if' expression '{' block '}' [else_block]
               | 'if' expression '{' block '}'

elif_statement ::= 'elif' expression '{' block }' elif_statement
                 | 'elif' expression '{' block }' [else_block]
                 | 'elif' expression '{' block }'

else_block ::= 'else' '{' block '}'

(* For statement *)
for_statement ::= 'for' target_list 'in' expression '{' block '}'

(* Match statement *)
match_statement ::= 'match' expression '{' case_block+ '}'
case_block ::= 'case' expression '{' block '}' | 'default' '{' block '}'


```
