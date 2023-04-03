/* The flex input file consists of three sections, separated by
   a line with just %% in it.
   In the definitions section but not in the rules section, an unindented
   comment is also copied verbatim to the output. */

/**
 * definitions section
 */

/* In the definitions and rules sections, any indented text or text enclosed
  in "%{" and "%}" is copied verbatim to the output.
  "%{" and "%}" must appear unindented on lines by themselves. */

%{

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
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
%pointer

/* name definition form
  <name> <definition>
  The "name" is a word beginning with a letter or an underscore (_) followed
  by zero or more letters, digits, underscore  or dash.
  The "definition" is taken to begin at the firest non-whitespace character
  following the name and continuing to the end of the line.
  The definition can subsequently be referred to using "{name}", which will
  expand to "(definition)".
  example:
  DIGIT [0-9]

  {DIGIT}+"."{DIGIT}*
  is identical to:
  ([0-9])+"."([0-9])*
*/

DIGIT [0-9]
ALPHA [a-zA-Z]
WS [ \t]+

/**
 * rules section
 */

/* The rules section contains a series rules of the form:
  <pattern> <action>

  The "pattern" must be unindented and the action must begin on the same line.
  x         match the character 'x'
  .         any character (byte) except newline
  [xyz]     "character class" pattern matches either an x, y or z
            **Note that inside of a character class, all regular expression operators
            lose their special meaning except escape '\' and the character class
            operators '-', ']' and at the beginning of the class '^'.
            In addition to characters and ranges of characters, character classes
            can also contain character class exprission. These are expressions
            enclosed inside '[:' and ':]' delimiters (which themselves must appear
            between '[' and ']' of the character class.)
            [:alnum:] [:alpha:] [:blank:] [:cntrl:] [:digit:] [:graph:]
            [:lower:] [:print:] [:punct:] [:space:] [:upper:] [:xdigit:] **
  [abj-oZ]  "character class" matches an a, b, any letter from j through o, or Z
  [^A-Z]    "negated character class" mathces any character but those in the class
  [^A-Z\n]  any character except an uppercase letter or a newline
  r*        zero or more r
  r+        one or more r
  r?        zero or one r
  r{2, 5}   anywhere from 2 to 5 r
  r{2,}     2 or more r
  r{4}      exactly 4 r
  {name}    the expansion of the "name" definition (name)
  "[xyz]\"foo"  the literal string [xyz]"foo
  \x        if x is an 'a', 'b', 'f', 'n', 'r', 't' or 'v', then interpretation
            of \x, otherwise, a literal 'x' (used to escape operators such as *)
  \0        a NUL character
  \123      octal value 123
  \x2a      hexadecimal value 2a
  (r)       match an r, parentheses are used to override precedence
  rs        the regular expression r followed by the regular expression s;
            called "concatenation"
  r|s       either an r or s
  r/s       an r but only if it is followed by an s
  ^r        an r but only at the beginning of a line
  r$        an r but only at the end of a line
  <s>r      an r but only in start condition s
  <s1,s2>r  an r but only in any of start conditions s1, s2
  <*>r      an r in any start condition, even an exclusive one
  <<EOF>>   an end of file
  <s1,s2><<EOF>>    an end of file when in start condition s1 or s2

  The regular expressions listed above are grouped according to precedence,
  from highest precedence at the top to lowest at the bottom.
  Those grouped together have equal precedunce.

  Once the match is determined, the text corresponding to the match (token)
  is made available in the global character pointer "yytext" and its length
  in the global integer "yylength".

  Each pattern in a rule has a corresponding action, which can be any arbitrary
  C statement. The pattern ends at the first non-escaped whitespace character;
  the remainder of the line is its action. If the action is empty, then when
  the pattern is matched the input token is simply discarded. If the action
  contains a '{', then the action spans till the balancing '}' is found and
  the action may cross multiple lines. An action consisting solely of a vertical
  bar '|' means "same as the action for the next rule".
  There are a number of special directives which can be included within an action:
  ECHO - copies yytext to the scanner output
  BEGIN - followed by the name of a start condition places the scanner in the
          corresponding start condition.
  REJECT - directs the scanner to proceed on the "second best" rule which matched
           the input.
  yymore() - tells the scanner that the next time it mathces a rule, the corresponding
             token should be appended onto the current value of yytext rather than
             replacing it.
  yyless(n) - returns all but the first n characters of the current token back to
              the input stream, where they will be rescanned when the scanner looks
              for the next match. A argument of 0 to yyless will cause the entire
              current input string to be scanned again. Note that yyless is a macro
              and only be used int the flex input file, not from other source file.
  unput(c) - puts the character c back onto the input stream.
  input() - reads the next character from the input stream.
  YY_FLUSH_BUFFER - flushes the scanners internal buffer so that the next time the
                    scanner attempts to match a token, it will first refill the
                    buffer using YY_INPUT.
  yyterminate() - can be used in lieu of a return statement in an action. It terminates
                  the scanner and return a 0 to the scanners caller indicating all done.
                  It is an macro and may be redefined.
  */

/* In the rules section, any indented or "%{} text appearing before
  the first rule may be used to declare variables which are local to
  the scanning routine and after the declarations, code which is to
  be executed whenever the scanning routine is entered.
  Other indented or "%{}" text in the rule section is still copied to the
  output but its meaning is not well-defined and it may well cause compile-time
  error. */

/* start conditions
 * Any rule whose pattern is prefixed with "<sc>" will only be active then
 * the scanner is in the start condition named "sc".
 * <sc1,sc2,sc3>pattern action
 * Active only when the current start condition is either "sc1", "sc2" or "sc3".
 *
 *
 */

%%

%{
printf("start lexical analysis\n");
int new_lines = 0;
%}

"+" { /* plus operator */ }

("0x"|"0X")+[0-9a-fA-F]+ {
    printf("Saw an hexadeciaml digit: %s\n", yytext);
}

{DIGIT}+ {
    printf("Saw an integer: %s\n", yytext);
    /* return NUMBER; */
}

{WS}+ { /* ignore space */ }

"\n" { ++new_lines; /* return END; */ }

. {
    printf("Unrecognized symbol at line %d: %s\n",
            yylineno, yytext);
    /* exit(0); */
}

%% /* if user code section is missing, the second "%%" may be skipped. */


/**
 * user code section
 */

/* The presence of user code section is optional and it is simply copied to
  output verbatim. */

/* Generate scanner
 * yylex(): This definition may be changed by defining the "YY_DECL" macro.
 * Whenever "yylex" is called, it scans tokens from the global input file
 * "yyin". It continues until it either reaches an EOF or one of its actions
 * executes a return statement. If the scanner reaches an EOF, subsequently
 * calls are undefined unless either "yyin" is pointed at a new input file
 * or yyrestart() is called. "yyrestart()" takes one argument, a "FILE*"
 * pointer(which can be nil if you have set up "YY_INPUT" to scan from
 * a source other than yyin), and initializes "yyin" fro scanning from that
 * file. Note that "yyrestart()" does not reset the start condition to
 * INITIAL. If "yylext()" stops scannig due to executing a return statement
 * in one of the actions, the scanner may then be called again and it will
 * resume scanning where it left off.
 * By default, the scanner use block-reads rather than simple "getc()" call
 * to read characters from "yyin". The nature of how it gets its input can
 * be controlled by defining the "YY_INPUT(buf, result, max_size)" macro.
 * Its action is to place up to max_size characters in the character array
 * buf and return in the integer variable result either the number of
 * characters read or the constant YY_NULL to indicate EOF. The default
 * YY_INPUT reads from the global file pointer "yyin".
 * When the scanner receives an EOF indication from YY_INPUT, it then check
 * the "yywrap()" function. If "yywrap()" returns false (0) then it is assumed
 * that the function is gone ahead and set up yyin to point to another input
 * file, and scanning continues, otherwise, the scanner terminates and return
 * 0 to its caller. Note that either case, the start condition remains unchanged,
 * it does not revert to INITIAL. you may use %option noyywrap to do which
 * case the scanner behaves as though "yywrap()" returned 1 or link with "-lfl"
 * to obtain the default version of the routine.
 * Three routines are available for scanning from in-memory buffers rather than
 * files: "yy_scan_string()", "yy_scan_bytes" and "yy_scan_buffer()".
 * The scanner writes its "ECHO" output to the yyout global(default stdout),
 * which may be redefined by the euser simply by assigning it ot some other
 * FILE pointer.
 *                +---------------<---------------------+
 *                |                                     |
 * yylex() -> YY_INPUT() -> EOF ? <TRUE> -> yywrap() <FALSE>
 *                |               <FALSE>            <TRUE> -> return 0
 *                + <- scan ---------+
 *
 * - YY_USER_INIT calling in the first of yylex()
 * - YY_USER_ACTION callling in the first of each action
 */

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s files...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        const char* fname = argv[i];
        struct stat state;
        if (stat(fname, &state) != 0 || state.st_size == 0)
            continue;
        char* buf = malloc(state.st_size);
        YY_BUFFER_STATE bs = yy_scan_buffer(buf, state.st_size);
        while (yylex() != 0) {
            /* TODO */
        }
        yy_delete_buffer(bs);
        free(buf);
    }

    return 0;
}
