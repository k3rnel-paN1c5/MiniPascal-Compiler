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
     KBOOL = 262,
     KFUNC = 263,
     KPROC = 264,
     KWHILE = 265,
     KDO = 266,
     KBEGIN = 267,
     KEND = 268,
     KIF = 269,
     KTHEN = 270,
     KELSE = 271,
     KARRAY = 272,
     KOF = 273,
     KMOD = 274,
     KNOT = 275,
     KOR = 276,
     KAND = 277,
     KASSIGNE = 278,
     KIDENT = 279,
     KINTNUM = 280,
     KREALNUM = 281,
     KTRUE = 282,
     KFALSE = 283,
     KGT = 284,
     KLT = 285,
     KGE = 286,
     KLE = 287,
     KET = 288,
     KNE = 289,
     KMULT = 290,
     KADD = 291,
     KSUB = 292,
     KDIVIDE = 293,
     IF_PREC = 294,
     BINOP = 295
   };
#endif
/* Tokens.  */
#define KPROGRAM 258
#define KVAR 259
#define KINT 260
#define KREAL 261
#define KBOOL 262
#define KFUNC 263
#define KPROC 264
#define KWHILE 265
#define KDO 266
#define KBEGIN 267
#define KEND 268
#define KIF 269
#define KTHEN 270
#define KELSE 271
#define KARRAY 272
#define KOF 273
#define KMOD 274
#define KNOT 275
#define KOR 276
#define KAND 277
#define KASSIGNE 278
#define KIDENT 279
#define KINTNUM 280
#define KREALNUM 281
#define KTRUE 282
#define KFALSE 283
#define KGT 284
#define KLT 285
#define KGE 286
#define KLE 287
#define KET 288
#define KNE 289
#define KMULT 290
#define KADD 291
#define KSUB 292
#define KDIVIDE 293
#define IF_PREC 294
#define BINOP 295




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

