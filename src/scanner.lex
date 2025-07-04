%{
    #include "ast.h"
    #include <iostream>
    #include "parser.h"
    using std::endl;
    using std::cout;
    int lin = 0, col = 0;
%}
%option noyywrap

DIGIT [0-9]
ALPHA [a-zA-Z]

%X ML_COMMENT 

%%

[ \t] {
    col += yyleng;
    }
[\n] {
    lin++;
    col = 0;    
}
"//".*  {
    //cout  << "One-Line Comment\n";
}
"{"      { BEGIN(ML_COMMENT); } 

<ML_COMMENT>"}" { BEGIN(INITIAL); } 
<ML_COMMENT>[\n] { lin++; col = 0; } 
<ML_COMMENT>.    { col += yyleng; }


[Pp][Rr][Oo][Gg][Rr][Aa][Mm] {
    col += yyleng;
    return KPROGRAM;
}
[Vv][Aa][Rr] {
    col += yyleng;
    return KVAR;
}
[Ii][Nn][Tt][Ee][Gg][Ee][Rr] {
    col += yyleng;
    return KINT;
}
[Rr][Ee][Aa][Ll] {
    col += yyleng;
    return KREAL;
}
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn] {
    col += yyleng;
    return KFUNC;
}
[Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee] {
    col += yyleng;
    return KPROC;
}

[Ww][Hh][Ii][Ll][Ee] {
    col += yyleng;
    return KWHILE;
}
[Dd][Oo] {
    col += yyleng;
    return KDO;
}
[Bb][Ee][Gg][Ii][Nn] {
    col += yyleng;
    return KBEGIN;
}
[Ee][Nn][Dd] {
    col += yyleng;
    return KEND;
}

[Ii][Ff] {
    col += yyleng;
    return KIF;
}
[Tt][Hh][Ee][Nn] {
    col += yyleng;
    return KTHEN;
}
[Ee][Ll][Ss][Ee] {
    col += yyleng;
    return KELSE;
}
[Aa][Rr][Rr][Aa][Yy] {
    col += yyleng;
    return KARRAY;
}
[Oo][Ff] {
    col += yyleng;
    return KOF;
}
[Dd][Ii][Vv] {
    col += yyleng;
    return KINTDIV;
}
[Nn][Oo][Tt] {
    col += yyleng;
    return KNOT;
}
[Oo][Rr] {
    col += yyleng;
    return KOR;
}
[Aa][Nn][Dd] {
    col += yyleng;
    return KAND;
}
[Bb][Oo][Oo][Ll][Ee][Aa][Nn] {
    col += yyleng;
    return KBOOL;
}
[Tt][Rr][Uu][Ee] {
    col += yyleng;
    yylval.tBool = new Bool(true, lin, col);
    return KTRUE;
}
[Ff][Aa][Ll][Ss][Ee] {
    col += yyleng;
    yylval.tBool = new Bool(false, lin, col);
    return KFALSE;
}
({ALPHA}|_)({ALPHA}|{DIGIT}|_)* {
    col += yyleng;
    yylval.tIdent = new Ident(strdup(yytext), lin, col);
    return KIDENT;
}
0{DIGIT}+ {
    col += yyleng;
    cout <<"Lexical Error(Leading Zeros) at column: " << col << " row: " << lin+1  << endl;
}
 (0|([1-9]{DIGIT}*)) {
    col += yyleng;
    yylval.tInt = new Integer(atoi(yytext), lin, col);
    return KINTNUM;
}

(({DIGIT}+\.{DIGIT}+([e|E][+-]?{DIGIT}+)?)|(\.{DIGIT}+([e|E][+-]?{DIGIT}+)?)|({DIGIT}+[e|E][+-]?{DIGIT}+)) {
    col += yyleng;
    yylval.tReal = new Real(atof(yytext), lin, col);
    return KREALNUM;
}

":=" {
    col += yyleng;
    return KASSIGN;
}
">" {
    col += yyleng;
    return KGT;
}
"<" {
    col += yyleng;
    return KLT;
}
">=" {
    col += yyleng;
    return KGE;
}
"<=" {
    col += yyleng;
    return KLE;
}
"=" {
    col += yyleng;
    return KET;
}
"<>" {
    col += yyleng;
    return KNE;
}
"+" {
    col += yyleng;
    return KADD;
}
"-" {
    col += yyleng;
    return KSUB;
}
"/" {
    col += yyleng;
    return KDIVIDE;
}
"*" {
    col += yyleng;
    return KMULT;
}
".." {
    col += yyleng;
    return *yytext;
}

[(),{};:]|"["|"]" {
    col += yyleng;
    return *yytext;
}



. {
    col += yyleng;
    cout <<"Lexical Error at line: " << lin+1 << " column: " << col  << endl;
    return 1;
    }

%%

