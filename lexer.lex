%{
  #include <stdlib.h>

  #define NICE_FORMATTING 0

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"
  #define YY_USER_ACTION upgradePosition();

  int lineNumber = 1;
  int columnNumber = 1;
  long formatCounter = 0;
  void upgradePosition() {
    if( *yytext == '\n' ) {
      lineNumber += 1;
      columnNumber = 1;
    } else {
      columnNumber += strlen(yytext);
    }
    formatCounter += 1;
  }

  void showPosition() {
    printf("%d,%d ", lineNumber, columnNumber - strlen(yytext));
  }
%}

SPACE                           [ \t\n]+
LETTER                          [A-Za-z_]
POSITIVE                        [1-9]
ZERO                            0
DIGIT                           {ZERO}|{POSITIVE}

CLASS                           class
INT                             int
BOOLEAN                         boolean

TRUE                            true
FALSE                           false

THIS                            this
NEW                             new
PUBLIC                          public
PRIVATE                         private
EXTENDS                         extends
RETURN                          return

SEMI                            ";"
DOT                             "."
COMMA                           ","
ASSIGN                          "="

L_PAREN                         "("
R_PAREN                         ")"
L_SQUARE                        "["
R_SQUARE                        "]"
L_BRACE                         "{"
R_BRACE                         "}"

IF                              if
ELSE                            else
WHILE                           while

AND                             "&&"
LESS                            "<"
PLUS                            "+"
MINUS                           "-"
MULT                            "*"
MOD                             "%"
OR                              "||"
BANG                            "!"

PRINT_LINE                      "System.out.println"
MAIN                            static[ \t\n]+void[ \t\n]+main
STRING                          String({SPACE})?{L_SQUARE}({SPACE})?{R_SQUARE}
STRING_ARG                      {STRING}{SPACE}{LETTER}({LETTER}|{DIGIT})*

ID                              {LETTER}({LETTER}|{DIGIT})*
INTEGER_NUMBER                  {POSITIVE}({DIGIT})*|{ZERO}


%%
{CLASS}                         printf( "CLASS " ); showPosition();
{INT}                           printf( "INT " ); showPosition();
{BOOLEAN}                       printf( "BOOLEAN " ); showPosition();

{TRUE}                          printf( "TRUE " ); showPosition();
{FALSE}                         printf( "FALSE " ); showPosition();

{THIS}                          printf( "THIS " ); showPosition();
{NEW}                           printf( "NEW " ); showPosition();
{PUBLIC}                        printf( "PUBLIC " ); showPosition();
{PRIVATE}                       printf( "PRIVATE " ); showPosition();
{EXTENDS}                       printf( "EXTENDS " ); showPosition();
{RETURN}                        printf( "RETURN " ); showPosition();

{SEMI}                          printf( "SEMI " ); showPosition();
{DOT}                           printf( "DOT " ); showPosition();
{COMMA}                         printf( "COMMA " ); showPosition();
{ASSIGN}                        printf( "ASSIGN " ); showPosition();

{L_PAREN}                       printf( "L_PAREN " ); showPosition();
{R_PAREN}                       printf( "R_PAREN " ); showPosition();
{L_SQUARE}                      printf( "L_SQUARE " ); showPosition();
{R_SQUARE}                      printf( "R_SQUARE " ); showPosition();
{L_BRACE}                       printf( "L_BRACE " ); showPosition();
{R_BRACE}                       printf( "R_BRACE " ); showPosition();

{IF}                            printf( "IF " ); showPosition();
{ELSE}                          printf( "ELSE " ); showPosition();
{WHILE}                         printf( "WHILE " ); showPosition();

{AND}                           printf( "AND " ); showPosition();
{LESS}                          printf( "LESS " ); showPosition();
{PLUS}                          printf( "PLUS " ); showPosition();
{MINUS}                         printf( "MINUS " ); showPosition();
{MULT}                          printf( "MULT " ); showPosition();
{MOD}                           printf( "MOD " ); showPosition();
{OR}                            printf( "OR " ); showPosition();
{BANG}                          printf( "BANG " ); showPosition();

{PRINT_LINE}                    printf( "PRINT_LINE " ); showPosition();
{MAIN}                          printf( "MAIN " ); showPosition();

{ID}                            printf( "ID(%s) ", yytext ); showPosition();
{INTEGER_NUMBER}                printf( "INTEGER_NUMBER(%s) ", yytext ); showPosition();

"//".*
{STRING_ARG}
\n                              if( NICE_FORMATTING ) {
                                    printf("\n");
                                }
[ \t\r]+
<<EOF>>                         {
                                    printf( "EOF\n" );
                                    return 0;
                                }

.	printf( NF_RED "\nFucking bullshit at %d,%d\n" NF_RESET, lineNumber, columnNumber );
