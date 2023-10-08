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
    return TOKEN_LIT_REAL;
}

"->" {
    return TOKEN_FUNC_RETURN;
}

["][^"\n"]*["] {
    
    return TOKEN_LIT_STR;
}


":=" { return TOKEN_BOP_ASSIGN; }
"+"  { return TOKEN_BOP_ADD;    }
"-"  { return TOKEN_BOP_SUB;    }
"*"  { return TOKEN_BOP_MUL;    }
"/"  { return TOKEN_BOP_DIV;    }
"="  { return TOKEN_BOP_EQ;     }
"><" { return TOKEN_BOP_NE;     }
"<"  { return TOKEN_BOP_LT;     }
"<=" { return TOKEN_BOP_LE;     }
">"  { return TOKEN_BOP_LT;     }
">=" { return TOKEN_BOP_LE;     }
":"  { return TOKEN_COLON ;     }
";"  { return TOKEN_SEMICOLON;  }
"!"  { return TOKEN_UOP_NEGATE; }
"("  { return TOKEN_LPAREN;     }
")"  { return TOKEN_RPAREN;     }
"{"  { return TOKEN_LBRACE;     }
"}"  { return TOKEN_RBRACE;     }
","  { return TOKEN_COMMA;      }


{ID_FIRST}{ID_AFTER}* {
 

    /* 
        KEYWORDS 

        let integer real string fn return break
        continue if elif else for while check
        scope case   
    */

    tokens myToken = isKeyword( yytext ); 

    if(myToken != TOKEN_ILLEGAL){
        return myToken; 
    }

    return TOKEN_ID;
}

"//".*      { /* Ignore this comment. */ }
[ \f\t\v]+  { /* Ignore this whitespace. */ }
[\r\n]+     { /* ignore newlines */ }


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
      inStr = strdup( "fn foo( x: integer, y: real) -> integer {\r\n let myVar: integer := 12; \r\n return myVar;\r\n }" );
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

        case TOKEN_FUNC_RETURN:
            std::cout << "TOKEN_FUNC_TYPE_RETURN" << std::endl;
            break;

        case TOKEN_LIT_INT:
            std::cout << "TOKEN_LIT_INT: " << yylval.intVal  << std::endl;
            break;

        case TOKEN_LIT_REAL:
            std::cout << "TOKEN_LIT_REAL: " << yylval.floatVal << std::endl;
            break;

        case TOKEN_LIT_STR:
            std::cout << "TOKEN_LIT_STRING: " <<  std::endl;
            break;

        case TOKEN_BOP_ASSIGN:
            std::cout << "TOKEN_BOP_ASSIGN" << std::endl;
            break;

        case TOKEN_BOP_ADD:
            std::cout << "TOKEN_BOP_ADD" << std::endl;
            break;

        case TOKEN_BOP_SUB:
            std::cout << "TOKEN_BOP_SUB" << std::endl;
            break;


        case TOKEN_BOP_MUL:
            std::cout << "TOKEN_BOP_MUL" << std::endl;
            break;

        case TOKEN_BOP_DIV:
            std::cout << "TOKEN_BOP_DIV" << std::endl;
            break;

        case TOKEN_BOP_EQ:
            std::cout << "TOKEN_BOP_EQ" << std::endl;
            break;


        case TOKEN_BOP_NE:
            std::cout << "TOKEN_BOP_NE" << std::endl;
            break;

        case TOKEN_BOP_LT:
            std::cout << "TOKEN_BOP_LT" << std::endl;
            break;

        case TOKEN_BOP_LE:
            std::cout << "TOKEN_BOP_LE" << std::endl;
            break;

        case TOKEN_BOP_GT:
            std::cout << "TOKEN_BOP_GT" << std::endl;
            break;

        case TOKEN_BOP_GE:
            std::cout << "TOKEN_BOP_GE" << std::endl;
            break;

        case TOKEN_COLON:
            std::cout << "TOKEN_COLON" << std::endl;
            break;

        case TOKEN_SEMICOLON:
            std::cout << "TOKEN_SEMICOLON" << std::endl;
            break;

        case TOKEN_UOP_NEGATE:
            std::cout << "TOKEN_UOP_NEGATE" << std::endl;
            break;

        case TOKEN_BREAK:
            std::cout << "TOKEN_BREAK" << std::endl;
            break;

        case TOKEN_CONTINUE:
            std::cout << "TOKEN_CONTINUE" << std::endl;
            break;

        case TOKEN_IF:
            std::cout << "TOKEN_IF" << std::endl;
            break;

        case TOKEN_ELIF:
            std::cout << "TOKEN_ElIF" << std::endl;
            break;

        case TOKEN_ELSE:
            std::cout << "TOKEN_ELSE" << std::endl;
            break;

        case TOKEN_FOR:
            std::cout << "TOKEN_FOR" << std::endl;
            break;

        case TOKEN_WHILE:
            std::cout << "TOKEN_WHILE" << std::endl;
            break;

        case TOKEN_CHECK:
            std::cout << "TOKEN_CHECK" << std::endl;
            break;

        case TOKEN_SCOPE:
            std::cout << "TOKEN_SCOPE" << std::endl;
            break;

        case TOKEN_CASE:
            std::cout << "TOKEN_CASE" << std::endl;
            break;

        case TOKEN_LPAREN:
            std::cout << "TOKEN_LPAREN" << std::endl;
            break;

        case TOKEN_RPAREN:
            std::cout << "TOKEN_RPAREN" << std::endl;
            break;

        case TOKEN_LBRACE:
            std::cout << "TOKEN_LBRACE" << std::endl;
            break;

        case TOKEN_RBRACE:
            std::cout << "TOKEN_RBRACE" << std::endl;
            break;

        case TOKEN_COMMA:
            std::cout << "TOKEN_COMMA" << std::endl;
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
