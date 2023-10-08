// Dalio, Brian A.
// dalioba
// 2023-04-19
//----------------------------------------------------------------
#include <stdlib.h>

#include "node.h"

//----------------------------------------------------------------
static void  _processStmt( Node *stmt );

static Node *_processBlock( Node *block );
static void  _processBreak();
static void  _processContinue();
static void  _processDecl( Node *stmt );
static void  _processExpr( Node *expr );
static void  _processExprStmt( Node *stmt );
static void  _processIf( Node *stmt );
static void  _processRead( Node *stmt );
static void  _processRepeat( Node *stmt );
static void  _processWhile( Node *stmt );
static void  _processWrite( Node *stmt );

//----------------------------------------------------------------
static int _numErrs;

Node *semantic( Node *root )
{
  Node *result = NULL;

  // Being optimistic, we assume that no errors will occur.
  _numErrs = 0;

  // Ensure the root node is truly a block.
  if ( root->kind != KIND_BLOCK ) {
    printf( "semantic : root is kind %s (%d) instead of %s (%d)?\n",
      kindToStr[ root->kind ], root->kind,
      kindToStr[ KIND_BLOCK ], KIND_BLOCK );
    _numErrs++;
  } else {
    // It's OK to process ...

    // First open and initialize the background global scope.
    openScope( 0 );

    // Handle the root node ...
    result = _processBlock( root );

    // Close the background global scope.
    closeScope();
  }

  // How did we do?
  printf( "semantic : %d semantic error%s detected.\n",
    _numErrs, _numErrs == 1 ? "" : "s" );

  // NULL returned if any semantic errors detected.
  return _numErrs == 0 ? result : NULL;
}

//----------------------------------------------------------------
static int blockNum = 1;
static int inLoop = 0;

static Node *_processBlock( Node *block )
{
  // New block gets next block num as its unique identifier.
  block->blockNum = blockNum++;

  // Each block is a scope, so open a new scope.
  openScope( block->blockNum );

  // Process each statement of the block.
  for ( Node *stmt = block->stmtList; stmt; stmt = stmt->listNext ) {
    _processStmt( stmt );
  }

  // Block is finished, so close its scope.
  closeScope();

  // All done!
  return block;
}

//----------------------------------------------------------------
static void _processBreak() {
    // check if in a loop.
    if ( !inLoop ) {
        _numErrs++;
        printf( "_processBreak:  must be in a looping construct.\n");
    }
    inLoop = 0;

}


//----------------------------------------------------------------
static void _processContinue() {
    // check if in a loop.
    if ( !inLoop ) {
        _numErrs++;
        printf( "_processContinue:  must be in a looping construct.\n");
    }
}


//----------------------------------------------------------------
static void _processDecl( Node *stmt )
{
  // Ensure name isn't being redeclared.
  if ( findInCurrentScope( stmt->id->strVal ) ) {
    _numErrs++;
    printf( "_processDecl : attempt to redeclare %s in the same scope.\n",
      stmt->id->strVal );
    return;
  }

  // Semantically check the init expr.
  _processExpr( stmt->initExpr );

  // Enter the declared ID into the current scope with the given type.
  stmt->id = addNameToScope( stmt->id->strVal, stmt->which );

  // Ensure ID and init expr are the same type.
  if ( stmt->id->type != stmt->initExpr->type ) {
    _numErrs++;
    printf( "_processDecl : %s's declared type and initExpr type are not the same.\n",
      stmt->id->strVal );
    return;
  }
}

//----------------------------------------------------------------
static void _inappropriateBOP( const int which, const int ltype, const int rtype )
{
  printf( "BOP %s given inappropriate operand types %s and %s.\n",
    kindToStr[ which ], kindToStr[ ltype ], kindToStr[ rtype ] );
  _numErrs++;
}

static int _bopType( const int which, const int ltype, const int rtype )
{
  //--------------------------------------
  // The NOTHING type pollutes all upstream computations.
  // We report the error when NOTHING is first generated and
  //  silently ignore it in upstream computations.
  if ( ltype == KIND_NOTHING || rtype == KIND_NOTHING ) {
    // At least one of our operands has already futzed up.  Just
    //  pass the NOTHING type up silently.
    return KIND_NOTHING;
  }

  //--------------------------------------
  // There are so few operators that allow string operands that
  //  it's easiest to handle them up front as special cases.
  if ( ltype == KIND_STR || rtype == KIND_STR ) {
    switch ( which ) {
      case KIND_BOP_SUB :
      case KIND_BOP_ADD :
        if ( ltype == rtype ) {
          // STR + STR is string concatenation.
          return KIND_STR;
        }
        // + is inappropriate for mixed string / other type.
        _inappropriateBOP( which, ltype, rtype );
        return KIND_NOTHING;

      default :
        // No other operators are acceptable for str operand(s).
        _inappropriateBOP( which, ltype, rtype );
        return KIND_NOTHING;
    }
  }

  //--------------------------------------
  // Left and Right type now must both be numeric, int or real.
  switch ( which ) {
    case KIND_BOP_SUB :
    case KIND_BOP_ADD : case KIND_BOP_DIV :
      if ( ltype == rtype ) return ltype;
      return KIND_REAL;

    default :
      // WTF?
      printf( "_bopType : Unknown BOP kind %d?\n", which );
      _numErrs++;
      return KIND_NOTHING;
  }
}

