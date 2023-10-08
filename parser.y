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

// Add the following parameters to the lexer and parser calls.
//  This is necessary when we're getting rid of global references.
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { void **result }

// Generate a header file with the token names, union of possible
//  attributes, etc.  (This header will be included by the lexer.)
%defines "parser.tab.h"

// Enable the use of yylloc.  In the rules below, we get location
//  information via the '@' marker.
%locations

// The union of all possible attributes that can be returned by
//  any category of token.
%union {
    Node                                   *node;
    NodeStatement                          *stmt;
    NodeExpression                         *expr;
    NodeInteger                            *intVal;
    NodeDouble                             *doubleVal;
    NodeIdentifier                         *id;
    NodeMethodCall                         *methodCall;
    NodeBinaryOperator                     *bop;
    NodeAssignment                         *assignment;
    NodeBlock                              *block;
    NodeExpressionStatement                *exprStmt;
    NodeReturnStatement                    *returnStmt;
    NodeVariableDeclaration                *varvec;
    NodeIfDeclaration                      *ifDecl;
    NodeElifDeclaration                    *ElifDecl;
}

// Token names (and types, if required)
  // Keyword tokens -- no attribute, so no type required.
%token TOKEN_LET
%token TOKEN_BOP_ASSIGN
%token TOKEN_INTEGER TOKEN_REAL TOKEN_STRING
%token TOKEN_BREAK TOKEN_CONTINUE TOKEN_REPEAT TOKEN_WHILE 
%token TOKEN_IF TOKEN_ELIF TOKEN_ELSE
%token '+' '-' '*' '/' 


// Unary Operator
// %token tok_UOP_NOT

// Operator tokens -- no attribute, so no type required.
//%token tok_???

// Primitive (leaf-level) item nodes.  These token categories
//  have attributes, so they need their type specified.
%token <node>  TOKEN_ID
%token <node>  TOKEN_LIT_INT  TOKEN_LIT_REAL  TOKEN_LIT_STR

// The nonterminal names that have a value.  A type has to be
//  given.  (So why did yacc use "%token" for "token" but "%type"
//  for "nonterminal"?  Because it's shorter?  Who knows?)
// %type <node>   breakStmt contStmt declStmt exprStmt ifStmt readStmt repeatStmt whileStmt writeStmt
%type <node>   block expr exprList exprStmt stmt stmtList ifStmt declStmt

%type <node>   epsilon

// Precedence / associativity -- Use %left, %right, and %nonassoc
//  to indicate associtivity of the operator.  The lines go from
//  LOWEST precedence to HIGHEST precedence.
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
  : stmt              { $$ = new NodeBlock(); $$->statements.push_back($<stmt>1); }
  | stmtList ';' stmt { $1->statements.push_back($<stmt>2); }
  ;

// breakStmt
//   : TOKEN_BREAK       { $$ = makeBreak(); }
//   ;
// contStmt
//   : TOKEN_CONTINUE    { $$ = makeContinue(); }
//   ;

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

//-- Read statement ----------------------------------------------
//  readStmt
//    : TOKEN_READ exprList       { $$ = makeRead( $2 ); }
//    ;

//-- Repeat statement ---------------------------------------------
//  repeatStmt
//    : TOKEN_REPEAT block TOKEN_UNTIL expr { $$ = makeRepeat( $2, $4 ); }
//    ;

//-- While statement ---------------------------------------------
// whileStmt
//   : TOKEN_WHILE expr block    { $$ = makeWhile( $2, $3 ); }
//   ;

//-- Write statement ---------------------------------------------
//  writeStmt
//    : TOKEN_WRITE exprList      { $$ = makeWrite( $2 ); }
//    ;

//-- Expressions -------------------------------------------------

// Binary Operators
expr
  : expr '+' expr           { $$ = new NodeBinaryOperator( *$1, '+', *$3 ); }
  | expr '-' expr           { $$ = new NodeBinaryOperator( *$1, '-', *$3 ); }
  | expr '*' expr           { $$ = new NodeBinaryOperator( *$1, '*', *$3 ); }
  | expr '/' expr           { $$ = new NodeBinaryOperator( *$1, '/', *$3 ); }
  ;

// Unary Operators
//  expr
//  : TOKEN_UOP expr          { $$ = makeUnaOp( KIND_UOP_NOT, $2 ); }
//  : '-' expr %prec NEGATE   { $$ = makeUnaOp( KIND_UOP_NEGATE, $2 ); }
//  | '+' expr %prec POSITE   { $$ = makeUnaOp( KIND_UOP_POSITE, $2 ); }
//  ;

expr
  : '(' expr ')'            { $$ = $2; }
  | TOKEN_ID
  | TOKEN_LIT_INT
  | TOKEN_LIT_REAL
  | TOKEN_LIT_STR
  ;

// Unary Operators
// expr
//   : TOKEN_UOP expr          { $$ = makeUnaOp( KIND_UOP_NOT, $2 ); }
//   | '-' expr %prec NEGATE   { $$ = makeUnaOp( KIND_UOP_NEGATE, $1 ); }
//   | '+' expr %prec POSITE   { $$ = makeUnaOp( KIND_UOP_POSITE, $1 ); }
//   ;


// exprList
//   : exprList ',' expr       { $$ = appendToNodeList( $1, $3 ); }
//   | expr
//   ;

//-- Epsilon -----------------------------------------------------
epsilon : { $$ = NULL; } ;


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
