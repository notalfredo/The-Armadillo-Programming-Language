// Dalio, Brian A.
// dalioba
// 2023-04-10
//----------------------------------------------------------------
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"
#include "lexer.yy.h"

#include "node.h"

#include "exitStates.h"

// Defined in the lexer.
extern int  yycolno;
extern int  numIllegalChars;

// Semantic processor, in semantic.c.
extern Node *semantic( Node *root );

// Forward decls.
static int _isPositive( char *str );

//----------------------------------------------------------------
int main( int argc, char *argv[] )
{
  int exitStatus = ES_OK;

  //-- Argument processing ---------------------------------------
  // Get the program's name without the preceding path, if any.
  //  If you're on a defective OS that doesn't use / as the path
  //  separator, change the #define.
#define PATH_SEPARATOR ('/')
  char *progName = strrchr( argv[0], PATH_SEPARATOR );
  progName = progName == NULL ? argv[0] : progName+1;

  // The names of a source file and a place to put the IR
  //  are required.
  if ( argc != 3 ) {
    // Incorrect number of args!
    printf(
      "Usage: %s <SRC file name> <IR file name>\n",
      progName );
    exit( ES_WRONGNUMARGS );
  }

  // Ensure we can open the source file for input.
  char *srcFileName = argv[1];
  FILE *srcFP       = fopen( srcFileName, "r" );
  if ( srcFP == NULL ) {
    printf( "%s : Unable to open \"%s\" for input.  (%d) %s.\n",
      progName, srcFileName, errno, strerror( errno ) );

    exit( ES_NOSRCFILE );
  }

  // Ensure we can open the IR file for output.
  char *irFileName = argv[2];
  FILE *irFP       = fopen( irFileName, "w" );
  if ( irFP == NULL ) {
    printf( "%s : Unable to open \"%s\" for output.  (%d) %s.\n",
      progName, irFileName, errno, strerror( errno ) );

    exit( ES_NOIRFILE );
  }

  //-- Get ready for the parse -----------------------------------
  yyscan_t scanner;

  // Initialize the Lexical Analyzer (the scanner).
  if ( yylex_init( &scanner ) ) {
    printf(
      "%s : Unable to initialize the lexical analyzer.  Error is (%d) \"%s\".\n",
      progName, errno, strerror( errno ) );

    exit( ES_NOSCANNER );
  }

  printf( "%s : Processing \"%s\" ...\n",
    progName, srcFileName );

  // Read the input characters from the already opened input file.
  yyrestart( srcFP, scanner );

  // Initialize the current location.
  //  flex updates yylineno automatically, but we have to
  //    do the initialization.
  //  flex itself doesn't do anything with column numbers so we
  //    track those on our own.
  yyset_lineno( 1, scanner );
  yycolno  = 1;

  // Lexer hasn't seen any illegal characters yet.
  numIllegalChars = 0;

  // Set parsing debugging on if the environment variable YYDEBUG
  //  exists and its value is interpretable as positive.
  yydebug = _isPositive( getenv( "YYDEBUG" ) );

  //-- Do the parse! ---------------------------------------------
  // We use yyparse() to do the parse and it returns a state, 0 if
  //  no error, non-zero if there was an error.
  // The lexer counts how many illegal characters were ignored.
  void *result = NULL;
  int parseState = yyparse( scanner, &result );

  // Tell the user what happened.
  printf( "%s : %d illegal character%s seen.\n",
    progName,
    numIllegalChars, numIllegalChars == 1 ? "" : "s" );
  printf( "%s : Parse state is %d.\n",
    progName, parseState );

  if ( parseState != 0 ) {
    exitStatus = ES_PARSEFAILED;
    goto getOut;   // Read note below at the label.
  }

  //-- Semantic processing ---------------------------------------
  result = semantic( result );

  if ( result == NULL ) {
    exitStatus = ES_SEMANTICFAILED;
    goto getOut;   // Read note below at the label.
  }

  // Dump the intermediate representation.
  dumpfNode( irFP, result );
  dumpfScopes( irFP );

  //-- All done! -------------------------------------------------
getOut: // A heinous GOTO target!  Argh, but C doesn't really
        //  have a clean way of doing this sort of control
        //  structure.

  // Close any open files.
  fclose( srcFP );
  fclose( irFP );

  // Get rid of the allocated scanner so we're not a scummy
  //  memory leaker.  :)
  yylex_destroy( scanner );

  // Now jettison the allocated nodes.
  freeAllNodes();

  // parseState is whether there was a parse error or not.
  return exitStatus;
}

//----------------------------------------------------------------
// Various ways to make a "positive" response.
static char *_posWords[] = {
  "1", "y", "on", "yes", "oui",
};

#define NUM_POSITIVE_WORDS ((int)(sizeof(_posWords)/sizeof(char *)))

//--------------------------------------
static int _caselessEqual( const char *l, const char *r )
{
  // If the strings aren't the same length, they can't be equal.
  if (strlen( l ) != strlen( r ) ) return 0;

  // Compare corresponding characters, each converted to lower
  //  case first.
  for ( int i=0; l[i] != '\0'; i++ ) {
    // Not the same, therefore not equal strings.
    if ( tolower( l[i] ) != tolower( r[i] ) ) return 0;
  }

  // All characters matched!
  return 1;
}

static int _isPositive( char *str )
{
  // Returns 1 if the string indicated by str can be interpreted
  //  as "positive".  Add additional alternatives to _posWords[]
  //  if you think of them.

  // NULL or empty string is not "positive".
  if ( str == NULL || str[0] == '\0' ) return 0;

  // Check all the ways we currently correlate to "positive".
  for (int i=0; i < NUM_POSITIVE_WORDS; i++ ) {
    // Matched, therefore str is "positive".
    if ( _caselessEqual( str, _posWords[i]) ) return 1;
  }

  // Well, nothing matched.  Report not "positive".
  return 0;
}

//----------------------------------------------------------------
