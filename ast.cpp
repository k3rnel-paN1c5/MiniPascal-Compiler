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
    this->compoundStatment = comst;
}

Ident::Ident(string n, int lin, int col)  : Node(lin, col)
{
    this->name = n;
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
    this->tp = typ;
    idlst->father = this;
}
IdentList::IdentList(Ident* id, int lin, int col) : Node(lin, col)
{
    this->identLst = new vector<Ident*>;
    this->AddIdent(id);
    this->identLst->push_back(id);
    id->father = this;
}
void IdentList::AddIdent(Ident* id){
    this->identLst->push_back(id);
    id->father = this;
}
Type::Type(TypeEnum tp, int lin, int col): Node(lin, col)
{
    this->typ = tp;
}
StdType::StdType(TypeEnum tp, int lin, int col) : Type(tp, lin, col)
{

}

Array::Array(int beg, int end, TypeEnum tp, int lin, int col) : Type(tp, lin, col)
{
    this->beginIndex = beg;
    this->endIndex = end;
}

SubDecs::SubDecs(int lin, int col) : Node(lin, col)
{
    this->subdecs = new vector<SubDec*>;
}
void SubDecs::AddDec(SubDec * sd)
{
    this->subdecs->push_back(sd);
}
SubDec::SubDec(int lin, int col) : Node(lin, col){

}
Func::Func(Ident* ident, Args* ags, StdType* tp, int lin, int col) : SubDec(lin, col)
{
    this->id = ident;
    this->args = ags;
    this->typ = tp;
}
Proc::Proc(Ident* ident, Args* ags, int lin, int col) : SubDec(lin, col)
{
    this->id = ident;
    this->args = ags;
}
Args:: Args(ParList* parls, int lin, int col) : Node(lin, col)
{
    this->parList = parls;
}
ParList::ParList(ParDec* prDec, int lin, int col):Node(lin, col)
{
    this->parList = new vector<ParDec*>;
    this->parList->push_back(prDec);
}
CompStmt::CompStmt(OptionalStmts* opst, int lin, int col) : Stmt(lin, col){
    this->optitonalStmts = opst;
}
OptionalStmts::OptionalStmts(StmtList* stls, int lin, int col) : Node(lin, col)
{
    this->stmtList = stls;
}
StmtList::StmtList(Stmt* st, int lin, int col):Node(lin, col)
{
    this->stmts = new vector<Stmt*>;
    this->AddStmt(st);
}
void StmtList::AddStmt(Stmt* st){
    this->stmts->push_back(st);
}
Stmt::Stmt(int lin, int col) : Node(lin, col)
{
    
}
Assign::Assign(Var* vr, Exp* ex, int lin, int col): Stmt(lin, col)
{
    this->var = vr;
    this->exp = ex;
}
ProcStmt::ProcStmt(Ident* ident, ExpList* exls, int lin, int col ) : Stmt(lin, col)
{
    this->id = ident;
    this->expls = exls;
}
ExpList::ExpList(Exp* ex, int lin, int col):Node(lin, col){
    this->expList = new vector<Exp*>;
    this->AddExp(ex);
}
void ExpList::AddExp(Exp* ex){
    this->expList->push_back(ex);
}
Exp::Exp(int lin, int col) : Node(lin, col)
{

}
IdExp::IdExp(Ident* ident, int lin, int col) : Exp(lin, col)
{
    this->id = ident;
}