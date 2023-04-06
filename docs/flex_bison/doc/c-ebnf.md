# The syntax of C in Backus-Naur form

```EBNF
(* conf form language, version 0.1 *)

translation-unit = {external-declaration};

external-declaration = function-definition
                     | declaration;

function-definition = declaration-specifiers, declarator, [declaration-list], compound-statement;

declaration = declaration-specifiers, [init-declarator-list], ';'
            | static-assert-declaration
            | ';';

declaration-specifiers = declaration-specifier, {declaration-specifier};

declaration-specifier = type-specifier
                      | type-qualifier
                      | function-specifier
                      | alignment-specifier;

declarator = [pointer], direct-declarator;

declaration-list = declaration, {declaration};

compound-statement = '{', {declaration-or-statement}, '}';

declaration-or-statement = declaration | statement;

init-declarator-list = init-declarator, {',', init-declarator};

init-declarator = declarator, ['=', initializer];

static-assert-declaration = 'Assert', '(', constant-expression, ',', string-literal, ')', ';';

type-specifier = 'void'
               | 'char'
               | 'short'
               | 'int'
               | 'long'
               | 'float'
               | 'double'
               | 'signed'
               | 'unsigned'
               | '_Bool'
               | atomic-type-specifier
               | struct-or-union-specifier
               | enum-specifier;

type-qualifier = 'const'
               | 'restrict'
               | 'volatile'
               | '_Atomic';

function-specifier = 'inline'
                   | '_Noreturn';

alignment-specifier = '_Alignas', '(', type-name, ')'
                    | '_Alignas', '(', constant-expression, ')';

pointer = '*', [type-qualifier-list], [pointer];

direct-declarator = identifier
                  | '(', declarator, ')'
                  | direct-declarator, '[', ['*'], ']'
                  | direct-declarator, '[', 'static', [type-qualifier-list], assignment-expression, ']'
                  | direct-declarator, '[', type-qualifier-list, ['*'], ']'
                  | direct-declarator, '[', type-qualifier-list, ['static'], assignment-expression, ']'
                  | direct-declarator, '[', assignment-expression, ']'
                  | direct-declarator, '(', parameter-type-list, ')'
                  | direct-declarator, '(', identifier-list, ')'
                  | direct-declarator, '(', ')';

identifier-list = identifier, {',', identifier};

initializer-list = designative-initializer, {',', designative-initializer};

designative-initializer = [designation], initializer;

initializer = '{', initializer-list, [','], '}'
            | assignment-expression;

constant-expression = conditional-expression;  (* with constraints *)

atomic-type-specifier = '_Atomic', '(', type-name, ')';

struct-or-union-specifier = struct-or-union, '{', struct-declaration-list, '}'
                          | struct-or-union, identifier, ['{', struct-declaration-list, '}'];

struct-or-union = 'struct'
                | 'union';

struct-declaration-list = struct-declaration, {struct-declaration};

struct-declaration = specifier-qualifier-list, ';'     (* for anonymous struct/union *)
                   | specifier-qualifier-list, struct-declarator-list, ';'
                   | static-assert-declaration;

enum-specifier = 'enum', '{', enumerator-list, [','], '}'
               | 'enum', identifier, ['{', enumerator-list, [','], '}'];

enumerator-list = enumerator, {',', enumerator};

(* NOTE: Please define enumeration-constant for identifier inside enum { ... }. *)
enumerator = enumeration-constant, ['=', constant-expression];

enumeration-constant = identifier;

type-name = specifier-qualifier-list, [abstract-declarator];

specifier-qualifier-list = specifier-qualifier, {specifier-qualifier};

specifier-qualifier = type-specifier | type-qualifier;

abstract-declarator = pointer, [direct-abstract-declarator]
                    | direct-abstract-declarator;

direct-abstract-declarator = '(', abstract-declarator, ')'
                           | '(', parameter-type-list, ')'
                           | '(', ')'
                           | '[', ['*'], ']'
                           | '[', 'static', [type-qualifier-list], assignment-expression, ']'
                           | '[', type-qualifier-list, [['static'], assignment-expression], ']'
                           | '[', assignment-expression, ']'
                           | direct-abstract-declarator, '[', ['*'], ']'
                           | direct-abstract-declarator, '[', 'static', [type-qualifier-list], assignment-expression, ']'
                           | direct-abstract-declarator, '[', type-qualifier-list, [['static'], assignment-expression], ']'
                           | direct-abstract-declarator, '[', assignment-expression, ']'
                           | direct-abstract-declarator, '(', parameter-type-list, ')'
                           | direct-abstract-declarator, '(', ')';

struct-declarator-list = struct-declarator, {',', struct-declarator};

type-qualifier-list = type-qualifier, {type-qualifier};

parameter-type-list = parameter-list, [',', '...'];

struct-declarator = ':', constant-expression
                  | declarator, [':', constant-expression];

assignment-operator = '='
                    | '*='
                    | '/='
                    | '%='
                    | '+='
                    | '-='
                    | '<<='
                    | '>>='
                    | '&='
                    | '^='
                    | '|=';

parameter-list = parameter-declaration, {',', parameter-declaration};

parameter-declaration = declaration-specifiers, [declarator | abstract-declarator];

expression = assignment-expression, {',', assignment-expression};

assignment-expression = conditional-expression
                      | unary-expression, assignment-operator, assignment-expression;

conditional-expression = logical-or-expression, ['?', expression, ':', conditional-expression];

logical-or-expression = logical-and-expression, {'||', logical-and-expression};

logical-and-expression = inclusive-or-expression, {'&&', inclusive-or-expression};

inclusive-or-expression = exclusive-or-expression, {'|', exclusive-or-expression};

exclusive-or-expression = and-expression, {'^', and-expression};

and-expression = equality-expression, {'&', equality-expression};

equality-expression = relational-expression, {('==' | '!='), relational-expression};

relational-expression = shift-expression, {('<' | '>' | '<=' | '>='), shift-expression};

shift-expression = additive-expression, {('<<' | '>>'), additive-expression};

additive-expression = multiplicative-expression, {('+' | '-'), multiplicative-expression};

multiplicative-expression = cast-expression, {('*' | '/' | '%'), cast-expression};

cast-expression = unary-expression
                | '(', type-name, ')', cast-expression;

unary-expression = postfix-expression
                 | ('++' | '--'), unary-expression
                 | unary-operator, cast-expression;

postfix-expression = primary-expression
                   | postfix-expression, '[', expression, ']'
                   | postfix-expression, '(', [argument-expression-list], ')'
                   | postfix-expression, ('.' | '->'), identifier
                   | postfix-expression, ('++' | '--')
                   | '(', type-name, ')', '{', initializer-list, [','], '}';

unary-operator = '+'
               | '-'
               | '~'
               | '!';

primary-expression = identifier
                   | constant
                   | string
                   | '(', expression, ')';

argument-expression-list = assignment-expression, {',', assignment-expression};

constant = integer-constant
         | character-constant
         | floating-constant
         | enumeration-constant;

string = string-literal;

designation = designator-list, '=';

designator-list = designator, {designator};

designator = '[', constant-expression, ']'
           | '.', identifier;

statement = labeled-statement
          | compound-statement
          | expression-statement
          | selection-statement
          | iteration-statement;

labeled-statement = 'case', constant-expression, ':', statement
                  | 'default', ':', statement;

expression-statement = [expression];

selection-statement = 'if', '(', expression, ')', statement, 'else', statement
                    | 'if', '(', expression, ')', statement
                    | 'switch', '(', expression, ')', statement;

 iteration-statement = 'while', '(', expression, ')', statement
                     | 'for', '(', [expression], ';', [expression], ';', [expression], ')', statement
                     | 'for', '(', declaration, [expression], ';', [expression], ')', statement;

```
