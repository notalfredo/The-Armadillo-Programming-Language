//---- DEFINITIONS  ----------------------------------------------
%{
#include <stdarg.h>
#include <stdio.h>

#include "parser.tab.h"
#include "lexer.yy.h"

// Defined below in the User Code section.
extern void yyerror( YYLTYPE *, void *, void *, const char * );
extern void printfLoc( YYLTYPE loc, char *fmt, ... );
extern void fprintLoc( FILE *fp, YYLTYPE loc );
%}

// Make the parser reentrant so we don't have those pesky global
//  data items.  This affects the signature to the yyerror
//  routine.  We also have to supply additional info to the lexer
//  and parser when we do this.  See the following %lex-param,
//  %parse-param definitions.
%define api.pure full

%code provides
{
  // Tell Flex the expected prototype of yylex.
#define YY_DECL                                 \
  int yylex (YYSTYPE* yylval, YYLTYPE *yylloc, void* scanner)
  YY_DECL;
}

// With reentrancy, we have to pass around all of the scanner
//  state.  The type of a pointer to an instance of that state is
//  called yyscan_t.
%code requires {
#include "node.h"

  typedef void *yyscan_t;
}

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { void **result }

%defines "parser.tab.h"

%locations

%union {
    Node                                   *node;
    NodeStatement                          *stmt;
    StatementList                          *stmtList;
    NodeExpression                         *expr;
    NodeInteger                            *intVal;
    NodeDouble                             *doubleVal;
    NodeString                             *stringVal;
    NodeIdentifier                         *id;
    NodeMethodCall                         *methodCall;
    NodeBinaryOperator                     *bop;
    NodeAssignment                         *assignment;
    NodeBlock                              *block;
    NodeExpressionStatement                *exprStmt;
    NodeReturnStatement                    *returnStmt;
    NodeVariableDeclaration                *varDecl;
    NodeIfDeclaration                      *ifDecl;
    NodeElifDeclaration                    *ElifDecl;
}

%token TOKEN_LET
%token TOKEN_BOP_ASSIGN
%token TOKEN_INTEGER TOKEN_REAL TOKEN_STRING
%token TOKEN_BREAK TOKEN_CONTINUE TOKEN_REPEAT TOKEN_WHILE 
%token TOKEN_IF TOKEN_ELIF TOKEN_ELSE
%token '+' '-' '*' '/' 

%token <id>         TOKEN_ID
%token <intVal>     TOKEN_LIT_INT 
%token <doubleVal>  TOKEN_LIT_REAL 
%token <stringVal>  TOKEN_LIT_STR

%type <block>    block
%type <expr>     expr
%type <stmt>     stmt
%type <stmtList> stmtList
%type <exprStmt> exprStmt
%type <ifDecl>   ifStmt
%type <varDecl>  declStmt

%left '+' '-' // tok_UOP_NOT
%left '/' '*' '%'
%right '^'
%right NEGATE POSITE PERCENT

%% //---- RULES --------------------------------------------------
start
  : block                     { *result = $1; }
  ;

block
  : '{' stmtList ';' '}'      { $$ =  $2; }
  | '{' '}'                   { $$ = new NodeBlock(); }
  ;

//-- Statements --------------------------------------------------
stmt
  : block
  | declStmt
  | exprStmt
  | ifStmt
  ;

stmtList
  : stmt              { $$ = new NodeBlock(); $$->StmtList.push_back($<stmt>1); }
  | stmtList ';' stmt { $1->StmtList.push_back($<stmt>2); }
  ;

//-- Declaration -------------------------------------------------
declStmt
  : TOKEN_LET TOKEN_ID ':' TOKEN_INTEGER TOKEN_BOP_ASSIGN TOKEN_LIT_INT {
    $$ = new NodeVariableDeclaration( *$2, TOKEN_INTEGER, $6 );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_REAL TOKEN_BOP_ASSIGN TOKEN_LIT_REAL {
    $$ = new NodeVariableDeclaration( *$2, TOKEN_REAL, $6 );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_STRING TOKEN_BOP_ASSIGN TOKEN_LIT_STR {
    $$ = new NodeVariableDeclaration( *$2,TOKEN_STRING, $6 );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_INTEGER {
    $$ = new NodeVariableDeclaration( *$2, TOKEN_INTEGER, NULL );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_REAL {
    $$ = new NodeVariableDeclaration( *$2, TOKEN_REAL, NULL );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_STRING {
    $$ = new NodeVariableDeclaration( *$2, TOKEN_STRING, NULL );
  }
  ;

//-- Expr statement ---------------------------------------------
exprStmt
  : expr    { $$ = new NodeExpressionStatement( *$1 ); }
  ;

//-- If statement ---------------------------------------------
ifStmt
  : TOKEN_IF expr block TOKEN_ELSE block    { $$ = new NodeIfDeclaration( $2, $3, $5 ); }
  | TOKEN_IF expr block                     { $$ = new NodeIfDeclaration( $2, $3, NULL ); }
  ;

//-- Expressions -------------------------------------------------

// Binary Operators
expr
  : expr '+' expr           { $$ = new NodeBinaryOperator( *$1, '+', *$3 ); }
  | expr '-' expr           { $$ = new NodeBinaryOperator( *$1, '-', *$3 ); }
  | expr '*' expr           { $$ = new NodeBinaryOperator( *$1, '*', *$3 ); }
  | expr '/' expr           { $$ = new NodeBinaryOperator( *$1, '/', *$3 ); }
  ;

expr
  : '(' expr ')'            { $$ = $2; }
  | TOKEN_ID
  | TOKEN_LIT_INT
  | TOKEN_LIT_REAL
  | TOKEN_LIT_STR
  ;

%% //---- USER CODE ----------------------------------------------

// Prints an error message.  Automatically called by the
//  generated parser when necessary.
void yyerror(  YYLTYPE *llocp, void *_scanner, void *_result, const char *msg )
{
  (void) _scanner;      // Not used at present
  (void) _result;       // Not used at present

  printfLoc( *llocp, "Error: %s\n", msg );
}

// Prints a formated message with a location at the front.
void printfLoc( YYLTYPE loc, char *fmt, ... )
{
  fprintLoc( stdout, loc );
  fputc( ' ', stdout );

  va_list ap;
  va_start( ap, fmt );
  vprintf( fmt, ap );
  va_end( ap );
}
//  
//  // Prints a location in a minimal way.
void fprintLoc( FILE *fp, YYLTYPE loc )
{
  if ( loc.first_line == loc.last_line ) {
    if ( loc.first_column == loc.last_column ) {
      //  <line>:<col>
      //    Location is on one line and is only one character wide.
      fprintf( fp, "%d(%d)", loc.first_line, loc.first_column );
    } else {
      //  <line>:<col>-<col>
      //    Location is on one line but is multiple characters wide.
      fprintf( fp, "%d(%d-%d)", loc.first_line, loc.first_column, loc.last_column );
    }
  } else {
    //  <line>:<col>-<line>:<col>
    //    Location is across more than one line.
    fprintf( fp, "%d(%d)-%d(%d)",
      loc.first_line, loc.first_column,
      loc.last_line, loc.last_column );
  }
}

//----------------------------------------------------------------
