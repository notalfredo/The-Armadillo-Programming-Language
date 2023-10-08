// Dalio, Brian A.
// dalioba
// 2023-04-10
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

//----------------------------------------------------------------
// Scopes
//----------------------------------------------------------------
static Node *_scopeList = NULL;

static Node *_currentScope = NULL;

//----------------------------------------------------------------
Node *addNameToScope( const char *name, const int type )
{
  // Get a new ID Node so we can change it.
  Node *id = makeID( name );

  // This declared ID goes on the list of IDs for this scope.
  id->listNext = _currentScope->entryList;
  _currentScope->entryList = id;

  // Ensure the ID remembers which scope it belongs to and its
  //  own type.
  id->scopeNum = _currentScope->scopeNum;
  id->type     = type;

  // All done!
  return id;
}

//----------------------------------------------------------------
void closeScope()
{
  // Ensure there really is a scope to close.
  if ( _currentScope == NULL ) {
    printf( "closeScope : no scope to close?\n" );
    exit( 1 );
  }

  // When we close a scope, the current scope becomes the parent
  //  of that scope.
  _currentScope = _currentScope->parent;
}

//----------------------------------------------------------------
static void _dumpfScope( FILE *fp, Node *scope )
{
  fprintf( fp, "  (SCOPE <num:%d> <parent:%d> <len:%d>\n",
    scope->scopeNum,
    scope->parent == NULL ? -1 : scope->parent->scopeNum,
    nodeListLen( scope->entryList ) );

  for ( Node *id = scope->entryList; id; id = id->listNext ) {
    fprintf( fp, "    ( %s \"%s\" )\n",
      kindToStr[ id->type ], id->strVal );
  }

  fprintf( fp, "  )\n" );
}

void dumpfScopes( FILE *fp )
{
  fprintf( fp, "\n(SCOPES <len:%d>\n",
    nodeListLen( _scopeList ) );

  for ( Node *scope = _scopeList; scope; scope = scope->listNext ) {
    _dumpfScope( fp, scope );
  }

  fprintf( fp, ")\n" );
}

void dumpfScopesDecls( void (*allocVar)( const int, const int, const char * ) )
{
  for ( Node *scope = _scopeList; scope; scope = scope->listNext ) {
    for ( Node *id = scope->entryList; id; id = id->listNext ) {
      allocVar( id->type, scope->scopeNum, id->strVal );
    }
  }
}

//----------------------------------------------------------------
static Node *_findInScope( const char *name, const Node *scope )
{
  // If there's no scope, there can't be anything in it.
  if ( scope == NULL ) return NULL;

  // Look through all of the ids in this scope.
  for ( Node *id = scope->entryList; id; id = id->listNext ) {
    // Here it is!
    if ( strcmp( name, id->strVal ) == 0 ) return id;
  }

  // Nope, wasn't there.
  return NULL;
}

Node *findInCurrentScope( const char *name )
{
  return _findInScope( name, _currentScope );
}

Node *findInScopeHierarchy( const char *name )
{
  // Look through each scope in the hierarchy from the current
  //  scope up through the parent chain.
  for ( Node *scope = _currentScope; scope; scope = scope->parent ) {
    Node *id = _findInScope( name, scope );

    // Found it!
    if ( id != NULL ) return id;
  }

  // Couldn't find the given name in the scope hierarchy.
  return NULL;
}

//----------------------------------------------------------------
void openScope( int scopeNum )
{
  // Allocate a new scope node.
  Node *me = allocNode();

  me->kind     = KIND_SCOPE;
  me->scopeNum = scopeNum;

  // All scopes are on the scope list.
  me->listNext = _scopeList;
  _scopeList   = me;

  // Our parent is whatever scope is current when we are created.
  me->parent = _currentScope;

  // We become the current scope when we are created.
  _currentScope = me;
}

//----------------------------------------------------------------
