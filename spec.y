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

%token END 0

%token  ERROR

%left   MINUS PLUS
%left   MOD
%left   MULT

%%

/*tokens: token
      | tokens token
      ;*/
program : class_main class_seq END          { printf( "program: \n" ); onTokenParsed(); }
        | class_main END                    { printf( "program: \n" ); onTokenParsed(); }
        ;
class_main : CLASS id L_BRACE
              MAIN L_PAREN R_PAREN L_BRACE
              stm R_BRACE                   { printf( "main: \n" ); onTokenParsed(); }
            R_BRACE
        ;
class_seq : class_seq class                 { printf( "class_seq: \n" ); onTokenParsed(); }
        | class_seq class_ext               { printf( "class_seq: \n" ); onTokenParsed(); }
        | class                             { printf( "class_seq: \n" ); onTokenParsed(); }
        | class_ext                         { printf( "class_seq: \n" ); onTokenParsed(); }
        ;
class_ext : CLASS id EXTENDS id L_BRACE
          var_seq method_seq R_BRACE        { printf( "class_ext: \n" ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          var_seq R_BRACE                   { printf( "class_ext: \n" ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          method_seq EXTENDS id R_BRACE     { printf( "class_ext: \n" ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE R_BRACE { printf( "class_ext: \n" ); onTokenParsed(); }
        ;
class : CLASS id L_BRACE
          var_seq method_seq R_BRACE        { printf( "class: \n" ); onTokenParsed(); }
        | CLASS id L_BRACE
          var_seq R_BRACE                   { printf( "class: \n" ); onTokenParsed(); }
        | CLASS id L_BRACE
          method_seq R_BRACE                { printf( "class: \n" ); onTokenParsed(); }
        | CLASS id L_BRACE R_BRACE          { printf( "class: \n" ); onTokenParsed(); }
        ;
var_seq : var_seq var
        | var
        ;
var     : type id SEMI
        ;
method_seq : method_seq method
        | method
        ;
method  : qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_seq stm_ret R_BRACE
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_seq stm_ret R_BRACE
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_ret R_BRACE
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_ret R_BRACE
        ;
qualifier : PUBLIC
        | PRIVATE
        ;
arg_seq : %empty
        | arg_seq type id
        | type id
        ;
type    : INT L_SQUARE R_SQUARE
        | BOOLEAN
        | INT
        | id
        ;
stm_ret : RETURN exp SEMI
        ;
stm_seq : stm_seq stm
        | stm

stm     : L_BRACE stm_seq R_BRACE
        | IF L_PAREN stm R_PAREN ELSE stm
        | WHILE L_PAREN exp R_PAREN stm
        | PRINT_LINE L_PAREN exp R_PAREN SEMI
        | id ASSIGN exp SEMI
        | id L_SQUARE exp R_SQUARE ASSIGN exp SEMI
        ;

exp_seq : exp_seq COMMA exp
        | exp
        ;
exp     : exp AND exp
        | exp LESS exp
        | exp PLUS exp
        | exp MINUS exp
        | exp MULT exp
        | exp MOD exp
        | exp OR exp
        | exp L_SQUARE exp R_SQUARE
        | exp DOT LENGTH
        | exp DOT id L_PAREN R_PAREN
        | exp DOT id L_PAREN exp_seq R_PAREN
        | INTEGER_NUMBER
        | BOOLEAN_VALUE
        | id
        | THIS
        | NEW INT L_SQUARE exp R_SQUARE
        | NEW id L_PAREN R_PAREN
        | BANG exp
        | L_PAREN exp R_PAREN
        ;
id      : ID
        ;
/*token   :  CLASS                         { printf( "CLASS " ); onTokenParsed(); }
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
        ;*/
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
