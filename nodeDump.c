// Browning, Nathanael
// njb5063
// 2023-04-14
//----------------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>

#include "node.h"

// How many spaces to a tab.
#define TABSTOP (2)

//----------------------------------------------------------------
// static void _irChars( const char *fmt, ... );  // See below!

static void _irLine( const int indent, const char *fmt, ... );
static void _dumpNodeInternal( const int indent, const Node *me );

//----------------------------------------------------------------
// kindToStr[] must be kept in sync with the node kind enumeration
//  in node.h.
char *kindToStr[] = {
  "[UNKNOWN]",

  "INTEGER", "REAL", "STRING", "ID",

  "BOP_ADD", "BOP_DIV", "BOP_SUB",

  "UOP_NOT",

  "BLOCK", "DECL", "IF", "READ", "REPEAT", "WHILE", "WRITE",

  "SCOPE",
};

//----------------------------------------------------------------
// Node dump
//----------------------------------------------------------------
static FILE *_irFP;

void dumpNode( Node *me ) {
  // By default, we dump to stdout.
  dumpfNode( stdout, me );
}

void dumpfNode( FILE *fp, Node *me ) {
  // Remember the output FP in a global space so we don't have to
  //  keep passing it around.
  _irFP = fp;

  // Dump the requested node starting with indent level 0 (at the
  //  left margin).
  _dumpNodeInternal( 0, me );
}

//----------------------------------------------------------------
static void _dumpNodeInternal( const int indent, const Node *me )
{
  switch ( me->kind ) {
    //-- Leaf items ----------------------------------------------
    case KIND_INT :
      _irLine( indent, "(INTEGER <type:\"%s\"> %d)\n",
        kindToStr[ me->type ], me->intVal );
      break;

    case KIND_REAL :
      _irLine( indent, "(REAL <type:\"%s\"> %.17e)\n",
        kindToStr[ me->type ], me->realVal );
      break;

    case KIND_STR :
      _irLine( indent, "(STRING <type:\"%s\"> \"%s\")\n",
        kindToStr[ me->type ], me->strVal );
      break;

    case KIND_ID :
      _irLine( indent, "(ID \"%s\" <scope:%d> <type:\"%s\">)\n",
        me->strVal, me->scopeNum, kindToStr[ me->type ] );
      break;

    //-- Binary operators ----------------------------------------
    case KIND_BOP_ADD : case KIND_BOP_DIV : case KIND_BOP_SUB :
      _irLine( indent, "(%s <type:\"%s\"> \n",
        kindToStr[ me->kind ], kindToStr[ me->type ] );

      _dumpNodeInternal( indent+1, me->left );
      _dumpNodeInternal( indent+1, me->right );

      _irLine( indent, ")\n" );

      break;

    //-- Unary operators -----------------------------------------
    case KIND_UOP_POSITE:
    case KIND_UOP_NEGATE:
    case KIND_UOP_NOT:
      _irLine( indent, "(%s <type:\"%s\"> \n",
        kindToStr[ me->kind ], kindToStr[ me->type ] );

      _dumpNodeInternal( indent+1, me->left );

      _irLine( indent, ")\n" );

      break;

    //-- Statements ----------------------------------------------
    case KIND_BLOCK :
      _irLine( indent, "(BLOCK <num:%d> <len:%d>\n",
        me->blockNum, nodeListLen( me->stmtList ) );

      for ( Node *now = me->stmtList; now; now = now->listNext ) {
        _dumpNodeInternal( indent+1, now );
      }

      _irLine( indent, ")\n" );

      break;

    case KIND_BREAK :
      _irLine( indent, "(BREAK)\n" );
      break;
    case KIND_CONT :
      _irLine( indent, "(CONTINUE)\n" );
      break;

    case KIND_DECL :
      _irLine( indent, "(DECL <type:\"%s\"> <id:\"%s\">\n",
        kindToStr[ me->which ], me->id->strVal );

      _dumpNodeInternal( indent+1, me->initExpr );

      _irLine( indent, ")\n" );

      break;

    case KIND_EXPR :
      _irLine( indent, "(EXPR\n" );

      _dumpNodeInternal( indent+1, me->expr );

      _irLine( indent, ")\n" );

      break;

    case KIND_IF :
      _irLine( indent, "(IF\n" );

      _dumpNodeInternal( indent+1, me->test );
      _dumpNodeInternal( indent+1, me->thenBlock );

      _irLine( indent, "(ELSE\n" );
      _dumpNodeInternal( indent+1, me->elseBlock );
      _irLine( indent, ")\n" );

      break;

    case KIND_WHILE :
      _irLine( indent, "(WHILE\n" );

      _dumpNodeInternal( indent+1, me->test );
      _dumpNodeInternal( indent+1, me->block );

      _irLine( indent, ")\n" );

      break;

    case KIND_READ : case KIND_WRITE :
      _irLine( indent, "(%s <len:%d>\n",
        kindToStr[ me->kind], nodeListLen( me->exprList ) );

      for ( Node *now = me->exprList; now; now = now->listNext ) {
        _dumpNodeInternal( indent+1, now );
      }

      _irLine( indent, ")\n" );

      break;

    case KIND_REPEAT :
      _irLine( indent, "(REPEAT\n" );

      _dumpNodeInternal( indent+1, me->block );


      _irLine( indent, " UNTIL \n" );
      _dumpNodeInternal( indent+1, me->test );

      _irLine( indent, ")\n" );

      break;

    //-- Unknown node kind? --------------------------------------
    default :
      printf( "dumpNode:  WTF?  Don't understand node kind %d\n",
        me->kind );

      _irLine( indent, "(WTF-%d)\n", me->kind );

      break;
  }
}

//----------------------------------------------------------------
/** Not used at present, but maybe someday someone will want
      this routine.  We therefore just comment it out.

static void _irChars( const char *fmt, ... )
{
  // Issue some characters to the IR file without preceding
  //  indent.  For convenience, we interpret the fmt as a printf
  //  format string and consume subsequent arguments as required.
  va_list ap;
  va_start( ap, fmt );
  vfprintf( _irFP, fmt, ap );
  va_end( ap );
}
**/

static void _irLine( const int indent, const char *fmt, ... )
{
  // Indent as many levels as requested.  Each level is
  //  TABSTOP spaces wide.
 if ( indent > 0 ) fprintf( _irFP, "%*s", indent*TABSTOP, " " );

  // Now issue the characters to the IR file.  For convenience,
  //  we interpret the fmt as a printf format string and consume
  //  subsequent arguments as required.
  va_list ap;
  va_start( ap, fmt );
  vfprintf( _irFP, fmt, ap );
  va_end( ap );
}

//----------------------------------------------------------------
