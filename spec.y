%{
  #include <stdio.h>
  #include <string.h>

  #define SHOW_COORDINATES 0

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

%type<stringVal>  id

%left   OR
%left   AND
%left   MINUS PLUS
%left   MOD
%left   MULT
%left   LESS
%left   DOT
%left   L_SQUARE

%right  BANG

%%

program : class_main class_seq END          { printf( "program: \n" ); onTokenParsed(); }
        | class_main END                    { printf( "program: main only \n" ); onTokenParsed(); }
        ;
class_main : CLASS id L_BRACE
              PUBLIC MAIN L_PAREN R_PAREN L_BRACE
              stm R_BRACE                   { printf( "main: \n" ); onTokenParsed(); }
            R_BRACE
        ;
class_seq : class_seq class                 { printf( "class_seq: class ext \n" ); onTokenParsed(); }
        | class_seq class_ext               { printf( "class_seq: class_ext \n" ); onTokenParsed(); }
        | class                             { printf( "class_seq: class first \n" ); onTokenParsed(); }
        | class_ext                         { printf( "class_seq: class_ext first\n" ); onTokenParsed(); }
        ;
class_ext : CLASS id EXTENDS id L_BRACE
          var_seq method_seq R_BRACE        { printf( "class_ext: (%s:%s) var method \n", $2, $4 ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          var_seq R_BRACE                   { printf( "class_ext: (%s:%s) var \n", $2, $4 ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          method_seq R_BRACE                { printf( "class_ext: (%s:%s) method \n", $2, $4 ); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE R_BRACE { printf( "class_ext: (%s:%s) empty \n", $2, $4 ); onTokenParsed(); }
        ;
class : CLASS id L_BRACE
          var_seq method_seq R_BRACE        { printf( "class: (%s) var method \n", $2 ); onTokenParsed(); }
        | CLASS id L_BRACE
          var_seq R_BRACE                   { printf( "class: (%s) var \n", $2 ); onTokenParsed(); }
        | CLASS id L_BRACE
          method_seq R_BRACE                { printf( "class: (%s) method \n", $2 ); onTokenParsed(); }
        | CLASS id L_BRACE R_BRACE          { printf( "class: (%s) empty \n", $2 ); onTokenParsed(); }
        ;
var_seq : var_seq var                       { printf( "var_seq: \n" ); onTokenParsed(); }
        | var                               { printf( "var_seq_first: \n" ); onTokenParsed(); }
        ;
var     : type id SEMI                      { printf( "var: (%s)\n", $2 ); onTokenParsed(); }
        ;
method_seq : method_seq method              { printf( "method_seq: \n" ); onTokenParsed(); }
        | method                            { printf( "method_seq_first: \n" ); onTokenParsed(); }
        ;
method  : qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_seq stm_ret R_BRACE { printf( "method: var stm ret \n" ); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_seq stm_ret R_BRACE         { printf( "method: stm ret \n" ); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_ret R_BRACE         { printf( "method: var ret \n" ); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_ret R_BRACE                 { printf( "method: return \n" ); onTokenParsed(); }
        ;
qualifier : PUBLIC                          { printf( "qualifier: public \n" ); onTokenParsed(); }
        | PRIVATE                           { printf( "qualifier: private \n" ); onTokenParsed(); }
        ;
arg_seq : %empty                            { printf( "arg_seq(empty): empty \n" ); onTokenParsed(); }
        | arg_seq COMMA type id                   { printf( "arg_seq: comma \n" ); onTokenParsed(); }
        | type id                           { printf( "arg_seq_first: \n" ); onTokenParsed(); }
        ;
type    : INT L_SQUARE R_SQUARE             { printf( "type: int-array \n" ); onTokenParsed(); }
        | BOOLEAN                           { printf( "type: bool \n" ); onTokenParsed(); }
        | INT                               { printf( "type: int \n" ); onTokenParsed(); }
        | id                                { printf( "type: id(%s) \n", yylval.stringVal ); onTokenParsed(); }
        ;
stm_ret : RETURN exp SEMI                   { printf( "stm_ret: \n" ); onTokenParsed(); }
        ;
stm_seq : stm_seq stm                       { printf( "stm_seq: \n" ); onTokenParsed(); }
        | stm                               { printf( "stm_seq_first: \n" ); onTokenParsed(); }

stm     : L_BRACE stm_seq R_BRACE           { printf( "stm: braces \n" ); onTokenParsed(); }
        | IF L_PAREN exp R_PAREN stm ELSE stm   { printf( "stm: if-else \n" ); onTokenParsed(); }
        | WHILE L_PAREN exp R_PAREN stm     { printf( "stm: loop \n" ); onTokenParsed(); }
        | PRINT_LINE L_PAREN exp R_PAREN SEMI { printf( "stm: print \n" ); onTokenParsed(); }
        | id ASSIGN exp SEMI                { printf( "stm: assign \n" ); onTokenParsed(); }
        | id L_SQUARE exp R_SQUARE ASSIGN exp SEMI  { printf( "stm: assign_element \n" ); onTokenParsed(); }
        ;

exp_seq : exp_seq COMMA exp                 { printf( "exp_seq: \n" ); onTokenParsed(); }
        | exp                               { printf( "exp_seq_first: \n" ); onTokenParsed(); }
        ;
exp     : exp AND exp                       { printf( "exp: && \n" ); onTokenParsed(); }
        | exp LESS exp                      { printf( "exp: < \n" ); onTokenParsed(); }
        | exp PLUS exp                      { printf( "exp: + \n" ); onTokenParsed(); }
        | exp MINUS exp                     { printf( "exp: - \n" ); onTokenParsed(); }
        | exp MULT exp                      { printf( "exp: * \n" ); onTokenParsed(); }
        | exp MOD exp                       { printf( "exp: % \n" ); onTokenParsed(); }
        | exp OR exp                        { printf( "exp: || \n" ); onTokenParsed(); }
        | exp L_SQUARE exp R_SQUARE         { printf( "exp: access_element \n" ); onTokenParsed(); }
        | exp DOT LENGTH                    { printf( "exp: length \n" ); onTokenParsed(); }
        | exp DOT id L_PAREN R_PAREN        { printf( "exp: call_method \n" ); onTokenParsed(); }
        | exp DOT id L_PAREN exp_seq R_PAREN  { printf( "exp: call_method_arged \n" ); onTokenParsed(); }
        | INTEGER_NUMBER                    { printf( "exp: int_value(%d)\n", yylval.intVal ); onTokenParsed(); }
        | BOOLEAN_VALUE                     { printf( "exp: bool_value(%s) \n", yylval.intVal == 1 ? "true" : "false" ); onTokenParsed(); }
        | id                                { printf( "exp: id(%s) \n", $1 ); onTokenParsed(); }
        | THIS                              { printf( "exp: this \n" ); onTokenParsed(); }
        | NEW INT L_SQUARE exp R_SQUARE     { printf( "exp: new int[] \n" ); onTokenParsed(); }
        | NEW id L_PAREN R_PAREN            { printf( "exp: new id() \n" ); onTokenParsed(); }
        | BANG exp                          { printf( "exp: ! \n" ); onTokenParsed(); }
        | L_PAREN exp R_PAREN               { printf( "exp: () \n" ); onTokenParsed(); }
        ;
id      : ID                                { strcpy($$, $1); printf( "id: %s \n", yylval.stringVal ); onTokenParsed(); }
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
