#include "ast.h"
#include <iostream>

Node::Node(int lin, int col)
{
    this->line = lin;
    this->column = col;
    this->father = NULL;
}

Prog::Prog(Ident * n, Decs * decl, SubDecs * subdecl, CompStmt * comst, int lin, int col) : Node(lin, col)
{
    this->name = n;
    n->father = this;
    this->declarations = decl;
    decl->father = this;
    this->subDeclarations = subdecl;
    subdecl->father = this;
    this->compoundStatment = comst;
    comst->father = this;
}

Ident::Ident(string n, int lin, int col)  : Node(lin, col)
{
    this->name = n;
    this->symbol = NULL;
}

Decs::Decs(int lin, int col) : Node(lin, col)
{
    this->decs = new vector<ParDec*>;
}
void Decs::AddDec(ParDec * parDec)
{
    this->decs->push_back(parDec);
    parDec->father = this;
}
ParDec::ParDec(IdentList* idlst, Type * typ, int lin, int col) : Node(lin, col)
{ 
    this->identList = idlst;
    idlst->father = this;
    this->tp = typ;
    typ->father = this;
}
IdentList::IdentList(Ident* id, int lin, int col) : Node(lin, col)
{
    this->identLst = new vector<Ident*>;
    this->AddIdent(id);
}
void IdentList::AddIdent(Ident* id){
    this->identLst->push_back(id);
    id->father = this;
}
Type::Type(int lin, int col): Node(lin, col)
{

}
StdType::StdType(TypeEnum tp, int lin, int col) : Type(lin, col)
{
    this->type = tp;
}

Array::Array(int beg, int end, StdType* tp, int lin, int col) : Type(lin, col)
{
    this->beginIndex = beg;
    this->endIndex = end;
    this->stdType = tp;
}

SubDecs::SubDecs(int lin, int col) : Node(lin, col)
{
    this->subdecs = new vector<SubDec*>;
}
void SubDecs::AddDec(SubDec * sd)
{
    this->subdecs->push_back(sd);
    sd->father = this;
}

SubDec::SubDec(SubHead* head, CompStmt* cmst, int lin, int col) : Node(lin, col){
    this->subHead = head;
    if(head != NULL)
        head->father = this;
    this->compStmt = cmst;
    if(cmst != NULL)
        cmst->father  = this;
}
SubHead::SubHead(int lin, int col) : Node(lin, col)
{

}
Func::Func(Ident* ident, Args* ags, StdType* tp, int lin, int col) : SubHead(lin, col)
{
    this->id = ident;
    ident->father = this;
    this->args = ags;
    if(args != NULL)
        args->father = this;
    this->typ = tp;
    typ->father = this;
}
Proc::Proc(Ident* ident, Args* ags, int lin, int col) : SubHead(lin, col)
{
    this->id = ident;
    ident->father = this;
    this->args = ags;
    if(args != NULL)
        args->father = this;
}
Args:: Args(ParList* parls, int lin, int col) : Node(lin, col)
{
    this->parList = parls;
    if(parls != NULL)
        parls->father = this;
}
ParList::ParList(ParDec* prDec, int lin, int col):Node(lin, col)
{
    this->parList = new vector<ParDec*>;
    this->AddDec(prDec);
}
void ParList::AddDec(ParDec *prDec) 
{
    this->parList->push_back(prDec);
    prDec->father = this;
}
CompStmt::CompStmt(OptionalStmts* opst, int lin, int col) : Stmt(lin, col){
    this->optitonalStmts = opst;
    if(opst != NULL)
        opst->father = this;
}
OptionalStmts::OptionalStmts(StmtList* stls, int lin, int col) : Node(lin, col)
{
    this->stmtList = stls;
    if(stls != NULL)
        stls->father = this;
}
StmtList::StmtList(Stmt* st, int lin, int col):Node(lin, col)
{
    this->stmts = new vector<Stmt*>;
    this->AddStmt(st);
}
void StmtList::AddStmt(Stmt* st){
    this->stmts->push_back(st);
    st->father = this;
}
Stmt::Stmt(int lin, int col) : Node(lin, col)
{
    
}
Assign::Assign(Var* vr, Exp* ex, int lin, int col): Stmt(lin, col)
{
    this->var = vr;
    vr->father = this;
    this->exp = ex;
    ex->father = this;

}

