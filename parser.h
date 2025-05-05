/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KPROGRAM = 258,
     KVAR = 259,
     KINT = 260,
     KREAL = 261,
     KFUNC = 262,
     KPROC = 263,
     KWHILE = 264,
     KDO = 265,
     KBEGIN = 266,
     KEND = 267,
     KIF = 268,
     KTHEN = 269,
     KELSE = 270,
     KARRAY = 271,
     KOF = 272,
     KDIV = 273,
     KNOT = 274,
     KOR = 275,
     KAND = 276,
     KIDENT = 277,
     KINTNUM = 278
   };
#endif
/* Tokens.  */
#define KPROGRAM 258
#define KVAR 259
#define KINT 260
#define KREAL 261
#define KFUNC 262
#define KPROC 263
#define KWHILE 264
#define KDO 265
#define KBEGIN 266
#define KEND 267
#define KIF 268
#define KTHEN 269
#define KELSE 270
#define KARRAY 271
#define KOF 272
#define KDIV 273
#define KNOT 274
#define KOR 275
#define KAND 276
#define KIDENT 277
#define KINTNUM 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

