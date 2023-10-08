// Dalio, Brian A.
// dalioba
// 2023-04-12
//----------------------------------------------------------------
#include <string.h>

#include "node.h"

//----------------------------------------------------------------
// Make leaf items: IDs and Literals
//----------------------------------------------------------------
Node *makeID( const char *value )
{
  // We remember the name of an ID the same way we remember a
  //  string literal, we just don't have to strip off any
  //  quotation marks.
  Node *me = makeStrLit( value, 0 );

  // But we are an ID, a name, not a string literal.
  me->kind = KIND_ID;

  return me;
}

Node *makeIntLit( const int value )
{
  Node *me = allocNode();

  me->kind = KIND_INT;
  me->intVal = value;

  return me;
}

Node *makeRealLit( const double value )
{
  Node *me = allocNode();

  me->kind = KIND_REAL;
  me->realVal = value;

  return me;
}


Node *makeStrLit( const char *value, const int strip )
{
  Node *me = allocNode();

  me->kind = KIND_STR;

  // We have to make a copy of the passed value so any changes to
  //  the original value don't affect the literal we're making
  //  here.
  if ( strip ) {
    // "strip" means to omit the first and last characters of the
    //  passed string.  This is useful if the value is known to
    //  have, e.g., " characters at the front and back.
    me->strVal = strdup( value+1 );
    me->strVal[strlen(me->strVal)-1] = '\0';
  } else {
    // No strip.  Copy the entire string.
    me->strVal = strdup( value );
  }

  return me;
}

//----------------------------------------------------------------
