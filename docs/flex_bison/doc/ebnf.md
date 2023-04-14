# Backus Naur Form

Concept | Description
-|-
\<symbol\> | Angle brackets mean a non-terminal
symbol | Symbols without angle brackets are terminals
@symbol | Symbols can be removed
::= | means is defined as (some variants use "::=" instead)
\| | alternation symbol, means "or"
[] | indicate optional symbols
{} | indicate repetition
() | indicate grouping of grammatical structures to change priority and improve readability
space | space or nothing indicate sequence

## Summary BNF

1. BNF uses following notations
   1. Non-terminals enclosed in < and >
   2. Rules written as X ::= Y
   3. X is LHS of rule and can only be a non-terminal symbol
   4. Y is RHS of rule, Y can be
      1. terminal symbol
      2. non-terminal symbol
      3. concatenation of terminal and non-terminal symbol
      4. a set of strings separated by alternatioin symbol
   Example: \<S\> ::= a \<S\> | a

## Extended BNF

1. EBNF adding more meta-notation => shorter productions
2. Non-terminals begin with uppercase letters (discatd <>)
3. Repetitions(0 or more) are enclosed in {}
4. Options(0 or one) are enclosed in []
   stmt ::= if Cond then Stmt
          | if Cond then Stmt else Stmt
  => stmt ::= if Cond then Stmt [else Stmt]
5. Use () to group items together
   expr ::= item {+ item} | item {- item}
   => expr ::= item {(+|-) item}
6. Terminals that are grammar symbols are enclosed in quotes

## BNF

```BNF
(* BNF recursion example *)
<number> ::= <digit> | <number> <digit>
<digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0

(* BNF precedence example *)
<expression> ::= <expression> + <term>
               | <expression> - <term>
               | <term>
<term> ::= <term> * <factor>
         | <term> / <factor>
         | <factor>

(* BNF example *)
<number> ::= <digit> | <number> <digit>
<digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0
<while loop> ::= while (<condition>) <statement>
<assignment statement> ::= <variable> = <expression>
<statement list> ::= <statement> | <statement list> <statement>
<unsigned integer> ::= <digit> | <unsigned integer><digit>
<expression> ::= <expression> + <term> | <expression> - <term> | <term>
<term> ::= <term> * <factor> | <term> / <factor> | <factor>
<factor> ::= <primary> ^ <factor> | <primary>
<primay> ::= <primary> | <element>
<element> ::= (<expression>) | <variable> | <number>
```

## EBNF

Syntax | Description
-|-
\* | means 0 or more occurrences
\+ | means 1 or more occurrences
? | means 0 or 1 occurrences, sometimes [...] used instead
\- | except symbol
, | concatenate symbol
= | defining symbol
; | terminator symbol

```EBNF
expression ::= term {('+'|'-')} term
term ::= factor {('*' | '/') factor}
factor ::= number | '(' expression ')'
```

```EBNF
(*
  This example defines Extended BNF informally.
  Many of the syntax rules include a comment to explain their meaning;
  inside a comment a meta identifier is enclosed in angle brackets
  < and confusion with > to avoid similar English words.
  The non-terminal symbols <letter>, <decimal digit> and <character> are not defined.
  The position of <comments> is stated in a comment but not formally defined.
*)

syntax = syntax rule, {syntax rule};

(*
  A <syntax rule> defines the sequences of symbols represented by a <meta identifier>
*)
syntax rule = meta identify, '=', definitions list, ';'

(* | separates alternative <single definitions> *)
definitions list = ingle definition, {'|', single definition}

(* , separates ssuccessive <terms> *)
single definition = term, {',', term}

(*
  A <term> represents any sequence of symbols that is defined by the <factor> but not defined by the <exception>
*)
term = factor, ['-', exception]

(*
  A <factor> may be used as an <exception> if it could be replaced by a <factor> containing no <meta identifiers>
*)
exception = factor

(* The <integer> specifies the number of repetitions of the <primary> *)
factor = [integer, '*'], primary

primary = optional sequence | repeated sequence | special sequence | grouped sequence
          meta identifier | terminal string | empty;
empty = ;

(* The brackets [ and ] enclose symbols which are optional *)
optional sequence = '[', definitions list, ']'

(* The brackets { and } enclose symbols which may be repeated any number of times *)
repeated sequence = '{', definitions list, '}'

(* The brackets ( and ) allow any <definitions list> to be a <primary> *)
grouped sequence = '(', definitions list, ')'

(*
  A <terminal string> represents the <characters> between the quote symbols '-' or "-"
*)
terminal string = "'", character - "'", {character - "-"}, "'"
    | '"', character - '"', {character = '"'}, '"'

(*
  A <meta identifier> is the name of a syntactic element of the language begin defined
*)
meta identifier = letter, {letter | decimal digit}

integer = decimal digit, {decimal digit}

(* The meaning of a <special sequence> is not defined in the standard metalanguage *)
special sequence = '?', (character - '?'), '?'

(*
  A comment is allowed anywhere outside a <terminal string>, <meta identifier>, <integer> or <special sequence>
*);
comment = '(*', {comment symbol}, '*)'

comment symbol = comment | terminal string | special sequence | characters


(* THIS IS EXTENDED BNF DEFINED INFORMALLY. *)
syntax = syntax rule, (: syntax rule :).
syntax rule = meta identifier, '=', definitions list, '.'.
definitions list = single definition, (: '/', single definition :).
single definition = term, (: ',', term :).
term = factor, (/ '-', exception /).
exception = factor.
factor = (/ integer, '*' /), primary.
primary = optional sequence / repeated sequence / special sequence
          / grouped sequence / meta identifier / terminal / empty.
empty = .
optional sequence = '(/', definitions list,  '/)'.
repeated sequence = '(:', definitions list, ':)'.
grouped sequence = '(', definitions list, ')'.
terminal = "'", character - "'", (: character - "'" :), "'"
          / '"', character - '"', (: character - '"' :), '"'.
meta identifier = letter, (: letter / digit :).
integer = digit, (: digit :).
special sequence = '?', (: character - '?' :), '?'.
comment = '(*', (: comment symbol :), '*)'.
comment symbol = comment / terminal / special sequence / character


```

## BNF vs EBNF

* Grammar for decimal numbers in plain BNF:

```BNF
(* example 1*)
<expr> ::= '-' <num> | <num>
<num> ::= <digits> | <digits> '.' <digits>
<digits> ::= <digits> | <digit> <digits>
<digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

(* example 2 *)
<expr> ::= <digits> | empty

(* example 3*)
<id> ::= <letter> | <id><letter> | <id><digit>
```

* Some grammar in EBNF:

```EBNF
(* example 1*)
<expr> := '-'? <digit>+ ('.' <digit>+)?
<digit> := '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

(* example 2 *)
<expr> ::= <digits>*

(* example 3*)
<id> ::= <letter> (<letter> | <digit>)*
```

* Look for recursion in grammar

```EBNF
(* BNF *)
A ::= a A | B

(* EBNF *)
A ::= a {a} B
```

* Look for common string that can be factored out with grouping and options

```EBNF
(* BNF *)
A ::= a B | a

(* EBNF *)
A ::= a [B]
```
