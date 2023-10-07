/* 
==================================================================================
    DEFINITIONS 
==================================================================================
*/


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

