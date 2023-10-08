// Dalio, Brian A.
// dalioba
// 2023-04-19
//----------------------------------------------------------------
#if !defined( __NODE_H__ )
#define __NODE_H__

#include <stdio.h>

//----------------------------------------------------------------
// Any changes to this enumeration have to be synced with the
//  kindToStr[] array in nodeDump.h.
enum {
  // Just so any useful kind has a non-zero value.  Also used to
  //  indicate an illegal type result.
  KIND_NOTHING = 0,

  // "Leaf" items -- names and literals.  The literal kinds are
  //    also used for indicating type.
  KIND_INT,
  KIND_REAL,
  KIND_STR,
  KIND_ID,

  // Binary operators
  KIND_BOP_ADD,
  KIND_BOP_DIV,
  KIND_BOP_SUB,

  // Unary operators
  KIND_UOP_PERCENT,
  KIND_UOP_POSITE,
  KIND_UOP_NEGATE,
  KIND_UOP_NOT,

  // Statements
  KIND_BLOCK,
  KIND_BREAK,
  KIND_CONT,
  KIND_DECL,
  KIND_EXPR,
  KIND_IF,
  KIND_READ,
  KIND_REPEAT,
  KIND_WHILE,
  KIND_WRITE,

  // Scope info
  KIND_SCOPE,
};

//----------------------------------------------------------------
typedef struct _Node {
  // Do not EVER touch or even LOOK AT this field in your code.
  //  Its secrets are NOT to be revealed to mere mortals.
  struct _Node *_____;

  //--------------------------------------
  // A node can be in a list.  If so, this points to the NEXT
  //  node in the list, if any.
  struct _Node *listNext;

  //--------------------------------------
  // What kind of node are we?
  int kind;

  // What type are we? (KIND_INT, KIND_REAL, KIND_STR for now)
  int type;

  //--------------------------------------
  // If we're a literal or ID, here's our value.
  int      intVal;      // Used by KIND_INT
  double   realVal;     // Used by KIND_REAL
  char    *strVal;      // Used by both KIND_ID and KIND_STR

  //--------------------------------------
  // If we're an expression, here are our operands.
  struct _Node *left;
  struct _Node *right;

  //--------------------------------------
  // If we're a statement, here are our parts.  Different
  //  statements fill in different subsets of these fields.

  // BLOCK statement
  int           blockNum;
  struct _Node *stmtList;

  // DECL statement
  int           which;        // KIND_INT, KIND_REAL, KIND_STR
  struct _Node *id;
  struct _Node *initExpr;

  // EXPR statement
  struct _Node *expr;

  // IF statement
  struct _Node *thenBlock;
  struct _Node *elseBlock;

  // READ, WRITE statement
  struct _Node *exprList;

  // IF, WHILE, REPEAT UNTIL statement
  struct _Node *test;
  struct _Node *block;

  // SCOPE
  struct _Node *parent;
  int           scopeNum;     // also used by KIND_ID when entered into a scope
  struct _Node *entryList;
} Node;

//----------------------------------------------------------------
// nodeAllocFree.c
extern Node *allocNode();
extern Node *appendToNodeList( Node *list, Node *node );
extern void  freeAllNodes();
extern void  freeNode( Node *me );
extern int nodeListLen( Node *me );

// nodeDump.c
extern char *kindToStr[];

void dumpNode( Node *me );
void dumpfNode( FILE *fp, Node *me );

// nodeExprs.c
extern Node *makeBinOp( const int which, Node *o1, Node *o2 );
extern Node *makeUnaOp( const int which, Node *o1 );

// nodeLeafItems.c
extern Node *makeID( const char *value );
extern Node *makeIntLit( const int value );
extern Node *makeRealLit( const double value );
extern Node *makeStrLit( const char *value, int strip );

// nodeStmts.c
extern Node *makeBlock( Node *stmtList );
extern Node *makeBreak();
extern Node *makeContinue();
extern Node *makeDecl( const int which, Node *id, Node *initExpr );
extern Node *makeExpr( Node *expr );
extern Node *makeIf( Node *test, Node *thenBlock, Node *elseBlock );
extern Node *makeRead( Node *exprList );
extern Node *makeRepeat( Node *block, Node *test );
extern Node *makeWhile( Node *test, Node *block );
extern Node *makeWrite( Node *exprList );

// nodeScope.c
extern Node *addNameToScope( const char *name, const int type );
extern void  closeScope();
extern void  dumpfScopes( FILE *fp );
extern void  dumpfScopesDecls( void (*allocVar)( const int, const int, const char * ) );
extern Node *findInCurrentScope( const char *name );
extern Node *findInScopeHierarchy( const char *name );
extern void  openScope( int scopeNum );

//----------------------------------------------------------------
#endif
