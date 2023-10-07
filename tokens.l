/* 
==================================================================================
    DEFINITIONS 
==================================================================================
*/

%option noyywrap

/* C PROGRAM CODE */
%{
#include <stdio.h>
#include <iostream>

#include "keywords.h"

union {
    int intVal;
    float floatVal;
    char* stringVal;
} yylval;

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

[0-9]+ {
    yylval.intVal = (int) strtol(yytext, NULL, 10); 
    return TOKEN_LIT_INT;
}


[0-9]+.[0-9]+ {
    std::cout << "->" << yytext << "<-" << std::endl;
    yylval.floatVal = atof(yytext);
    return TOKEN_LIT_INT;
}




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

#include <stdio.h>
#include <iostream>


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
      inStr = strdup( "fn for 12.0" );
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

        case TOKEN_LET:
            std::cout << "TOKEN_LET" << std::endl;
            break;

        case TOKEN_ID:
            std::cout << "TOKEN_ID" << std::endl;
            break;

        case TOKEN_INTEGER:
            std::cout << "TOKEN_INTEGER" << std::endl;
            break;

        case TOKEN_REAL:
            std::cout << "TOKEN_REAL" << std::endl;
            break;

        case TOKEN_STRING:
            std::cout << "TOKEN_STRING" << std::endl;
            break;

        case TOKEN_FUNCTION_DECL:
            std::cout << "TOKEN_FUNCTION_DECL" << std::endl;
            break;
    
        case TOKEN_RETURN:
            std::cout << "TOKEN_RETURN" << std::endl;
            break;

        case TOKEN_FUNC_TYPE_RETURN:
            std::cout << "TOKEN_FUNC_TYPE_RETURN" << std::endl;
            break;

        case TOKEN_LIT_INT:
            std::cout << "TOKEN_LIT_INT: " << yylval.intVal  << std::endl;
            break;

        case TOKEN_LIT_REAL:
            std::cout << "TOKEN_REAL: " << yylval.floatVal << std::endl;
            break;

        case TOKEN_LIT_STR:
            std::cout << "TOKEN_STRING: " << yylval.stringVal << std::endl;
            break;








        case TOKEN_FOR:
            std::cout << "TOKEN_FOR" << std::endl;
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
