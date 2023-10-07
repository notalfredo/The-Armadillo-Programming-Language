/* 
==================================================================================
==================================================================================
*/

#if !defined( __KEYWORDS_H__ )
#define __KEYWORDS_H__

/* 
==================================================================================
==================================================================================
*/

typedef enum tokens {
    //Found unkown token
    TOKEN_ILLEGAL = 0,


    //Identifier
    TOKEN_LET,
    TOKEN_ID,


    //Identifier Types
    TOKEN_INTEGER,
    TOKEN_REAL,
    TOKEN_STRING,


    //Function
    TOKEN_FUNCTION_DECL,
    TOKEN_RETURN,
    TOKEN_FUNC_TYPE_RETURN,


    //Lit numbers
    TOKEN_LIT_INT,
    TOKEN_LIT_REAL,
    TOKEN_LIT_STR,


    // Binary operators
    TOKEN_BOP_ADD,
    TOKEN_BOP_SUB,
    TOKEN_BOP_MUL,
    TOKEN_BOP_DIV,

    TOKEN_BOP_EQ,
    TOKEN_BOP_NE,

    TOKEN_BOP_LT,
    TOKEN_BOP_LE,
    TOKEN_BOP_GT,
    TOKEN_BOP_GE,

    TOKEN_BOP_ASSIGN,


    // Unary operators
    TOKEN_UOP_NEGATE,


    // Statements
    TOKEN_BLOCK,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_EXPR,

    TOKEN_IF,
    TOKEN_ELIF,
    TOKEN_ELSE,

    TOKEN_FOR,
    TOKEN_WHILE,

    TOKEN_CHECK,
    TOKEN_SCOPE,
    TOKEN_CASE,


} tokens;


typedef struct {
  char *keyword_str;
  tokens token;
} Keyword;


Keyword keywords[] = {
  { "let", TOKEN_LET },
  { "if",  TOKEN_IF  },
  { "elif",  TOKEN_ELIF  },
  { "else",  TOKEN_ELSE  },

  { "real", TOKEN_REAL },
  { "string", TOKEN_STRING },
  { "integer", TOKEN_INTEGER },

  { "scope", TOKEN_SCOPE },
  { "case", TOKEN_CASE },
  { "check", TOKEN_CHECK },

  { "break", TOKEN_BREAK },
  { "continue", TOKEN_CONTINUE },
  { "for", TOKEN_FOR },
  { "while", TOKEN_WHILE },

  { "fn", TOKEN_FUNCTION_DECL },
  { "return", TOKEN_RETURN },

};

#define NUM_KEYWORDS ( sizeof(keywords) / sizeof(Keyword) )

/* 
==================================================================================
==================================================================================
*/
#endif
