%{
  #include <stdlib.h>

  #include "SyntaxTree/TreeTypes.h"
  #include "tokens.h"

  #define NICE_FORMATTING 0
  #define NICE_COORDINATES 0

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"
  #define YY_USER_ACTION upgradePosition(yytext);

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

  void upgradePosition(const char* yytext) {
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

  void showPosition(const char* yytext) {
    printf( "%d,%d ", lineNumber, columnNumber - strlen(yytext) );
  }

  void onProcessed(const char* yytext) {
    if( NICE_COORDINATES ) {
        showPosition(yytext);
    }
  }
%}

%option c++

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
{CLASS}                         {  onProcessed(yytext); return CLASS; }
{INT}                           {  onProcessed(yytext); return INT; }
{BOOLEAN}                       {  onProcessed(yytext); return BOOLEAN; }

{BOOLEAN_VALUE}                 {
                                   yylval.intVal = strcmp("true", yytext) == 0 ? 1 : 0;
                                   onProcessed(yytext);
                                   return BOOLEAN_VALUE;
                                }

{THIS}                          {  onProcessed(yytext); return THIS; }
{NEW}                           {  onProcessed(yytext); return NEW; }
{PUBLIC}                        {  onProcessed(yytext); return PUBLIC; }
{PRIVATE}                       {  onProcessed(yytext); return PRIVATE; }
{EXTENDS}                       {  onProcessed(yytext); return EXTENDS; }
{RETURN}                        {  onProcessed(yytext); return RETURN; }

{SEMI}                          {  onProcessed(yytext); return SEMI; }
{DOT}                           {  onProcessed(yytext); return DOT; }
{COMMA}                         {  onProcessed(yytext); return COMMA; }
{ASSIGN}                        {  onProcessed(yytext); return ASSIGN; }

{L_PAREN}                       {  onProcessed(yytext); return L_PAREN; }
{R_PAREN}                       {  onProcessed(yytext); return R_PAREN; }
{L_SQUARE}                      {  onProcessed(yytext); return L_SQUARE; }
{R_SQUARE}                      {  onProcessed(yytext); return R_SQUARE; }
{L_BRACE}                       {  onProcessed(yytext); return L_BRACE; }
{R_BRACE}                       {  onProcessed(yytext); return R_BRACE; }

{IF}                            {  onProcessed(yytext); return IF; }
{ELSE}                          {  onProcessed(yytext); return ELSE; }
{WHILE}                         {  onProcessed(yytext); return WHILE; }

{AND}                           {  onProcessed(yytext); return AND; }
{LESS}                          {  onProcessed(yytext); return LESS; }
{PLUS}                          {  onProcessed(yytext); return PLUS; }
{MINUS}                         {  onProcessed(yytext); return MINUS; }
{MULT}                          {  onProcessed(yytext); return MULT; }
{MOD}                           {  onProcessed(yytext); return MOD; }
{OR}                            {  onProcessed(yytext); return OR; }
{BANG}                          {  onProcessed(yytext); return BANG; }

{LENGTH}                        {  onProcessed(yytext); return LENGTH; }
{PRINT_LINE}                    {  onProcessed(yytext); return PRINT_LINE; }
{MAIN}                          {  onProcessed(yytext); return MAIN; }

{ID}                            {
                                    yylval.stringVal = yytext;
                                    onProcessed(yytext);
                                    return ID;
                                }
{INTEGER_NUMBER}                {
                                    yylval.intVal = atoi(yytext);
                                    onProcessed(yytext);
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

%%


extern "C" {
  int yywrap();
}

int yyFlexLexer::yywrap() {
  return ::yywrap();
}

yyFlexLexer lexer;

extern "C" int yylex()
{
    return lexer.yylex();
}
