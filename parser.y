%{
    #include "ast.h"
    #include <iostream>
    using std::endl;
    using std::cout;

    extern int yylex();
    extern int yyerror(const char*);
    extern int yydebug;
    
    extern int lin, col;
    Prog* root;

    SymbolTable *symbolTable = new SymbolTable();
%}

%union {
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
    While* tWhile;

    Integer*  tInt;
    Real* tReal;
    Bool* tBool;

    enum OpType tOp;
}

%token <tStdtype> KINT KREAL KBOOL
%token <tFunc> KFUNC
%token <tProc> KPROC
%token <tWhile> KWHILE
%token <tArray> KARRAY
%token <tIdent> KIDENT
%token <tInt> KINTNUM
%token <tReal> KREALNUM
%token <tBool> KTRUE KFALSE
%token KPROGRAM
%token KVAR
%token KDO
%token KBEGIN
%token KEND
%token KIF
%token KTHEN
%token KELSE
%token KOF
%token KMOD
%token KNOT
%token KOR
%token KAND
%token KASSIGN
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

%type <tProg> program
%type <tDecs> declarations
%type <tSubdecs> sub_declarations
%type <tSubdec> sub_dec
%type <tSubhead> sub_head
%type <tIdentlist> ident_list
%type <tType> type
%type <tStdtype> std_type
%type <tArgs> args
%type <tParlist> param_list
%type <tCompstmt> comp_stmt
%type <tOptionalstmts> optional_stmts
%type <tStmtlist> stmt_list
%type <tStmt> stmt
%type <tVar> variable
%type <tProcstmt> proc_stmt
%type <tExplist> exp_list
%type <tExp> exp
%type <tOp> binOp




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
    {
        $$ = new Prog($2, $4, $5, $6, lin, col);
        root = $$;
    }
;
declarations: declarations KVAR ident_list ':' type ';' 
    {
        $$ = $1;
        ParDec* parDec = new ParDec($3, $5, lin, col);
        $$->AddDec(parDec); 
        for(int i = 0; i < $3->identLst->size(); i++)
            symbolTable->AddSymbol($3->identLst->at(0), GLOBAL_VAR, $5);
    }
    | /* empty */
    {
        $$ = new Decs(lin, col);
    }
;
ident_list: KIDENT 
    {
        $$ = new IdentList($1, lin, col);
    }
    | ident_list ',' KIDENT
    {
        $$ = $1;
        $$->AddIdent($3);
    }
;
type: std_type 
    {
        $$ = $1
    }
    | KARRAY '[' KINTNUM '..' KINTNUM ']' KOF std_type
    {
        $$ = new Array($3->val, $5->val, $8, lin, col);
    }
;
std_type: KINT 
    {
        $$ = new StdType(INTTYPE, lin, col);
    }
    | KREAL 
    {
        $$ = new StdType(REALTYPE, lin, col);
    }
    | KBOOL
    {
        $$ = new StdType(BOOLTYPE, lin, col);
    }
;
sub_declarations: sub_declarations sub_dec ';'
    {
        $$ = $1;
        $$->AddDec($2);
        
    }
    | /* empty */
    {
        //cout << "reduced sub_declarations to empty\n";
        $$ = new SubDecs(lin, col);
    }
;
sub_dec: sub_head comp_stmt
    {
        $$ = new SubDec($1, $2, lin, col);
    }
;
sub_head: KFUNC KIDENT args ':' std_type ';'
    {
        $$ = new Func($2, $3, $5, lin, col);
    }
    | KPROC KIDENT args ';'
    {
        $$ = new Proc($2, $3, lin, col);
    }
;
args: '(' param_list ')' 
    {
        $$ =  new Args($2, lin, col);
    }
    | /* empty */
    {
        //cout << "args reduced to empty\n";
    }
;
param_list: ident_list ':' type 
    {
        ParDec* parDec = new ParDec($1, $3, lin, col);
        $$ = new ParList(parDec, lin, col);
    }
    | param_list ';' ident_list ':' type
    {
        $$ = $1;
        ParDec* parDec = new ParDec($3, $5, lin, col);
        $$->AddDec(parDec);
    }