static void _processExpr( Node *expr )
{
  switch ( expr->kind ) {
    //-- Leaf items ----------------------------------------------
    case KIND_INT : case KIND_REAL : case KIND_STR :
      expr->type = expr->kind;
      break;

    case KIND_ID : {
      Node *id = findInScopeHierarchy( expr->strVal );

      if ( id == NULL ) {
        _numErrs++;
        printf( "_processExpr: \"%s\" is not declared.\n",
          expr->strVal );

        expr->type = KIND_INT;
      } else {
        expr->scopeNum = id->scopeNum;
        expr->type = id->type;
      }
      break;
    }

    //-- UOP Operators -----------------------------------------------
    case KIND_UOP_POSITE: 
    case KIND_UOP_NEGATE: 
    case KIND_UOP_NOT: 
      _processExpr( expr->left );
      //_processExpr( expr->right );
      expr->type = expr->left->type;
      break;

    //-- BOP Operators -----------------------------------------------
    case KIND_BOP_SUB : case KIND_BOP_ADD : case KIND_BOP_DIV :
      _processExpr( expr->left );
      _processExpr( expr->right );
      expr->type = _bopType( expr->kind, expr->left->type, expr->right->type );
      break;

    //-- Unknown expr kind? --------------------------------------
    default :
      _numErrs++;
      printf( "_processExpr:  WTF?  Don't understand expr kind %s (%d)\n",
        kindToStr[ expr->kind ], expr->kind );
      break;
  }
}

//----------------------------------------------------------------
static void _processExprStmt( Node *stmt ) {
  // Semantically check the test expression.
  _processExpr( stmt->expr );
}

//----------------------------------------------------------------
static void _processIf( Node *stmt ) {
  // Semantically check the test expression.
  _processExpr( stmt->test );

  // The type of the test expression must be integer.  If not,
  //  increment the number of errors detected and alert the user
  //  to what happened.
  if ( stmt->test->type != KIND_INT ) {
    _numErrs++;
    printf( "_processIf:  test expr must be of type INTEGER, not %s.\n",
      kindToStr[ stmt->test->type ] );
  }

  // Semantically check the body of the then and else blocks.
  stmt->thenBlock = _processBlock( stmt->thenBlock );
  stmt->elseBlock = _processBlock( stmt->elseBlock );
}
//----------------------------------------------------------------
static void _processRead( Node *stmt )
{
  // Check each item in the exprlist.
  //  * Ensure each is an ID or string literal.
  //  * If ID, determine which var the ID denotes.
  for ( Node *expr = stmt->exprList; expr; expr = expr->listNext ) {
    if ( expr->kind == KIND_ID || expr->kind == KIND_STR ) {
      _processExpr( expr );
    } else {
      _numErrs++;
      printf( "_processRead:  read item must be ID or string literal, not %s.\n",
        kindToStr[ expr->kind ] );
    }
  }
}

//----------------------------------------------------------------
static void _processRepeat( Node *stmt ) {
    inLoop = 1;
    // Semantically check the body of the repeat.
    stmt->block = _processBlock( stmt->block );

    // Semantically check the test expression.
    _processExpr( stmt->test );

    // The type of the test expression must be integer.  If not,
    //  increment the number of errors detected and alert the user
    //  to what happened.
    if ( stmt->test->type != KIND_INT ) {
        _numErrs++;
        printf( "_processRepeat:  test expr must be of type INTEGER, not %s.\n",
        kindToStr[ stmt->test->type ] );
    }


}

//----------------------------------------------------------------
static void _processStmt( Node *stmt )
{
  switch ( stmt->kind ) {
    case KIND_BLOCK    : _processBlock   ( stmt ); break;
    case KIND_BREAK    : _processBreak   (); break;
    case KIND_CONT     : _processContinue(); break;
    case KIND_DECL     : _processDecl    ( stmt ); break;
    case KIND_EXPR     : _processExprStmt( stmt ); break;
    case KIND_IF       : _processIf      ( stmt ); break;
    case KIND_READ     : _processRead    ( stmt ); break;
    case KIND_REPEAT   : _processRepeat  ( stmt ); break;
    case KIND_WHILE    : _processWhile   ( stmt ); break;
    case KIND_WRITE    : _processWrite   ( stmt ); break;

    default : {
      printf( "_processStmt :  WTF?  Don't understand stmt kind %d\n",
        stmt->kind );
      exit( 1 );
    }
  }
}

//----------------------------------------------------------------
static void _processWhile( Node *stmt )
{
  inLoop = 1;

  // Semantically check the test expression.
  _processExpr( stmt->test );

  // The type of the test expression must be integer.  If not,
  //  increment the number of errors detected and alert the user
  //  to what happened.
  if ( stmt->test->type != KIND_INT ) {
    _numErrs++;
    printf( "_processWhile:  test expr must be of type INTEGER, not %s.\n",
      kindToStr[ stmt->test->type ] );
  }

  // Semantically check the body of the while.
  stmt->block = _processBlock( stmt->block );
}

//----------------------------------------------------------------
static void _processWrite( Node *stmt )
{
  // Semantically check each item in the exprlist.
  for ( Node *expr = stmt->exprList; expr; expr = expr->listNext ) {
    _processExpr( expr );
  }
}

//----------------------------------------------------------------
