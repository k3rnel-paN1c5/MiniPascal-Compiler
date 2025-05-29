#include "ast.h"
#include <iostream>
using namespace std;
#include "ast.h"
#include <iostream>
using namespace std;


TypeVisitor::TypeVisitor()
{

}

void TypeVisitor::Visit(Node *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Prog *n)
{

    cout << "Prog: '"<< n->name->name << "'" << endl;
    if (n->declarations) n->declarations->accept(this);
    if (n->subDeclarations) n->subDeclarations->accept(this);
    if (n->compoundStatment) n->compoundStatment->accept(this);
}

void TypeVisitor::Visit(Stmt *s)
{
    cout << "Stmt" << endl;
    s->accept(this);
}

void TypeVisitor::Visit(Ident *n)
{

    cout << "Identity: '"<<n->name <<"'"<<endl;
}


void TypeVisitor::Visit(Decs *n)
{


    cout << "Decs, Size: " << n->decs->size() <<endl;
    for (int i = 0; i < n->decs->size(); i++) {
        n->decs->at(i)->accept(this);
    }
}


void TypeVisitor::Visit(ParDec *n)
{
    cout << "ParDec" <<endl;
    n->identList->accept(this);
    if (n->tp) n->tp->accept(this);
}

void TypeVisitor::Visit(IdentList *n)
{

    cout << "IdentList, Size: " << n->identLst->size() <<endl;

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


    cout << "Sub Dec: "<<endl;

    n->subHead->accept(this);
    if(n->compStmt)  n->compStmt->accept(this);


}

void TypeVisitor::Visit(SubHead *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Func *n)
{

    cout << "Func: " << n->id->name << endl;

    if (n->args) n->args->accept(this);
    if (n->typ) n->typ->accept(this);

}

void TypeVisitor::Visit(Args *n)
{
    cout << "Args: " << endl;

    n->parList->accept(this);

    
}

void TypeVisitor::Visit(ParList *n)
{


    cout << "ParList, Size: " <<  n->parList->size() << endl;

    for (int i = 0;i< n->parList->size(); i++) {
        n->parList->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(Proc *n)
{


    cout << "Proc: " << n->id->name << endl;

    n->args->accept(this);

}
void TypeVisitor::Visit(FuncCall* a)
{

    cout << "Function Call: " << a->id->name <<endl;

    a->exps->accept(this);

}
void TypeVisitor::Visit(CompStmt *n)
{

    cout << "CompStmt" << endl;
    if(n->optitonalStmts) 
        n->optitonalStmts->accept(this);
}

void TypeVisitor::Visit(OptionalStmts *n)
{

    cout << "OptionalStmts" << endl;

    if (n->stmtList) n->stmtList->accept(this);

}

void TypeVisitor::Visit(StmtList *n)
{
    cout << "StmtList, Size: " << n->stmts->size() << endl;
    for (int i = 0; i <  n->stmts->size() ; i++) {
        n->stmts->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(Var *n)
{
    cout << "Var: " << n->id->name <<endl;
}

void TypeVisitor::Visit(Exp *e)
{

    cout << "Exp" << endl;
    e->accept(this);
}

void TypeVisitor::Visit(Assign *n)
{


    cout << "Assign: " << endl;

    n->var->accept(this);
    n->exp->accept(this);

}

void TypeVisitor::Visit(ProcStmt *n)
{
    cout << "ProcStmt: " << n->id->name << endl;
    if (n->expls) n->expls->accept(this);
}

void TypeVisitor::Visit(ExpList *n)
{


    cout << "ExpList, Size: "<< n->expList->size() << endl;

    for (int i = 0; i < n->expList->size(); i++) {
        n->expList->at(i)->accept(this);
    }

}

void TypeVisitor::Visit(IfThen *n)
{


    cout << "If Then: " << endl;

    n->expr->accept(this);
    n->stmt->accept(this);

}

void TypeVisitor::Visit(IfThenElse *n)
{


    cout << "If Then Else: " << endl;

    n->expr->accept(this);
    n->trueStmt->accept(this);
    n->falseStmt->accept(this);

}

void TypeVisitor::Visit(While *n)
{

    cout << "While: " << endl;

    n->expr->accept(this);
    n->stmt->accept(this);

}


void TypeVisitor::Visit(Type *t)
{
    cout << "Type" << endl;
    t->accept(this);
}

void TypeVisitor::Visit(StdType *t)
{

    cout << "StdType: " << TypeEnumToString(t->type) << endl;
}

void TypeVisitor::Visit(IdExp *e)
{

    cout << "IdExpr: " << e->id->name << endl;
}

void TypeVisitor::Visit(Integer *n)
{

    cout << "Integer: " << n->val << endl;
}

void TypeVisitor::Visit(Real *n)
{

    cout << "Real: " << n->val << endl;
}

void TypeVisitor::Visit(Bool *n)
{

    cout << "Bool: " << (n->val ? "true" : "false") << endl;
}
void TypeVisitor::Visit(Array *a)
{

    cout << "Array, Beg: "<<a->beginIndex << " End: "<< a->endIndex << endl;
    a->stdType->accept(this);
}

void TypeVisitor::Visit(ArrayElement  *a)
{

    cout << "Array Element of Array: "<<a->id->name << " at index: ";
    a->index->accept(this);
}

void TypeVisitor::Visit(BinOp *b)
{

    cout << "BinOp: " << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

}

void TypeVisitor::Visit(Add *b)
{

    cout << "Add: " << endl;

    b->leftExp->accept(this);
    b->rightExp->accept(this);

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
