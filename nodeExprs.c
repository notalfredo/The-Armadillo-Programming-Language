// Dalio, Brian A.
// dalioba
// 2023-04-12
//----------------------------------------------------------------
#include "node.h"

//----------------------------------------------------------------
// Expressions
//----------------------------------------------------------------
Node *makeBinOp( const int which, Node *o1, Node *o2 )
{
  Node *me = allocNode();

  me->kind  = which;
  me->left  = o1;
  me->right = o2;

  return me;
}


//----------------------------------------------------------------
Node *makeUnaOp( const int which, Node *o1)
{
  Node *me = allocNode();

  me->kind  = which;
  me->left  = o1;

  return me;
}

//----------------------------------------------------------------
