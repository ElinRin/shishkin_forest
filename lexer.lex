%{
  #include <stdlib.h>

  #define NICE_FORMATTING 0
  #define NICE_COORDINATES 1

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"
  #define YY_USER_ACTION upgradePosition();

  int lineNumber = 1;
  int columnNumber = 1;
  void upgradePosition() {
    if( *yytext == '\n' ) {
      lineNumber += 1;
      columnNumber = 1;
    } else {
      columnNumber += strlen(yytext);
    }
  }

  void showPosition() {
    printf( "%d,%d ", lineNumber, columnNumber - strlen(yytext) );
  }

  void onProcessed() {
    if( NICE_COORDINATES ) {
        showPosition();
    }
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
{CLASS}                         printf( "CLASS " ); onProcessed();
{INT}                           printf( "INT " ); onProcessed();
{BOOLEAN}                       printf( "BOOLEAN " ); onProcessed();

{TRUE}                          printf( "TRUE " ); onProcessed();
{FALSE}                         printf( "FALSE " ); onProcessed();

{THIS}                          printf( "THIS " ); onProcessed();
{NEW}                           printf( "NEW " ); onProcessed();
{PUBLIC}                        printf( "PUBLIC " ); onProcessed();
{PRIVATE}                       printf( "PRIVATE " ); onProcessed();
{EXTENDS}                       printf( "EXTENDS " ); onProcessed();
{RETURN}                        printf( "RETURN " ); onProcessed();

{SEMI}                          printf( "SEMI " ); onProcessed();
{DOT}                           printf( "DOT " ); onProcessed();
{COMMA}                         printf( "COMMA " ); onProcessed();
{ASSIGN}                        printf( "ASSIGN " ); onProcessed();

{L_PAREN}                       printf( "L_PAREN " ); onProcessed();
{R_PAREN}                       printf( "R_PAREN " ); onProcessed();
{L_SQUARE}                      printf( "L_SQUARE " ); onProcessed();
{R_SQUARE}                      printf( "R_SQUARE " ); onProcessed();
{L_BRACE}                       printf( "L_BRACE " ); onProcessed();
{R_BRACE}                       printf( "R_BRACE " ); onProcessed();

{IF}                            printf( "IF " ); onProcessed();
{ELSE}                          printf( "ELSE " ); onProcessed();
{WHILE}                         printf( "WHILE " ); onProcessed();

{AND}                           printf( "AND " ); onProcessed();
{LESS}                          printf( "LESS " ); onProcessed();
{PLUS}                          printf( "PLUS " ); onProcessed();
{MINUS}                         printf( "MINUS " ); onProcessed();
{MULT}                          printf( "MULT " ); onProcessed();
{MOD}                           printf( "MOD " ); onProcessed();
{OR}                            printf( "OR " ); onProcessed();
{BANG}                          printf( "BANG " ); onProcessed();

{PRINT_LINE}                    printf( "PRINT_LINE " ); onProcessed();
{MAIN}                          printf( "MAIN " ); onProcessed();

{ID}                            printf( "ID(%s) ", yytext ); onProcessed();
{INTEGER_NUMBER}                printf( "INTEGER_NUMBER(%s) ", yytext ); onProcessed();

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
