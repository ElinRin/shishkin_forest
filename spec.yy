%{
  #include <stdio.h>
  #include <string.h>

  #include "SyntaxTree/TreeTypes.h"
  #include "PrettyPrint/PrintVisitor.h"

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
%type <class_decl> class class_ext;
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

program : class_main class_seq END          { $$ = new Program(COORDS, $1, $2); onTokenParsed(); program = $$; }
        | class_main END                    { $$ = new Program(COORDS, $1, nullptr); onTokenParsed(); }
        ;
class_main : CLASS id L_BRACE
              PUBLIC MAIN L_PAREN R_PAREN L_BRACE
              stm R_BRACE
            R_BRACE                         { $$ = new MainClass(COORDS, $2, $9); onTokenParsed(); }
        ;
class_seq : class_seq class                 { $$ = new Sequence<const ClassDeclaration>(COORDS, $1, $2); onTokenParsed(); }
        | class_seq class_ext               { $$ = new Sequence<const ClassDeclaration>(COORDS, $1, $2); onTokenParsed(); }
        | class                             { $$ = new Sequence<const ClassDeclaration>(COORDS, $1); onTokenParsed(); }
        | class_ext                         { $$ = new Sequence<const ClassDeclaration>(COORDS, $1); onTokenParsed(); }
        ;
class_ext : CLASS id EXTENDS id L_BRACE
          var_seq method_seq R_BRACE        { $$ = new ClassDeclaration(COORDS, $2, $4, $6, $7); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          var_seq R_BRACE                   { $$ = new ClassDeclaration(COORDS, $2, $4, $6, nullptr); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE
          method_seq R_BRACE                { $$ = new ClassDeclaration(COORDS, $2, $4, nullptr, $6); onTokenParsed(); }
        | CLASS id EXTENDS id L_BRACE R_BRACE { $$ = new ClassDeclaration(COORDS, $2, $4, nullptr, nullptr); onTokenParsed(); }
        ;
class : CLASS id L_BRACE
          var_seq method_seq R_BRACE        { $$ = new ClassDeclaration(COORDS, $2, nullptr,  $4, $5); onTokenParsed(); }
        | CLASS id L_BRACE
          var_seq R_BRACE                   { $$ = new ClassDeclaration(COORDS, $2, nullptr, $4, nullptr); onTokenParsed(); }
        | CLASS id L_BRACE
          method_seq R_BRACE                { $$ = new ClassDeclaration(COORDS, $2, nullptr, nullptr, $4); onTokenParsed(); }
        | CLASS id L_BRACE R_BRACE          { $$ = new ClassDeclaration(COORDS, $2, nullptr, nullptr, nullptr); onTokenParsed(); }
        ;
var_seq : var_seq var                       { $$ = new Sequence<const VarDeclaration>(COORDS, $1, $2); onTokenParsed(); }
        | var                               { $$ = new Sequence<const VarDeclaration>(COORDS, $1); onTokenParsed(); }
        ;
var     : type id SEMI                      { $$ = new VarDeclaration(COORDS, $1, $2); onTokenParsed(); }
        ;
method_seq : method_seq method              { $$ = new Sequence<const MethodDeclaration>(COORDS, $1, $2); onTokenParsed(); }
        | method                            { $$ = new Sequence<const MethodDeclaration>(COORDS, $1); onTokenParsed(); }
        ;
method  : qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_seq stm_ret R_BRACE { $$ = new MethodDeclaration(COORDS, $1, $2, $3, $5, $8, $9, $10); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_seq stm_ret R_BRACE         { $$ = new MethodDeclaration(COORDS, $1, $2, $3, $5, nullptr, $8, $9); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            var_seq stm_ret R_BRACE         { $$ = new MethodDeclaration(COORDS, $1, $2, $3, $5, $8, nullptr, $9); onTokenParsed(); }
        | qualifier type id L_PAREN arg_seq R_PAREN L_BRACE
            stm_ret R_BRACE                 {
                                              $$ = new MethodDeclaration(COORDS, $1, $2, $3, $5, nullptr, nullptr, $8);
                                              onTokenParsed();
                                            }
        ;
qualifier : PUBLIC                          { $$ = new Qualifier(COORDS, Q_Public); onTokenParsed(); }
        | PRIVATE                           { $$ = new Qualifier(COORDS, Q_Private); onTokenParsed(); }
        ;
arg_seq : %empty                            { $$ = new Sequence<const VarDeclaration>(COORDS); onTokenParsed(); }
        | arg_seq COMMA type id             {
                                              $$ = new Sequence<const VarDeclaration>(COORDS, $1, new VarDeclaration(COORDS, $3, $4));
                                              onTokenParsed();
                                            }
        | type id                           {
                                              $$ = new Sequence<const VarDeclaration>(COORDS, new VarDeclaration(COORDS, $1, $2));
                                              onTokenParsed();
                                            }
        ;
type    : INT L_SQUARE R_SQUARE             { $$ = new Type(COORDS, T_IntArray); onTokenParsed(); }
        | BOOLEAN                           { $$ = new Type(COORDS, T_Boolean); onTokenParsed(); }
        | INT                               { $$ = new Type(COORDS, T_Int); onTokenParsed(); }
        | id                                { $$ = new Type(COORDS, T_UserType, $1); onTokenParsed(); }
        ;
stm_ret : RETURN exp SEMI                   { $$ = new ReturnStatement(COORDS, $2); onTokenParsed(); }
        ;
stm_seq : stm_seq stm                       { $$ = new Sequence<const IStatement>(COORDS, $1, $2); onTokenParsed(); }
        | stm                               { $$ = new Sequence<const IStatement>(COORDS, $1); onTokenParsed(); }

stm     : L_BRACE stm_seq R_BRACE           { $$ = new BraceSequenceStatement(COORDS, $2); onTokenParsed(); }
        | IF L_PAREN exp R_PAREN stm ELSE stm   { $$ = new IfElseStatement(COORDS, $3, $5, $7); onTokenParsed(); }
        | WHILE L_PAREN exp R_PAREN stm     { $$ = new WhileStatement(COORDS, $3, $5); onTokenParsed(); }
        | PRINT_LINE L_PAREN exp R_PAREN SEMI { $$ = new PrintLineStatement(COORDS, $3); onTokenParsed(); }
        | id ASSIGN exp SEMI                { $$ = new AssignStatement(COORDS, $1, $3); onTokenParsed(); }
        | id L_SQUARE exp R_SQUARE ASSIGN exp SEMI  {
                                                      $$ = new AssignArrayElementStatement(COORDS, $1, $3, $6);
                                                      onTokenParsed();
                                                    }
        ;

exp_seq : exp_seq COMMA exp                 { $$ = new Sequence<const IExpression>(COORDS, $1, $3); onTokenParsed(); }
        | exp                               { $$ = new Sequence<const IExpression>(COORDS, $1); onTokenParsed(); }
        ;
exp     : exp AND exp                       {
                                              $$ = new BinaryExpression(COORDS, BET_And, $1, $3);
                                              onTokenParsed();
                                            }
        | exp LESS exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Less, $1, $3);
                                              onTokenParsed();
                                            }
        | exp PLUS exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Plus, $1, $3);
                                              onTokenParsed();
                                            }
        | exp MINUS exp                     {
                                              $$ = new BinaryExpression(COORDS, BET_Minus, $1, $3);
                                              onTokenParsed();
                                            }
        | exp MULT exp                      {
                                              $$ = new BinaryExpression(COORDS, BET_Mult, $1, $3);
                                              onTokenParsed();
                                            }
        | exp MOD exp                       {
                                              $$ = new BinaryExpression(COORDS, BET_Mod, $1, $3);
                                              onTokenParsed();
                                            }
        | exp OR exp                        {
                                              $$ = new BinaryExpression(COORDS, BET_Or, $1, $3);
                                              onTokenParsed();
                                            }
        | exp L_SQUARE exp R_SQUARE         {
                                              $$ = new ArrayMemberExpression(COORDS, $1, $3);
                                              onTokenParsed();
                                            }
        | exp DOT LENGTH                    { $$ = new ArrayLengthExpression(COORDS, $1); onTokenParsed(); }
        | exp DOT id L_PAREN R_PAREN        { $$ = new CallMemberExpression(COORDS, $1, $3, nullptr);
                                              onTokenParsed();
                                            }
        | exp DOT id L_PAREN exp_seq R_PAREN  {
                                                  $$ = new CallMemberExpression(COORDS, $1, $3, $5);
                                                  onTokenParsed();
                                              }
        | INTEGER_NUMBER                    { $$ = new ValueExpression(COORDS, VT_Int, yylval.intVal); onTokenParsed(); }
        | BOOLEAN_VALUE                     { $$ = new ValueExpression(COORDS, VT_Boolean, yylval.intVal == 1 ? 1 : 0); onTokenParsed(); }
        | id                                { $$ = new IdExpression(COORDS, $1); onTokenParsed(); }
        | THIS                              { $$ = new ThisExpression(COORDS); onTokenParsed(); }
        | NEW INT L_SQUARE exp R_SQUARE     { $$ = new NewIntArrayExpression(COORDS, $4); onTokenParsed(); }
        | NEW id L_PAREN R_PAREN            { $$ = new NewObjectExpression(COORDS, $2); onTokenParsed(); }
        | BANG exp                          { $$ = new NotExpression(COORDS, $2); onTokenParsed(); }
        | L_PAREN exp R_PAREN               { $$ = new ContainerExpression(COORDS, $2); onTokenParsed(); }
        ;
id      : ID                                { $$ = new Id(COORDS, std::string(yylval.stringVal)); onTokenParsed(); }
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
  PrintVisitor printer("tree.dot");
  printer.CreateGraph(program);
  delete program;
}
