#include "ast.h"
#include <iostream>
using namespace std;

void PrintVisitor::Visit(Node *n)
{
    n->accept(this);
}

void PrintVisitor::Visit(Prog *n)
{
    cout << "Prog: '"<< n->name->name << "'" << endl;
    if (n->declarations) n->declarations->accept(this);
    if (n->subDeclarations) n->subDeclarations->accept(this);
    if (n->compoundStatment) n->compoundStatment->accept(this);
}

void PrintVisitor::Visit(Stmt *s)
{
    cout << "Stmt" << endl;
}

void PrintVisitor::Visit(Ident *n)
{
}


void PrintVisitor::Visit(Decs *n)
{
    cout << "Decs, Size: " << n->decs->size() <<endl;
     for (int i = 0; i < n->decs->size(); i++) {
        n->decs->at(i)->accept(this);
    }
}


void PrintVisitor::Visit(ParDec *n)
{
    cout << "ParDec" <<endl;
    n->identList->accept(this);
    if (n->tp) n->tp->accept(this);
}

void PrintVisitor::Visit(IdentList *n)
{
    cout << "IdentList, Size: " << n->identLst->size() <<endl;
    for (int i = 0; i < n->identLst->size(); i++) {
        n->identLst->at(i)->accept(this);
    }
}

void PrintVisitor::Visit(SubDecs *n)
{
    cout << "SubDecs, Size: " << n->subdecs->size() <<endl;
    for (int i = 0; i < n->subdecs->size(); i++) {
        n->subdecs->at(i)->accept(this);
    }
}
void PrintVisitor::Visit(SubDec *n)
{
    n->subHead->accept(this);
    if(n->compStmt)  n->compStmt->accept(this);

}

void PrintVisitor::Visit(SubHead *n)
{
    n->accept(this);
}

void PrintVisitor::Visit(Func *n)
{
    cout << "Func: " << n->id->name << endl;
    if (n->args) n->args->accept(this);
    if (n->typ) n->typ->accept(this);
}

void PrintVisitor::Visit(Args *n)
{
    cout << "Args" << endl;
}
void PrintVisitor::Visit(ParList *n)
{
    cout << "ParList, Size: " <<  n->parList->size() << endl;
    for (int i = 0;i< n->parList->size(); i++) {
        n->parList->at(i)->accept(this);
    }
}

void PrintVisitor::Visit(Proc *n)
{
    cout << "Proc: " << n->id->name << endl;
    n->args->accept(this);
}
void PrintVisitor::Visit(FuncCall* a)
{
    cout << "Function Call: " << a->id->name <<endl;
    a->exps->accept(this);
}
void PrintVisitor::Visit(CompStmt *n)
{
    cout << "CompStmt" << endl;
    if(n->optitonalStmts) 
        n->optitonalStmts->accept(this);
}

void PrintVisitor::Visit(OptionalStmts *n)
{
    cout << "OptionalStmts" << endl;
    if (n->stmtList) n->stmtList->accept(this);
}

void PrintVisitor::Visit(StmtList *n)
{
    cout << "StmtList, Size: " << n->stmts->size() << endl;
    for (int i = 0; i <  n->stmts->size() ; i++) {
        n->stmts->at(i)->accept(this);
    }
}

void PrintVisitor::Visit(Var *n)
{
    cout << "Var: " << n->id->name;
    n->id->accept(this);
}

void PrintVisitor::Visit(Exp *e)
{
    cout << "Exp" << endl;
    e->accept(this);
}

void PrintVisitor::Visit(Assign *n)
{
    cout << "Assign" << endl;
    n->var->accept(this);
    n->exp->accept(this);
}

void PrintVisitor::Visit(ProcStmt *n)
{
    cout << "ProcStmt: " << n->id->name << endl;
    if (n->expls) n->expls->accept(this);
}

void PrintVisitor::Visit(ExpList *n)
{
    cout << "ExpList, Size: "<< n->expList->size() << endl;
    for (int i = 0; i < n->expList->size(); i++) {
        n->expList->at(i)->accept(this);
    }
}

void PrintVisitor::Visit(IfThen *n)
{
    cout << "IfThen" << endl;
    n->expr->accept(this);
    n->stmt->accept(this);
}

void PrintVisitor::Visit(IfThenElse *n)
{
    cout << "IfThenElse" << endl;
    n->expr->accept(this);
    n->trueStmt->accept(this);
    n->falseStmt->accept(this);
}

void PrintVisitor::Visit(While *n)
{
    cout << "While" << endl;
    n->expr->accept(this);
    n->stmt->accept(this);
}


void PrintVisitor::Visit(Type *t)
{
    cout << "Type" << endl;
    t->accept(this);
}

void PrintVisitor::Visit(StdType *t)
{
    cout << "StdType: " << t->type << endl;
}

void PrintVisitor::Visit(IdExp *e)
{
    cout << "IdExpr: " << e->id->name << endl;
}

void PrintVisitor::Visit(Integer *n)
{
    cout << "Integer: " << n->val << endl;
}

void PrintVisitor::Visit(Real *n)
{
    cout << "Real: " << n->val << endl;
}

void PrintVisitor::Visit(Bool *n)
{
    cout << "Bool: " << (n->val ? "true" : "false") << endl;
}
void PrintVisitor::Visit(Array *a)
{
    cout << "Array, Beg: "<<a->beginIndex << " End: "<< a->endIndex << endl;
    a->stdType->accept(this);
}

void PrintVisitor::Visit(ArrayElement  *a)
{
    cout << "Array Element of Array: "<<a->id->name << endl;
    a->index->accept(this);
}

void PrintVisitor::Visit(BinOp *b)
{
    cout << "BinOp" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Add *b)
{
    cout << "Add" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Sub *b)
{
    cout << "Sub" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}


void PrintVisitor::Visit(Mult *b)
{
    cout << "Mult" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Divide *b)
{
    cout << "Divide" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Mod *b)
{
    cout << "Mod" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(GT *b)
{
    cout << "GT" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(LT *b)
{
    cout << "LT" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(GE *b)
{
    cout << "GE" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(LE *b)
{
    cout << "LE" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}


void PrintVisitor::Visit(ET *b)
{
    cout << "ET" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(NE *b)
{
    cout << "NE" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(And *b)
{
    cout << "And" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Or *b)
{
    cout << "Or" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void PrintVisitor::Visit(Not *n)
{
    cout << "Not" << endl;
    n->exp->accept(this);
}
