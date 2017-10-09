%{
  #include <stdio.h>

  #define SHOW_COORDINATES 1

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"

  void showTokenPosition();

  int yyerror(char* s);
  int yylex();

  void onTokenParsed() {
    if(SHOW_COORDINATES) {
      showTokenPosition();
    }
  }
%}

%union {
  int intVal;
  char* stringVal;
}

%locations

%token  CLASS
%token  INT
%token  BOOLEAN
%token  THIS
%token  NEW
%token  PUBLIC
%token  PRIVATE
%token  EXTENDS
%token  RETURN

%token  SEMI
%token  DOT
%token  COMMA
%token  ASSIGN

%token  L_PAREN
%token  R_PAREN
%token  L_SQUARE
%token  R_SQUARE
%token  L_BRACE
%token  R_BRACE

%token  IF
%token  ELSE
%token  WHILE

%token  AND
%token  LESS
%token  PLUS
%token  MINUS
%token  MULT
%token  MOD
%token  OR
%token  BANG

%token  LENGTH
%token  PRINT_LINE
%token  MAIN

%token  <stringVal>  ID
%token  <intVal>  INTEGER_NUMBER
%token  <intVal>  BOOLEAN_VALUE

%token  ERROR

%%

tokens: token
      | tokens token
      ;
token :  CLASS                         { printf( "CLASS " ); onTokenParsed(); }
      |  INT                           { printf( "INT " ); onTokenParsed(); }
      |  BOOLEAN                       { printf( "BOOLEAN " );onTokenParsed(); }
      |  BOOLEAN_VALUE                 {
                                         printf( "BOOLEAN_VALUE(%s) ", yylval.intVal == 1 ? "TRUE" : "FALSE" );
                                         onTokenParsed();
                                       }
      |  THIS                          { printf( "THIS " ); onTokenParsed(); }
      |  NEW                           { printf( "NEW " ); onTokenParsed(); }
      |  PUBLIC                        { printf( "PUBLIC " ); onTokenParsed(); }
      |  PRIVATE                       { printf( "PRIVATE " ); onTokenParsed(); }
      |  EXTENDS                       { printf( "EXTENDS " ); onTokenParsed(); }
      |  RETURN                        { printf( "RETURN " ); onTokenParsed(); }
      |  SEMI                          { printf( "SEMI " ); onTokenParsed(); }
      |  DOT                           { printf( "DOT " ); onTokenParsed(); }
      |  COMMA                         { printf( "COMMA " ); onTokenParsed(); }
      |  ASSIGN                        { printf( "ASSIGN " ); onTokenParsed(); }
      |  L_PAREN                       { printf( "L_PAREN " ); onTokenParsed(); }
      |  R_PAREN                       { printf( "R_PAREN " ); onTokenParsed(); }
      |  L_SQUARE                      { printf( "L_SQUARE " ); onTokenParsed(); }
      |  R_SQUARE                      { printf( "R_SQUARE " ); onTokenParsed(); }
      |  L_BRACE                       { printf( "L_BRACE " ); onTokenParsed(); }
      |  R_BRACE                       { printf( "R_BRACE " ); onTokenParsed(); }
      |  IF                            { printf( "IF " ); onTokenParsed(); }
      |  ELSE                          { printf( "ELSE " ); onTokenParsed(); }
      |  WHILE                         { printf( "WHILE " ); onTokenParsed(); }
      |  AND                           { printf( "AND " ); onTokenParsed(); }
      |  LESS                          { printf( "LESS " ); onTokenParsed(); }
      |  PLUS                          { printf( "PLUS " ); onTokenParsed(); }
      |  MINUS                         { printf( "MINUS " ); onTokenParsed(); }
      |  MULT                          { printf( "MULT " ); onTokenParsed(); }
      |  MOD                           { printf( "MOD " ); onTokenParsed(); }
      |  OR                            { printf( "OR " ); onTokenParsed(); }
      |  BANG                          { printf( "BANG " ); onTokenParsed(); }
      |  LENGTH                        { printf( "LENGTH " ); onTokenParsed(); }
      |  PRINT_LINE                    { printf( "PRINT_LINE " ); onTokenParsed(); }
      |  MAIN                          { printf( "MAIN " ); onTokenParsed(); }
      |  ID                            { printf( "ID(%s) ", yylval.stringVal ); onTokenParsed(); }
      |  INTEGER_NUMBER                { printf( "INTEGER_NUMBER(%d) ", yylval.intVal ); onTokenParsed(); }
      |  ERROR                         {
                                          printf( NF_RED "\nFucking bullshit at %d,%d\n" NF_RESET,
                                              yylloc.first_line, yylloc.first_column );
                                          onTokenParsed();
                                       }
      ;
%%

void showTokenPosition() {
  printf( "%d,%d:%d ", yylloc.first_line, yylloc.first_column,
      yylloc.last_column );
}

int yyerror(char* s) {
  printf( NF_RED "Fucking bullshit %s at %d,%d:%d\n" NF_RESET, s, yylloc.first_line, yylloc.first_column,
      yylloc.last_column);
}

int main(void) {
  yyparse();
}
