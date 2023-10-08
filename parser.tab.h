/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 33 "parser.y"

#include "node.h"

  typedef void *yyscan_t;

#line 55 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_LET = 258,               /* TOKEN_LET  */
    TOKEN_BOP_ASSIGN = 259,        /* TOKEN_BOP_ASSIGN  */
    TOKEN_INTEGER = 260,           /* TOKEN_INTEGER  */
    TOKEN_REAL = 261,              /* TOKEN_REAL  */
    TOKEN_STRING = 262,            /* TOKEN_STRING  */
    TOKEN_BREAK = 263,             /* TOKEN_BREAK  */
    TOKEN_CONTINUE = 264,          /* TOKEN_CONTINUE  */
    TOKEN_REPEAT = 265,            /* TOKEN_REPEAT  */
    TOKEN_WHILE = 266,             /* TOKEN_WHILE  */
    TOKEN_IF = 267,                /* TOKEN_IF  */
    TOKEN_ELIF = 268,              /* TOKEN_ELIF  */
    TOKEN_ELSE = 269,              /* TOKEN_ELSE  */
    TOKEN_ID = 270,                /* TOKEN_ID  */
    TOKEN_LIT_INT = 271,           /* TOKEN_LIT_INT  */
    TOKEN_LIT_REAL = 272,          /* TOKEN_LIT_REAL  */
    TOKEN_LIT_STR = 273,           /* TOKEN_LIT_STR  */
    NEGATE = 274,                  /* NEGATE  */
    POSITE = 275,                  /* POSITE  */
    PERCENT = 276                  /* PERCENT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 55 "parser.y"

    Node                                   *node;
    NodeStatement                          *stmt;
    StatementList                          *stmtList;
    NodeExpression                         *expr;
    NodeInteger                            *intVal;
    NodeDouble                             *doubleVal;
    NodeString                             *stringVal;
    NodeIdentifier                         *id;
    NodeMethodCall                         *methodCall;
    NodeBinaryOperator                     *bop;
    NodeAssignment                         *assignment;
    NodeBlock                              *block;
    NodeExpressionStatement                *exprStmt;
    NodeReturnStatement                    *returnStmt;
    NodeVariableDeclaration                *varDecl;
    NodeIfDeclaration                      *ifDecl;
    NodeElifDeclaration                    *ElifDecl;

#line 113 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (yyscan_t scanner, void **result);

/* "%code provides" blocks.  */
#line 23 "parser.y"

  // Tell Flex the expected prototype of yylex.
#define YY_DECL                                 \
  int yylex (YYSTYPE* yylval, YYLTYPE *yylloc, void* scanner)
  YY_DECL;

#line 148 "parser.tab.h"

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
