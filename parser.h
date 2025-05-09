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
     KINT = 258,
     KREAL = 259,
     KBOOL = 260,
     KFUNC = 261,
     KPROC = 262,
     KWHILE = 263,
     KARRAY = 264,
     KIDENT = 265,
     KINTNUM = 266,
     KREALNUM = 267,
     KTRUE = 268,
     KFALSE = 269,
     KPROGRAM = 270,
     KVAR = 271,
     KDO = 272,
     KBEGIN = 273,
     KEND = 274,
     KIF = 275,
     KTHEN = 276,
     KELSE = 277,
     KOF = 278,
     KMOD = 279,
     KNOT = 280,
     KOR = 281,
     KAND = 282,
     KASSIGN = 283,
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
#define KINT 258
#define KREAL 259
#define KBOOL 260
#define KFUNC 261
#define KPROC 262
#define KWHILE 263
#define KARRAY 264
#define KIDENT 265
#define KINTNUM 266
#define KREALNUM 267
#define KTRUE 268
#define KFALSE 269
#define KPROGRAM 270
#define KVAR 271
#define KDO 272
#define KBEGIN 273
#define KEND 274
#define KIF 275
#define KTHEN 276
#define KELSE 277
#define KOF 278
#define KMOD 279
#define KNOT 280
#define KOR 281
#define KAND 282
#define KASSIGN 283
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
typedef union YYSTYPE
#line 13 "parser.y"
{
    // AST Nodes
    Prog* tProg;
    Ident* tIdent;
    Decs* tDecs;
    IdentList* tIdentlist;
    Type* tType;
    StdType* tStdtype;
    Array* tArray;
    SubDecs* tSubdecs;
    SubDec* tSubdec;
    SubHead* tSubhead;
    Func* tFunc;
    Proc* tProc;
    Args* tArgs;
    ParList* tParlist;
    CompStmt* tCompstmt;
    OptionalStmts* tOptionalstmts;
    StmtList* tStmtlist;
    Stmt* tStmt;
    ExpList* tExplist;
    Exp* tExp;
    Var* tVar;
    ProcStmt* tProcstmt;
    BinOp* tBinop;
    While* tWhile;

    Integer*  tInt;
    Real* tReal;
    Bool* tBool;


}
/* Line 1529 of yacc.c.  */
#line 163 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

