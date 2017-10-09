%{
  #include <stdlib.h>

  #include "tokens.h"

  #define NICE_FORMATTING 0
  #define NICE_COORDINATES 0

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"
  #define YY_USER_ACTION upgradePosition();

  int lineNumber = 1;
  int columnNumber = 1;

  void setStartPosition() {
    yylloc.first_line = lineNumber;
    yylloc.first_column = columnNumber;
  }

  void setEndPosition() {
    yylloc.last_line = lineNumber;
    yylloc.last_column = columnNumber;
  }

  void upgradePosition() {
    if( *yytext == '\n' ) {
      lineNumber += 1;
      columnNumber = 1;
      setStartPosition();
    } else {
      setStartPosition();
      columnNumber += strlen(yytext);
    }
    setEndPosition();
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

LENGTH                          length
PRINT_LINE                      "System.out.println"
MAIN                            static[ \t\n]+void[ \t\n]+main
STRING                          String({SPACE})?{L_SQUARE}({SPACE})?{R_SQUARE}
STRING_ARG                      {STRING}{SPACE}{LETTER}({LETTER}|{DIGIT})*

ID                              {LETTER}({LETTER}|{DIGIT})*
INTEGER_NUMBER                  {POSITIVE}({DIGIT})*|{ZERO}
BOOLEAN_VALUE                   {TRUE}|{FALSE}


%%
{CLASS}                         {  onProcessed(); return CLASS; }
{INT}                           {  onProcessed(); return INT; }
{BOOLEAN}                       {  onProcessed(); return BOOLEAN; }

{BOOLEAN_VALUE}                 {
                                   yylval.intVal = strcmp("true", yytext) == 0 ? 1 : 0;
                                   onProcessed();
                                   return BOOLEAN_VALUE;
                                }

{THIS}                          {  onProcessed(); return THIS; }
{NEW}                           {  onProcessed(); return NEW; }
{PUBLIC}                        {  onProcessed(); return PUBLIC; }
{PRIVATE}                       {  onProcessed(); return PRIVATE; }
{EXTENDS}                       {  onProcessed(); return EXTENDS; }
{RETURN}                        {  onProcessed(); return RETURN; }

{SEMI}                          {  onProcessed(); return SEMI; }
{DOT}                           {  onProcessed(); return DOT; }
{COMMA}                         {  onProcessed(); return COMMA; }
{ASSIGN}                        {  onProcessed(); return ASSIGN; }

{L_PAREN}                       {  onProcessed(); return L_PAREN; }
{R_PAREN}                       {  onProcessed(); return R_PAREN; }
{L_SQUARE}                      {  onProcessed(); return L_SQUARE; }
{R_SQUARE}                      {  onProcessed(); return R_SQUARE; }
{L_BRACE}                       {  onProcessed(); return L_BRACE; }
{R_BRACE}                       {  onProcessed(); return R_BRACE; }

{IF}                            {  onProcessed(); return IF; }
{ELSE}                          {  onProcessed(); return ELSE; }
{WHILE}                         {  onProcessed(); return WHILE; }

{AND}                           {  onProcessed(); return AND; }
{LESS}                          {  onProcessed(); return LESS; }
{PLUS}                          {  onProcessed(); return PLUS; }
{MINUS}                         {  onProcessed(); return MINUS; }
{MULT}                          {  onProcessed(); return MULT; }
{MOD}                           {  onProcessed(); return MOD; }
{OR}                            {  onProcessed(); return OR; }
{BANG}                          {  onProcessed(); return BANG; }

{LENGTH}                        {  onProcessed(); return LENGTH; }
{PRINT_LINE}                    {  onProcessed(); return PRINT_LINE; }
{MAIN}                          {  onProcessed(); return MAIN; }

{ID}                            {
                                    yylval.stringVal = yytext;
                                    onProcessed();
                                    return ID;
                                }
{INTEGER_NUMBER}                {
                                    yylval.intVal = atoi(yytext);
                                    onProcessed();
                                    return INTEGER_NUMBER;
                                }

"//".*
{STRING_ARG}
\n                              if( NICE_FORMATTING ) {
                                    printf("\n");
                                }
[ \t\r]+
<<EOF>>                         {
                                  if( NICE_FORMATTING ) {
                                      printf( "EOF\n" );
                                  }
                                    return 0;
                                }

.	return ERROR;
