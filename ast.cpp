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
    this->identLst->push_back(id);
    id->father = this;
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
    head->father = this;
    this->compStmt = cmst;
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
    args->father = this;
    this->typ = tp;
    typ->father = this;
}
Proc::Proc(Ident* ident, Args* ags, int lin, int col) : SubHead(lin, col)
{
    this->id = ident;
    ident->father = this;
    this->args = ags;
    args->father = this;
}
Args:: Args(ParList* parls, int lin, int col) : Node(lin, col)
{
    this->parList = parls;
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
    opst->father = this;
}
OptionalStmts::OptionalStmts(StmtList* stls, int lin, int col) : Node(lin, col)
{
    this->stmtList = stls;
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
