// Dalio, Brian A.
// dalioba
// 2022-04-26
//----------------------------------------------------------------
#include <stdlib.h>

#include "node.h"

//----------------------------------------------------------------
// Node allocation and freeing
//----------------------------------------------------------------
static Node *_allocatedNodes = NULL;

Node *allocNode()
{
  // Allocate a zeroed out Node structure.
  Node *me = (Node *) calloc( 1, sizeof( Node ) );

  // If we didn't get it, weep uncontrollably and terminate the
  //  process.
  if ( me == NULL ) {
    printf( "Unable to allocate a Node (of size %zu)\n.",
      sizeof( Node ) );
    exit( -1 );
  }

  // Push me onto the front of the allocated nodes list.
  me->_____ = _allocatedNodes;
  _allocatedNodes = me;

  // Return a pointer to the spiffy new Node structure.
  return me;
}

void freeNode( Node *me )
{
  switch ( me->kind ) {
    // String values are copied with strdup(), so they have to
    //  be freed.
    case KIND_STR : case KIND_ID :
      free( me->strVal );
      break;

    //-- Some kind of node without internal memory alloc ---------
    default :
      break;
  }

  // Now that anything inside of me is properly handled, me
  //  can be freed.
  free( me );
}

void freeAllNodes()
{
  // Chain down the linked list of all allocated nodes, freeing
  //  as we go.
  Node *next = NULL;

  for ( Node *now = _allocatedNodes; now; now = next ) {
    // Remember who's next to be freed.  (This field could get
    //  wiped by freeNode(), so we get a copy beforehand.)
    next = now->_____;

    // Tidily free the current node and all of its included parts.
    freeNode( now );
  }
}

//----------------------------------------------------------------
// Append node to end of given node list
//----------------------------------------------------------------
Node *appendToNodeList( Node *list, Node *me )
{
  Node *orig = list;

  if ( list == NULL ) {
    return NULL;
  }

  while ( list->listNext != NULL ) {
    list = list->listNext;
  }

  list->listNext = me;

  return orig;
}

//----------------------------------------------------------------
// Number of nodes in a nodelist -- Does NOT detect if list loops
//  itself!  Therefore, possible infinite loop here.
//----------------------------------------------------------------
int nodeListLen( Node *me )
{
  return me == NULL ? 0 : 1 + nodeListLen( me->listNext );
}

//----------------------------------------------------------------
// Some node characterization / manipulation functions.
//----------------------------------------------------------------
int isLeaf( Node *me )
{
  return
    me->kind == KIND_INT ||
    me->kind == KIND_STR || me->kind == KIND_ID;
}

//----------------------------------------------------------------
