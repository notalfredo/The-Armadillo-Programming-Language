// Dalio, Brian A.
// dalioba
// 2023-04-19
//----------------------------------------------------------------
#include "node.h"

//----------------------------------------------------------------
// Statements
//----------------------------------------------------------------
Node *makeBlock( Node *stmtList )
{
  Node *me = allocNode();
  me->kind = KIND_BLOCK;

  me->stmtList = stmtList;

  return me;
}

//----------------------------------------------------------------
Node *makeBreak()
{
  Node *me = allocNode();
  me->kind = KIND_BREAK;

  return me;
}

//----------------------------------------------------------------
//Node *makeContinue()
Node *makeContinue()
{
  Node *me = allocNode();
  me->kind = KIND_CONT;

  return me;
}

//----------------------------------------------------------------
Node *makeDecl( const int which, Node *id, Node *initExpr )
{
  Node *me = allocNode();
  me->kind = KIND_DECL;

  me->which    = which;
  me->id       = id;
  me->initExpr = initExpr;

  return me;
}

//----------------------------------------------------------------
Node *makeExpr( Node *expr ) {
  Node *me = allocNode();
  me->kind = KIND_EXPR;

  me->expr = expr;

  return me;
}

//----------------------------------------------------------------
Node *makeIf( Node *test, Node *thenBlock, Node *elseBlock ) {
  Node *me = allocNode();
  me->kind = KIND_IF;

  me->test = test;
  me->thenBlock = thenBlock;
  me->elseBlock = elseBlock;

  return me;
}
//----------------------------------------------------------------
Node *makeRead( Node *exprList )
{
  Node *me = allocNode();
  me->kind = KIND_READ;

  me->exprList = exprList;

  return me;
}

//----------------------------------------------------------------
Node *makeRepeat( Node *block, Node *test )
{
  Node *me = allocNode();
  me->kind = KIND_REPEAT;
  me->block = block;
  me->test = test;

  return me;
}

//----------------------------------------------------------------
Node *makeWhile( Node *test, Node *block )
{
  Node *me = allocNode();
  me->kind = KIND_WHILE;

  me->test = test;
  me->block = block;

  return me;
}

//----------------------------------------------------------------
Node *makeWrite( Node *exprList )
{
  Node *me = allocNode();
  me->kind = KIND_WRITE;

  me->exprList = exprList;

  return me;
}

//----------------------------------------------------------------