;
comp_stmt: KBEGIN optional_stmts KEND
    {
        $$ = new CompStmt($2, lin, col);
    }
;
optional_stmts: stmt_list 
    {
        $$ = new OptionalStmts($1, lin, col);
    }
    | /* empty */
    {
        //cout << "Reduced optional_stmts to empty\n";
    }
;
stmt_list: stmt 
    {
        $$ = new StmtList($1, lin, col);
    }
    | stmt_list ';' stmt
    {
        $$ = $1;
        $$->AddStmt ($3);
    }
;
stmt: variable KASSIGN exp
    {
        $$ = new Assign($1, $3, lin, col);
    }
    | proc_stmt
    {
        $$ = $1;
    }
    | comp_stmt
    {
        $$ = $1;
    }
    | KIF exp KTHEN stmt %prec IF_PREC
    {
        $$ = new IfThen($2, $4, lin, col);
    }
    | KIF exp KTHEN stmt KELSE stmt
    {
        $$ = new IfThenElse($2, $4, $6, lin, col);
    }
    | KWHILE exp KDO stmt
    {
        $$ = new While($2, $4, lin, col);
    }
;
variable: KIDENT
    {
        $$ = new Var($1, lin, col);
    }
    | KIDENT '[' exp ']'
    {   
        $$ = new ArrayElement($1, $3, lin, col)
    }
;
proc_stmt: KIDENT
    {
        $$ = new ProcStmt($1, NULL, lin, col);
    }
    | KIDENT '(' exp_list ')'
    {
        $$ = new ProcStmt($1, $3, lin, col);
    }
;
exp_list: exp 
    {
        $$ = new ExpList($1, lin, col);
    }
    | exp_list ',' exp
    {
        $$ = $1;
        $$->AddExp($3);
    }
;
exp: KIDENT
    {
        $$ = new IdExp($1, lin, col)
    }
    | KINTNUM
    {
        $$ = $1;
    }
    | KREALNUM
    {
        $$ = $1;
    }
    | KTRUE
    {
        $$ = $1;
    }
    | KFALSE
    {
        $$ = $1;
    }
    | KIDENT '(' exp_list ')'
    {
        $$ = new FuncCall($1, $3, lin, col);
    }
    | '(' exp ')'
    {
        $$ = $2;
    }
    | exp binOp exp %prec BINOP
    {
       switch ($2) {
            case OP_ADD:  $$ = new Add($1, $3, lin, col); break;
            case OP_SUB:  $$ = new Sub($1, $3, lin, col); break;
            case OP_MULT: $$ = new Mult($1, $3, lin, col); break;
            case OP_DIV:  $$ = new Divide($1, $3, lin, col); break;
            case OP_MOD:  $$ = new Mod($1, $3, lin, col); break;
            case OP_GT:   $$ = new GT($1, $3, lin, col); break;
            case OP_GE:   $$ = new GE($1, $3, lin, col); break;
            case OP_LT:   $$ = new LT($1, $3, lin, col); break;
            case OP_LE:   $$ = new LE($1, $3, lin, col); break;
            case OP_ET:   $$ = new ET($1, $3, lin, col); break;
            case OP_NE:   $$ = new NE($1, $3, lin, col); break;
            case OP_AND:  $$ = new And($1, $3, lin, col); break;
            case OP_OR:   $$ = new Or($1, $3, lin, col); break;
        }
    }
    | KNOT exp
    {
        $$ = new Not($2, lin, col);
    }

;
binOp: KADD    { $$ = OP_ADD; }
     | KSUB    { $$ = OP_SUB; }
     | KMULT   { $$ = OP_MULT; }
     | KDIVIDE { $$ = OP_DIV; }
     | KMOD    { $$ = OP_MOD; }
     | KGT     { $$ = OP_GT; }
     | KGE     { $$ = OP_GE; }
     | KLT     { $$ = OP_LT; }
     | KLE     { $$ = OP_LE; }
     | KET     { $$ = OP_ET; }
     | KNE     { $$ = OP_NE; }
     | KAND    { $$ = OP_AND; }
     | KOR     { $$ = OP_OR; } 
;


%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s << " Line: "<<lin+1 << " Column: " << col << endl;
    return 0; 
}
