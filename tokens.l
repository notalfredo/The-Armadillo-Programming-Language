/* 
==================================================================================
    DEFINITIONS 
==================================================================================
*/

%option noyywrap

/* C PROGRAM CODE */
%{
#include <stdio.h>

#include "keywords.h"


tokens isKeyword( const char *str );
%}


ID_FIRST    [_a-zA-Z]
ID_AFTER    {ID_FIRST}|[0-9]

/* 
==================================================================================
    RULES 
==================================================================================
*/

%%


{ID_FIRST}{ID_AFTER}* {
  
    tokens myToken = isKeyword( yytext ); 

    if(myToken != TOKEN_ILLEGAL){
        return myToken; 
    }

    return TOKEN_ID;
}



%%

/* 
   ==================================================================================
   USER CODE 
   ==================================================================================
 */

#include "keywords.h"

tokens isKeyword( const char *str ) {

    for( size_t index = 0; index < NUM_KEYWORDS; index ++){
        if( strcmp( str, keywords[index].keyword_str ) == 0 ){
            return keywords[index].token;
        }
    }
    return TOKEN_ILLEGAL;
}



char *getFileContents( const char *fName )
{
  FILE *fp = fopen( fName, "r" );
  if ( fp == NULL ) {
    printf( "Unable to open '%s' for read.\n", fName );
    exit( -1 );
  }

  char   *buffer = NULL;
  size_t  len;
  ssize_t numBytes = getdelim( &buffer, &len, '\0', fp );

  fclose( fp );

  if ( numBytes == -1) {
    printf( "No contents read from '%s'?\n", fName );

    return strdup( "" );

  } else {
    printf( "%d byte%s read from '%s'.\n",
      (int) len, len == 1 ? "" : "s", fName );

    return buffer;
  }
}





//----------------------------------------------------------------
int main( int argc, char *argv[] )
{
  char *inStr;

  switch ( argc ) {
    case 1 :
      inStr = strdup( "fn for" );
      break;

    case 2 :
      inStr = getFileContents( argv[1] );
      break;

    default :
      printf( "Usage: %s [ <inFileName> ]\n", argv[0] );
      return 0;
  }

  printf( "Tokens for \"%s\":\n", inStr );


  //--------------------------------------
  // Tell flex to use this char * as the source of the characters
  //  it should be scanning.
  yy_scan_string( inStr );

  //--------------------------------------
  // Get the tokens one-by-one.
  int tok;

  while ( ( tok = yylex() ) ) {

    switch ( tok ) {

        case TOKEN_FUNCTION_DECL:
            printf("TOKEN_FUNCTION_DECL\n");
            break;

        case TOKEN_FOR:
            printf("TOKEN_FOR\n");
            break;

        default:
          printf( "Unknown token category %d?\n", tok );
          break;
    }
  }

  //--------------------------------------
  // Get rid of the input string.
  free( inStr );

  //--------------------------------------
  // No errors!
  return 0;
}
