%{
  #include <stdlib.h>

  #define NICE_FORMATTING 1

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"
  #define YY_USER_ACTION upgradePosition();

  int lineNumber = 0;
  int columnNumber = 0;
  long formatCounter = 0;
  void upgradePosition() {
    if( *yytext == '\n' ) {
      lineNumber += 1;
      columnNumber = 1;
    } else {
      columnNumber += 1;
    }
    formatCounter += 1;
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
STRING                          String

ID                              {LETTER}({LETTER}|{DIGIT})*
INTEGER_NUMBER                  {POSITIVE}({DIGIT})*|{ZERO}


%%
{CLASS}                         printf( "CLASS " );
{INT}                           printf( "INT " );
{BOOLEAN}                       printf( "BOOLEAN " );

{TRUE}                          printf( "TRUE " );
{FALSE}                         printf( "FALSE " );

{THIS}                          printf( "THIS " );
{NEW}                           printf( "NEW " );
{PUBLIC}                        printf( "PUBLIC " );
{PRIVATE}                       printf( "PRIVATE " );
{EXTENDS}                       printf( "EXTENDS " );
{RETURN}                        printf( "RETURN " );

{SEMI}                          printf( "SEMI " );
{DOT}                           printf( "DOT " );
{COMMA}                         printf( "COMMA " );
{ASSIGN}                        printf( "ASSIGN " );

{L_PAREN}                       printf( "L_PAREN " );
{R_PAREN}                       printf( "R_PAREN " );
{L_SQUARE}                      printf( "L_SQUARE " );
{R_SQUARE}                      printf( "R_SQUARE " );
{L_BRACE}                       printf( "L_BRACE " );
{R_BRACE}                       printf( "R_BRACE " );

{IF}                            printf( "IF " );
{ELSE}                          printf( "ELSE " );
{WHILE}                         printf( "WHILE " );

{AND}                           printf( "AND " );
{LESS}                          printf( "LESS " );
{PLUS}                          printf( "PLUS " );
{MINUS}                         printf( "MINUS " );
{MULT}                          printf( "MULT " );
{MOD}                           printf( "MOD " );
{OR}                            printf( "OR " );
{BANG}                          printf( "BANG " );

{PRINT_LINE}                    printf( "PRINT_LINE " );
{MAIN}                          printf( "MAIN " );
{STRING}                        printf( "STRING " );

{ID}                            printf( "ID(%s) ", yytext );
{INTEGER_NUMBER}                printf( "INTEGER_NUMBER(%s) ", yytext );

"//".*
\n                              if( NICE_FORMATTING ) {
                                    printf("\n");
                                }
[ \t\r]+
<<EOF>>                         {
                                    printf( "EOF\n" );
                                    return 0;
                                }

.	printf( NF_RED "\nFucking bullshit at %d,%d\n" NF_RESET, lineNumber, columnNumber );
