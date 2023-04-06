# EBNF

1. Terminal symbols: 单个字符或字符串表示语法中的基本元素
2. Non-Terminal symbols: 表示一个递归定义的符号，可以由其他符号定义，最终会被展开为终止符号序列
3. Alternation: 使用竖线|表示或关系，指定两个或多个选项
4. Optional element: 用方括号[]表示可选元素，可以出现零次或多次
5. Repetition: 使用花括号{}表示重复元素，可以出现零次或多次
6. Grouping: 使用括号()表示语法结构分组，以改变优先级提高可读性
7. Concatenation: 空格或没有连接符表示连接符，并要求两个元素在源码中依次出现

```EBNF
expression ::= term {('+'|'-')} term
term ::= factor {('*' | '/') factor}
factor ::= number | '(' expression ')'
```

8. ::= 表示定义
9. '+' '-' '*' '/' 表示加减乘除运算符 （单引号或双引号）
10. number表示数字字面值

## 定义

1. 序列 sequence: 零个或多个项目组成的有序列表
2. 子序列 subsequence: 序列内的序列
3. 非终止符 Non-Terminal symbol: 被定义语言的语法部分
4. 元标识符 meta-identify: 非终止符的名称
5. 起始符 start symbol: 一个或多个语法规则定义的非终止符，但不出现在任何其他语法规则中
6. 句子 sentence: 表示起始符的符号序列
7. 终止符 Terminal symbol: 一个或多个字符序列构成的不可分割的语言元素
8. \* 重复符号
9. \- 排除符号
10. , 连接符号
11. | 定义分割符号
12. = 定义符号
13. ；终止符号
14. 'first-quote-symbol'
15. "second-quote-symbol"
16. (\*start-comment-symbol end-comment-symbol\*)
17. (start-group-symbol end-group-symbol)
18. [start-optional-symbol end-optional-symbol]
19. {start-repeat-symbol end-repeat-symbol}
20. ?special-sequence?
21. -symbol

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

