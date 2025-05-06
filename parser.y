%{
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
%token KDIV
%token KNOT
%token KOR
%token KAND
%token KASSIGNE
%token KIDENT
%token KINTNUM
%token KTRUE
%token KFALSE


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
sub_declarations: sub_declarations
                | sub_dec ';'
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
stmt: variable KASSIGNE exp
    | proc_stmt
    | comp_stmt
    | KIF exp KTHEN stmt
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
   | KTRUE
   | KFALSE
   | KIDENT '(' exp_list ')'
   | '(' exp ')'
   | exp uniOp exp
   | KNOT exp
;
uniOp: '+'
;
%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s << endl;
    return 0; 
}
