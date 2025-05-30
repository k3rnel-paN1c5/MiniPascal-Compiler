#include "ast.h"
#include <iostream>
using namespace std;

extern SymbolTable* symbolTable;


TypeVisitor::TypeVisitor()
{

}

void TypeVisitor::Visit(Node *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Prog *n)
{
    cout << "Started Type Checking\n";
    if (n->declarations) n->declarations->accept(this);
    if (n->subDeclarations) n->subDeclarations->accept(this);
    if (n->compoundStatment) n->compoundStatment->accept(this);
}

void TypeVisitor::Visit(Stmt *s)
{
    s->accept(this);
}

void TypeVisitor::Visit(Ident *n)
{
    n->accept(this);
}


void TypeVisitor::Visit(Decs *n)
{

    for (int i = 0; i < n->decs->size(); i++) {
        n->decs->at(i)->accept(this);
    }
}


void TypeVisitor::Visit(ParDec *n)
{
    n->identList->accept(this);
    if (n->tp) n->tp->accept(this);
}

void TypeVisitor::Visit(IdentList *n)
{
    for (int i = 0; i < n->identLst->size(); i++) {
        n->identLst->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(SubDecs *n)
{
    for (int i = 0; i < n->subdecs->size(); i++) {
        n->subdecs->at(i)->accept(this);
    }
}
void TypeVisitor::Visit(SubDec *n)
{

    n->subHead->accept(this);
    if(n->compStmt)  n->compStmt->accept(this);


}

void TypeVisitor::Visit(SubHead *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Func *n)
{

    if (n->args) n->args->accept(this);
    if (n->typ) n->typ->accept(this);

}

void TypeVisitor::Visit(Args *n)
{

    n->parList->accept(this);

    
}

void TypeVisitor::Visit(ParList *n)
{



    for (int i = 0;i< n->parList->size(); i++) {
        n->parList->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(Proc *n)
{

    n->args->accept(this);

}
void TypeVisitor::Visit(FuncCall* a)
{


    a->exps->accept(this);

}
void TypeVisitor::Visit(CompStmt *n)
{
    if(n->optitonalStmts) 
        n->optitonalStmts->accept(this);
}

void TypeVisitor::Visit(OptionalStmts *n)
{
    if (n->stmtList) n->stmtList->accept(this);
}

void TypeVisitor::Visit(StmtList *n)
{

    for (int i = 0; i <  n->stmts->size() ; i++) {
        n->stmts->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(Var *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Exp *e)
{

    e->accept(this);
}

void TypeVisitor::Visit(Assign *n)
{
    n->var->accept(this);
    n->exp->accept(this);
    if(n->var->type == INTTYPE)
        cout << "INT";

}

void TypeVisitor::Visit(ProcStmt *n)
{
    if (n->expls) n->expls->accept(this);
}

void TypeVisitor::Visit(ExpList *n)
{
    for (int i = 0; i < n->expList->size(); i++) {
        n->expList->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(IfThen *n)
{
    n->expr->accept(this);
    n->stmt->accept(this);

}

void TypeVisitor::Visit(IfThenElse *n)
{
    n->expr->accept(this);
    n->trueStmt->accept(this);
    n->falseStmt->accept(this);

}

void TypeVisitor::Visit(While *n)
{

    n->expr->accept(this);
    n->stmt->accept(this);

}


void TypeVisitor::Visit(Type *t)
{
    t->accept(this);
}

void TypeVisitor::Visit(StdType *t)
{
    t->accept(this);
}

void TypeVisitor::Visit(IdExp *e)
{
    e->type = e->id->symbol->DataType;
}

void TypeVisitor::Visit(Integer *n)
{
    n->type = INTTYPE;
}

void TypeVisitor::Visit(Real *n)
{
    n->type = REALTYPE;
}

void TypeVisitor::Visit(Bool *n)
{
    n->type = BOOLTYPE;
}
void TypeVisitor::Visit(Array *a)
{
    a->stdType->accept(this);
}

void TypeVisitor::Visit(ArrayElement  *a)
{
    a->type = a->id->symbol->DataType;
    a->index->accept(this);
}

void TypeVisitor::Visit(BinOp *b)
{


    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Add *b)
{

    cout << "Add: " << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    if(b->leftExp->type == INTTYPE && b->rightExp->type == INTTYPE)
        cout  <<"Match while add\n";

}

void TypeVisitor::Visit(Sub *b)
{

    cout << "Sub: " << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}


void TypeVisitor::Visit(Mult *b)
{

    cout << "Mult:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Divide *b)
{

    cout << "Divide:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Mod *b)
{

    cout << "Mod:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(GT *b)
{

    cout << "GT:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(LT *b)
{

    cout << "LT:" << endl;
    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(GE *b)
{

    cout << "GE:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(LE *b)
{

    cout << "LE:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}


void TypeVisitor::Visit(ET *b)
{

    cout << "ET:" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(NE *b)
{

    cout << "NE" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(And *b)
{

    cout << "And" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Or *b)
{

    cout << "Or" << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Not *n)
{

    cout << "Not: " << endl;

    n->exp->accept(this);

}
