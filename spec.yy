%{
  #include <stdio.h>
  #include <string.h>

  #include "SyntaxTree/TreeTypes.h"

  #define SHOW_COORDINATES 0

  #define NF_RED  "\x1B[31m"
  #define NF_RESET "\x1B[0m"

  #define COORDS Coordinates(yylloc.first_line, yylloc.first_column)

  void showTokenPosition();

  int yyerror(char* s);

  extern "C" int yylex();

  void onTokenParsed() {
    if(SHOW_COORDINATES) {
      showTokenPosition();
    }
  }

  Program* program = nullptr;
%}

%union {
  Id* id;
  IExpression* exp;
  Sequence<const IExpression>* exp_seq;
  IStatement* stm;
  Sequence<const IStatement>* stm_seq;
  ReturnStatement* stm_ret;
  Type* type;
  Qualifier* qualifier;
  MethodDeclaration* method;
  Sequence<const MethodDeclaration>* method_seq;
  VarDeclaration* var;
  Sequence<const VarDeclaration>* var_seq;
  ClassDeclaration* class_decl;
  Sequence<const ClassDeclaration>* class_seq;
  MainClass* class_main;
  Program* program;

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

%type <id> id;
%type <exp> exp;
%type <exp_seq> exp_seq;
%type <stm> stm;
%type <stm_seq> stm_seq;
%type <stm_ret> stm_ret;
%type <type> type;
%type <qualifier> qualifier;
%type <method> method;
%type <method_seq> method_seq;
%type <var> var;
%type <var_seq> var_seq arg_seq;
%type <class_decl> class;
%type <class_seq> class_seq;
%type <class_main> class_main;
%type <program> program;

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
              stm R_BRACE
            R_BRACE                         { printf( "main: \n" ); onTokenParsed(); }
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
        | exp                               { $$ = new Sequence<const IExpression>(COORDS, $1); printf( "exp_seq_first: \n" ); onTokenParsed(); }
        ;
exp     : exp AND exp                       {
                                              $$ = new BinaryExpression(COORDS, BET_And, $1, $3);
                                              printf( "exp: && \n" );
                                              onTokenParsed();
                                            }
        | exp LESS exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Less, $1, $3);
                                              printf( "exp: < \n" );
                                              onTokenParsed();
                                            }
        | exp PLUS exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Plus, $1, $3);
                                              printf( "exp: + \n" );
                                              onTokenParsed();
                                            }
        | exp MINUS exp                     {
                                              $$ = new BinaryExpression(COORDS, BET_Minus, $1, $3);
                                              printf( "exp: - \n" );
                                              onTokenParsed();
                                            }
        | exp MULT exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Mult, $1, $3);
                                              printf( "exp: * \n" );
                                              onTokenParsed();
                                            }
        | exp MOD exp                       {
                                              $$ = new BinaryExpression(COORDS, BET_Mod, $1, $3);
                                              printf( "exp: % \n" );
                                              onTokenParsed();
                                            }
        | exp OR exp                        {
                                              $$ = new BinaryExpression(COORDS, BET_Or, $1, $3);
                                              printf( "exp: || \n" );
                                              onTokenParsed();
                                            }
        | exp L_SQUARE exp R_SQUARE         {
                                              $$ = new ArrayMemberExpression(COORDS, $1, $3);
                                              printf( "exp: access_element \n" );
                                              onTokenParsed();
                                            }
        | exp DOT LENGTH                    { $$ = new ArrayLengthExpression(COORDS, $1); printf( "exp: length \n" ); onTokenParsed(); }
        | exp DOT id L_PAREN R_PAREN        { $$ = new CallMemberExpression(COORDS, $1, $3, nullptr);
                                              printf( "exp: call_method \n" );
                                              onTokenParsed();
                                            }
        | exp DOT id L_PAREN exp_seq R_PAREN  {
                                                  $$ = new CallMemberExpression(COORDS, $1, $3, $5);
                                                  printf( "exp: call_method_arged \n" );
                                                  onTokenParsed();
                                              }
        | INTEGER_NUMBER                    { $$ = new ValueExpression(COORDS, VT_Int, yylval.intVal); printf( "exp: int_value(%d)\n", yylval.intVal ); onTokenParsed(); }
        | BOOLEAN_VALUE                     { $$ = new ValueExpression(COORDS, VT_Boolean, yylval.intVal == 1 ? 1 : 0); printf( "exp: bool_value(%s) \n", yylval.intVal == 1 ? "true" : "false" ); onTokenParsed(); }
        | id                                { $$ = new IdExpression(COORDS, $1); printf( "exp: id(%s) \n", $1 ); onTokenParsed(); }
        | THIS                              { $$ = new ThisExpression(COORDS); printf( "exp: this \n" ); onTokenParsed(); }
        | NEW INT L_SQUARE exp R_SQUARE     { $$ = new NewIntArrayExpression(COORDS, $4); printf( "exp: new int[] \n" ); onTokenParsed(); }
        | NEW id L_PAREN R_PAREN            { $$ = new NewObjectExpression(COORDS, $2); printf( "exp: new id() \n" ); onTokenParsed(); }
        | BANG exp                          { $$ = new NotExpression(COORDS, $2); printf( "exp: ! \n" ); onTokenParsed(); }
        | L_PAREN exp R_PAREN               { $$ = $2; printf( "exp: () \n" ); onTokenParsed(); }
        ;
id      : ID                                { $$ = new Id(COORDS, std::string(yylval.stringVal)); printf( "id: %s \n", yylval.stringVal ); onTokenParsed(); }
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