ProcStmt::ProcStmt(Ident* ident, ExpList* exls, int lin, int col ) : Stmt(lin, col)
{
    this->id = ident;
    ident->father = this;
    this->expls = exls;
    expls->father = this;
}
ExpList::ExpList(Exp* ex, int lin, int col):Node(lin, col){
    this->expList = new vector<Exp*>;
    this->AddExp(ex);
}
void ExpList::AddExp(Exp* ex){
    this->expList->push_back(ex);
    ex->father = this;
}
Exp::Exp(int lin, int col) : Node(lin, col)
{

}
IdExp::IdExp(Ident* ident, int lin, int col) : Exp(lin, col)
{
    this->id = ident;
    ident->father = this;
}
Integer::Integer(int value, int lin, int col) : Exp(lin, col)
{
    this->val=value;
}
Real::Real(float value, int lin, int col) : Exp(lin, col)
{
    this->val=value;
}
Bool::Bool(bool value, int lin, int col) : Exp(lin, col)
{
    this->val=value;
}
Not::Not(Exp* e, int lin, int col): Exp(lin, col)
{
    this->exp = e;
    e->father = this;
}
BinOp::BinOp(Exp* lexp, Exp* rexp, int lin, int col) : Exp(lin, col)
{
    this->leftExp = lexp;
    lexp->father = this;
    this->rightExp = rexp;
    rexp->father = this;
}
Add::Add(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
Sub::Sub(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
Mult::Mult(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
Divide::Divide(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
Mod::Mod(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
GT::GT(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
LT::LT(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
GE::GE(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
LE::LE(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
ET::ET(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
NE::NE(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
And::And(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
Or::Or(Exp* lexp, Exp* rexp, int lin, int col) : BinOp(lexp, rexp, lin, col)
{

}
IfThen::IfThen(Exp* exp , Stmt* st, int lin, int col) : Stmt(lin, col)
{
    this->expr = exp;
    exp->father = this;
    this->stmt = st;
    st->father = this;
}
IfThenElse::IfThenElse(Exp* exp, Stmt* frstSt, Stmt* scndSt, int lin, int col) : Stmt(lin, col)
{
    this->expr = exp;
    exp->father = this;
    this->trueStmt = frstSt;
    frstSt->father = this;
    this->falseStmt = scndSt;
    scndSt->father = this;
}
While::While(Exp* exp, Stmt* st, int lin, int col) : Stmt(lin, col)
{
    this->expr = exp;
    exp->father = this;
    this->stmt = st;
    st->father = this;
}
Var::Var(Ident* ident, int lin, int col) : Node(lin, col)
{
    this->id = ident;
    ident->father = this;
}
ArrayElement::ArrayElement(Ident* ident, Exp* ind, int lin, int col) : Var(ident, lin, col)
{
    this->index = ind;
    ind->father = this;
}

FuncCall::FuncCall(Ident * iden, ExpList *exls, int lin, int col) : Exp(lin, col)
{
    this->id = iden;
    this->exps =exls;
}

void Node::accept(Visitor * v) {
    v->Visit(this);
}

void Stmt::accept(Visitor * v) {
    v->Visit(this);
}

void Prog::accept(Visitor * v) {
    v->Visit(this);
}

void Ident::accept(Visitor * v) {
    v->Visit(this);
}

void Decs::accept(Visitor * v) {
    v->Visit(this);
}

void ParDec::accept(Visitor * v) {
    v->Visit(this);
}

void IdentList::accept(Visitor * v) {
    v->Visit(this);
}

void SubDecs::accept(Visitor * v) {
    v->Visit(this);
}

void SubDec::accept(Visitor * v) {
    v->Visit(this);
}

void SubHead::accept(Visitor * v) {
    v->Visit(this);
}

void Func::accept(Visitor * v) {
    v->Visit(this);
}

void Args::accept(Visitor * v) {
    v->Visit(this);
}

void ParList::accept(Visitor * v) {
    v->Visit(this);
}

void Proc::accept(Visitor * v) {
    v->Visit(this);
}
void FuncCall::accept(Visitor * v) {
    v->Visit(this);
}
void CompStmt::accept(Visitor * v) {
    v->Visit(this);
}

void OptionalStmts::accept(Visitor * v) {
    v->Visit(this);
}

void StmtList::accept(Visitor * v) {
    v->Visit(this);
}

void Var::accept(Visitor * v) {
    v->Visit(this);
}

void Exp::accept(Visitor * v) {
    v->Visit(this);
}

void Assign::accept(Visitor * v) {
    v->Visit(this);
}

void ProcStmt::accept(Visitor * v) {
    v->Visit(this);
}

void ExpList::accept(Visitor * v) {
    v->Visit(this);
}

void IfThen::accept(Visitor * v) {
    v->Visit(this);
}

void IfThenElse::accept(Visitor * v) {
    v->Visit(this);
}

void While::accept(Visitor * v) {
    v->Visit(this);
}

void Type::accept(Visitor * v) {
    v->Visit(this);
}

void StdType::accept(Visitor * v) {
    v->Visit(this);
}

void IdExp::accept(Visitor * v) {
    v->Visit(this);
}

void Integer::accept(Visitor * v) {
    v->Visit(this);
}

void Real::accept(Visitor * v) {
    v->Visit(this);
}

void Bool::accept(Visitor * v) {
    v->Visit(this);
}

void Array::accept(Visitor * v) {
    v->Visit(this);
}
void ArrayElement::accept(Visitor * v) {
    v->Visit(this);
}
void BinOp::accept(Visitor * v) {
    v->Visit(this);
}

void Add::accept(Visitor * v) {
    v->Visit(this);
}

void Sub::accept(Visitor * v) {
    v->Visit(this);
}

void Mult::accept(Visitor * v) {
    v->Visit(this);
}

void Divide::accept(Visitor * v) {
    v->Visit(this);
}

void Mod::accept(Visitor * v) {
    v->Visit(this);
}

void GT::accept(Visitor * v) {
    v->Visit(this);
}

void LT::accept(Visitor * v) {
    v->Visit(this);
}

void GE::accept(Visitor * v) {
    v->Visit(this);
}

void LE::accept(Visitor * v) {
    v->Visit(this);
}

void ET::accept(Visitor * v) {
    v->Visit(this);
}

void NE::accept(Visitor * v) {
    v->Visit(this);
}

void And::accept(Visitor * v) {
    v->Visit(this);
}

void Or::accept(Visitor * v) {
    v->Visit(this);
}

void Not::accept(Visitor * v) {
    v->Visit(this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Symbol Table

Errors* errorStack = new Errors();

FunctionSignature::FunctionSignature(string n, vector<TypeEnum> params, TypeEnum ret = (TypeEnum)-1){
    this->name = n;
    this->returnType = ret;
    this->paramTypes= params;
}

string FunctionSignature::getSignatureString(){
    string res = this->name + '@';
    for(int i = 0; i <  this->paramTypes.size(); i++){
        if(i > 0) res += ",";
        res += TypeEnumToString(this->paramTypes[i]);
    }
    if(this->returnType != (TypeEnum)-1)
        res += ("@" + TypeEnumToString(returnType));
    return res;
}

bool FunctionSignature::matches(vector<TypeEnum> callParams){
    int n = this->paramTypes.size();
    if(callParams.size()!=n)
        return false;
    for(int i = 0; i < n; i++)
        if(this->paramTypes[i] != callParams[i])
            return false;
    return true;
}
Symbol::Symbol(string name, SymbolKind kind, int type)
{
    this->Name = name;
    this->Kind = kind;
    this->Type = type;
    this->funcSig = NULL;
}
Symbol::Symbol(string name, SymbolKind kind, FunctionSignature* sig)
{
    this->Name = name;
    this->Kind = kind;
    this->Type = -1;
    this->funcSig = sig;
}
Scope::Scope()
{
    this->hashTab = new HashTable();
    this->Parent = NULL;
    this->Children = new vector<Scope*>;
}
void Scope::AddChildScope(Scope*  s){
    this->Children->push_back(s);
    s->Parent = this;
}

SymbolTable::SymbolTable(){
    this->Scopes = new vector<Scope*>();
    this->Scopes->push_back(new Scope());
    this->currentScope = this->Scopes->at(0);
    this->rootScope = this->currentScope;
}

bool SymbolTable::AddSymbol(Ident *ident, SymbolKind kind, int type)
{
    
    string key;
    switch(kind){
        case PARAM_VAR:
        key = "par" + ident->name; //parameter var for function or procedure
        break;
        case GLOBAL_VAR:
        key = 'g' + ident->name; // global var
        break;
        case LOCAL_VAR:
        key = 'l' + ident->name;
        break;
        default:
        return false;
        break;
    }
    
    Symbol* temp = this->currentScope->hashTab->GetMember(key);
    
    if(temp){
        errorStack->AddError("Redifintion Of Variable" + ident->name, ident->line, ident->column);
		return false;
    }
    Symbol* newSymbol = new Symbol(ident->name, kind, type);
    this->currentScope->hashTab->AddKey(key, newSymbol);
    ident->symbol = newSymbol;
    return true;
}

bool SymbolTable::AddSymbol(Ident* ident, SymbolKind kind, FunctionSignature* sig){
    string key;
    switch (kind)
    {
    case FUNC:
        key = "f";
        break;
    case PROC:
        key = "p";
        break;
    default:
        return false;
        break;
    }
    key = key + sig->getSignatureString();
    Symbol* temp = this->currentScope->hashTab->GetMember(key);
    
    if(temp){
        if(kind == PROC)
            errorStack->AddError("Redifinition of Procedure: " + ident->name, ident->line, ident->column);
		else
            errorStack->AddError("Redifinition of Function: " + ident->name, ident->line, ident->column);
        return false;
    }
    Symbol* newSymbol = new Symbol(ident->name, kind, sig);
    this->currentScope->hashTab->AddKey(key, newSymbol);
    ident->symbol = newSymbol;
    return true;

}

Symbol *SymbolTable::LookUpSymbol(Ident *ident)
{
    string key;
    Symbol* sym;
    // first look if there exist a parameter variable with that name
    key = "par" + ident->name;
    sym = this->currentScope->hashTab->GetMember(key);
    if(sym){
        ident->symbol = sym;
        return sym;
    }
    key = 'g' + ident->name;
    sym = this->Scopes->at(this->Scopes->size() - 2)->hashTab->GetMember(key);
		if (sym != NULL){
			ident->symbol = sym;
			return sym;
        }

        errorStack->AddError("Undeclared Variable: " + ident->name, ident->line, ident->column );
        return NULL;
}

Symbol* SymbolTable::LookUpSymbol(Ident* ident, SymbolKind kind, vector<TypeEnum> paramTypes){

    if(kind == FUNC)
        errorStack->AddError("Undeclared Function: " + ident->name, ident->line, ident->column );
    else
        errorStack->AddError("Undeclared Procedure: " + ident->name, ident->line, ident->column );

    return NULL;
}

void SymbolTable::NewScope()
{
    Scope* newScope = new Scope();
    this->Scopes->push_back(newScope);
    this->currentScope->AddChildScope(newScope);
    this->currentScope  = newScope;
}

void SymbolTable::CloseScope()
{
    this->currentScope = this->currentScope->Parent;
}
