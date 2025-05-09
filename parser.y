%{
    #include "ast.h"
    #include <iostream>
    using std::endl;
    using std::cout;
    extern int yylex();
    extern int yyerror(const char*);
    extern int yydebug;
    extern int lin, col;
%}

%token KPROGRAM
%token KVAR
%token KINT
%token KREAL
%token KBOOL
%token KFUNC
%token KPROC
%token KWHILE
%token KDO
%token KBEGIN
%token KEND
%token KIF
%token KTHEN
%token KELSE
%token KARRAY
%token KOF
%token KMOD
%token KNOT
%token KOR
%token KAND
%token KASSIGN
%token KIDENT
%token KINTNUM
%token KREALNUM
%token KTRUE
%token KFALSE
%token KGT
%token KLT
%token KGE
%token KLE
%token KET
%token KNE
%token KMULT
%token KADD
%token KSUB
%token KDIVIDE

%nonassoc IF_PREC
%nonassoc KELSE

%left KOR 
%left KAND
%left KNOT
%left KET KNE
%left KGT KGE KLT KLE
%left KADD KSUB
%left KMULT KDIVIDE KMOD
%left BINOP   



%%
program: KPROGRAM KIDENT ';' declarations sub_declarations comp_stmt
;
declarations: declarations KVAR ident_list ':' type ';' |
            /* empty */
;
ident_list: KIDENT  |
            ident_list ',' KIDENT
;
type: std_type |
    KARRAY '[' KINTNUM '..' KINTNUM ']' KOF std_type
;
std_type: KINT 
        | KREAL 
        | KBOOL
;
sub_declarations: sub_declarations sub_dec ';'
                | /* empty */
;
sub_dec: sub_head comp_stmt
;
sub_head: KFUNC KIDENT args ':' std_type
        | KPROC KIDENT args ';'
;
args: '(' param_list ')' 
    | /* empty */
;
param_list: ident_list ':' type 
          | param_list ';' ident_list ':' type
;
comp_stmt: KBEGIN optional_stmts KEND
;
optional_stmts: stmt_list 
              | /* empty */
;
stmt_list: stmt 
        | stmt_list ';' stmt
;
stmt: variable KASSIGN exp
    | proc_stmt
    | comp_stmt
    | KIF exp KTHEN stmt %prec IF_PREC
    | KIF exp KTHEN stmt KELSE stmt
    | KWHILE exp KDO stmt
;
variable: KIDENT
        | KIDENT '[' exp ']'
;
proc_stmt: KIDENT
         | KIDENT '(' exp_list ')'
;
exp_list: exp 
        | exp_list ',' exp
;
exp: KIDENT
   | KINTNUM
   | KREALNUM
   | KTRUE
   | KFALSE
   | KIDENT '(' exp_list ')'
   | '(' exp ')'
   | exp binOp exp %prec BINOP
   | KNOT exp

;
binOp: KADD 
     | KSUB 
     | KMULT 
     | KDIVIDE 
     | KMOD 
     | KGT 
     | KGE
     | KLT 
     | KLE 
     | KNE 
     | KET 
     | KAND
     | KOR 
;


%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s << " Line: "<<lin+1 << " Column: " << col << endl;
    return 0; 
}
