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
  Node *node;
}

// Token names (and types, if required)
  // Keyword tokens -- no attribute, so no type required.
%token TOKEN_LET TOKEN_ID
%token TOKEN_INTEGER TOKEN_REAL TOKEN_STRING
%token TOKEN_BREAK TOKEN_CONTINUE TOKEN_REPEAT TOKEN_WHILE 
%token TOKEN_IF TOKEN_ELIF TOKEN_ELSE


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
%type <node>   breakStmt contStmt declStmt exprStmt ifStmt readStmt repeatStmt whileStmt writeStmt
%type <node>   block expr exprList stmt stmtList

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
  : '{' stmtList ';' '}'      { $$ = makeBlock( $2 ); }
  | '{' '}'                   { $$ = makeBlock( NULL ); }
  ;

//-- Statements --------------------------------------------------
stmt
  : block
  | breakStmt
  | contStmt
  | declStmt
  | exprStmt
  | ifStmt
  | readStmt
  | repeatStmt
  | whileStmt
  | writeStmt
  ;

stmtList
  : stmt
  | stmtList ';' stmt { $$ = appendToNodeList( $1, $3 ); }
  ;

breakStmt
  : TOKEN_BREAK       { $$ = makeBreak(); }
  ;
contStmt
  : TOKEN_CONTINUE    { $$ = makeContinue(); }
  ;

//-- Declaration -------------------------------------------------
declStmt
  : TOKEN_LET TOKEN_ID ':' TOKEN_INTEGER TOKEN_BOP_ASSIGN TOKEN_LIT_INT {
    $$ = makeDecl( KIND_INT,  $2, makeIntLit( $6 ) );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_REAL TOKEN_BOP_ASSIGN TOKEN_LIT_REAL {
    $$ = makeDecl( KIND_REAL,  $2, makeRealLit( $6 ) );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_STRING TOKEN_BOP_ASSIGN TOKEN_LIT_STR {
    $$ = makeDecl( KIND_STRING,  $2, makeStringLit( $6 ) );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_INTEGER {
    $$ = makeDecl( KIND_INT,  $2, makeIntLit( 0 ) );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_REAL {
    $$ = makeDecl( KIND_REAL,  $2, makeRealLit( 0 ) );
  }
  | TOKEN_LET TOKEN_ID ':' TOKEN_STRING {
    $$ = makeDecl( KIND_STRING,  $2, makeStringLit( 0 ) );
  }
  ;

//-- Expr statement ---------------------------------------------
exprStmt
  : expr    { $$ = makeExpr( $1 ); }
  ;

//-- If statement ---------------------------------------------
ifStmt
  : TOKEN_IF expr block TOKEN_ELSE block    { $$ = makeIf( $2, $3, $5 ); }
  | TOKEN_IF expr block                   { $$ = makeIf( $2, $3, makeBlock( NULL ) ); }
  ;

//-- Read statement ----------------------------------------------
readStmt
  : TOKEN_READ exprList       { $$ = makeRead( $2 ); }
  ;

//-- Repeat statement ---------------------------------------------
repeatStmt
  : TOKEN_REPEAT block TOKEN_UNTIL expr { $$ = makeRepeat( $2, $4 ); }
  ;

//-- While statement ---------------------------------------------
whileStmt
  : TOKEN_WHILE expr block    { $$ = makeWhile( $2, $3 ); }
  ;

//-- Write statement ---------------------------------------------
writeStmt
  : TOKEN_WRITE exprList      { $$ = makeWrite( $2 ); }
  ;

//-- Expressions -------------------------------------------------

// Binary Operators
expr
  : expr '+' expr       { $$ = makeBinOp( KIND_BOP_ADD, $1, $3 ); }
  | expr '-' expr       { $$ = makeBinOp( KIND_BOP_SUB, $1, $3 ); }
  | expr '*' expr       { $$ = makeBinOp( KIND_BOP_MUL, $1, $3 ); }
  | expr '/' expr       { $$ = makeBinOp( KIND_BOP_DIV, $1, $3 ); }
  ;

// Unary Operators
expr
  : TOKEN_UOP expr        { $$ = makeUnaOp( KIND_UOP_NOT, $2 ); }
  | '-' expr %prec NEGATE   { $$ = makeUnaOp( KIND_UOP_NEGATE, $1 ); }
  | '+' expr %prec POSITE   { $$ = makeUnaOp( KIND_UOP_POSITE, $1 ); }
  ;

expr
  : '(' expr ')'        { $$ = $2; }
  | TOKEN_ID
  | TOKEN_LIT_INT
  | TOKEN_LIT_REAL
  | TOKEN_LIT_STR
  ;

exprList
  : exprList ',' expr { $$ = appendToNodeList( $1, $3 ); }
  | expr
  ;

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

// Prints a location in a minimal way.
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
