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
    LocalDecs* tLocalDec;
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
%token KINTDIV
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
%type <tLocalDec> local_dec
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
// %type <tOp> binOp




%nonassoc IF_PREC
%nonassoc KELSE

// %left BINOP   
%left KOR
%left KAND
%nonassoc KNOT 
%nonassoc KET KNE KLT KGT KLE KGE
%left KADD KSUB
%left KMULT KDIVIDE KINTDIV
%right KUMINUS 



%%
program: KPROGRAM KIDENT ';' kw declarations sub_declarations comp_stmt 
    {
        $$ = new Prog($2, $5, $6, $7, lin, col);
        root = $$;
    }
;
kw: KVAR | /* empty */
;
declarations: declarations  ident_list ':' type ';' 
    {
        $$ = $1;
        ParDec* parDec = new ParDec($2, $4, lin, col);
        $$->AddDec(parDec); 
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
    | KARRAY '[' KSUB KINTNUM '..' KSUB KINTNUM ']' KOF std_type
    {
        $$ = new Array(-1 * $4->val, -1 * $7->val, $10, lin, col);
    }
    | KARRAY '[' KSUB KINTNUM '..' KINTNUM ']' KOF std_type
    {
        $$ = new Array(-1 * $4->val, $6->val, $9, lin, col);
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
        $$ = new SubDecs(lin, col);
    }
;
sub_dec:  sub_head kw local_dec comp_stmt
    {
        $$ = new SubDec($1, $3, $4, lin, col);
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
    |'(' /* empty */ ')'
    {
        $$ = NULL;
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
local_dec: local_dec ident_list ':' type ';' 
        {
            $$ = $1;
            LocalDec* newDec = new LocalDec($2, $4,lin, col);
            $$->AddDec(newDec);
        }
        | /* empty */
        {
            // $$ = NULL;
            $$ = new LocalDecs(lin, col);
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
        $$ = new ArrayElement($1, $3, lin, col);
    }
;
proc_stmt: KIDENT  '('  ')'
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
        $$ = new IdExp($1, lin, col);
    }
    | KINTNUM
    {
        $$ = new Integer($1->val, lin, col);
    }
    | KREALNUM
    {
        $$ = new Real($1->val, lin, col);
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
    | KIDENT '(' ')'
    {
        $$ = new FuncCall($1, NULL, lin, col);

    }
    | '(' exp ')'
    {
        $$ = $2;
    }
    | exp KADD exp     { $$ = new Add($1, $3, lin, col); }
    | exp KSUB exp     { $$ = new Sub($1, $3, lin, col); }
    | exp KMULT exp    { $$ = new Mult($1, $3, lin, col); }
    | exp KDIVIDE exp  { $$ = new Divide($1, $3, lin, col); }
    | exp KINTDIV exp     { $$ = new IntDiv($1, $3, lin, col); }
    | exp KOR exp      { $$ = new Or($1, $3, lin, col); }
    | exp KAND exp     { $$ = new And($1, $3, lin, col); }
    | exp KET exp      { $$ = new ET($1, $3, lin, col); }
    | exp KNE exp      { $$ = new NE($1, $3, lin, col); }
    | exp KLT exp      { $$ = new LT($1, $3, lin, col); }
    | exp KGT exp      { $$ = new GT($1, $3, lin, col); }
    | exp KLE exp      { $$ = new LE($1, $3, lin, col); }
    | exp KGE exp      { $$ = new GE($1, $3, lin, col); }
    | KNOT exp         { $$ = new Not($2, lin, col); }
    | KSUB exp %prec KUMINUS { $$ = new UnaryMinus($2, lin, col); } 


;


%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s << " Line: "<<lin+1 << " Column: " << col << endl;
    return 0; 
}
