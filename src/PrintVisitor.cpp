#include "CommonTypes.h"
#include "ast.h"
#include <iostream>

using namespace std;


PrintVisitor::PrintVisitor()
{
    this->level = 0;
}

void PrintVisitor::Visit(Node *n)
{
    n->accept(this);
}

void PrintVisitor::Visit(Prog *n)
{

    cout << "Prog: '"<< n->name->name << "'" << endl;
    this->level++;
    if (n->declarations) n->declarations->accept(this);
    if (n->subDeclarations) n->subDeclarations->accept(this);
    if (n->compoundStatment) n->compoundStatment->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Stmt *s)
{
    cout << "Stmt" << endl;
    s->accept(this);
}

void PrintVisitor::Visit(Ident *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Identifier: '"<<n->name <<"'"<<endl;
}

void PrintVisitor::Visit(Decs *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Decs, Size: " << n->decs->size() <<endl;
    this->level++;
    for (int i = 0; i < n->decs->size(); i++) {
        n->decs->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(ParDec *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "ParDec" <<endl;
    this->level++;
    n->identList->accept(this);
    if (n->tp) n->tp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(IdentList *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "IdentList, Size: " << n->identLst->size() <<endl;
    this->level++;
    for (int i = 0; i < n->identLst->size(); i++) {
        n->identLst->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(SubDecs *n)
{
    if(n->subdecs==nullptr || n->subdecs->size() == 0)
        return;
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "SubDecs, Size: " << n->subdecs->size() <<endl;
    this->level++;
    for (int i = 0; i < n->subdecs->size(); i++) {
        n->subdecs->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(SubDec *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Sub Dec: "<<endl;
    this->level++;
    n->subHead->accept(this);
    if(n->localDecs) n->localDecs->accept(this);
    if(n->compStmt)  n->compStmt->accept(this);
    this->level--;

}

void PrintVisitor::Visit(SubHead *n)
{
    n->accept(this);
}

void PrintVisitor::Visit(Func *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Func: " << n->id->name << endl;
    this->level++;
    if (n->args) n->args->accept(this);
    this->level--;
    if (n->typ) n->typ->accept(this);
}

void PrintVisitor::Visit(Args *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Args: " << endl;
    this->level++;
    n->parList->accept(this);
    this->level--;
    
}

void PrintVisitor::Visit(ParList *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "ParList, Size: " <<  n->parList->size() << endl;
    this->level++;
    for (int i = 0;i< n->parList->size(); i++) {
        n->parList->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(Proc *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Proc: " << n->id->name << endl;
    this->level++;
    if(n->args)
        n->args->accept(this);
    this->level--;
}
void PrintVisitor::Visit(LocalDecs *n)
{
    if(n->localDecs == nullptr || n->localDecs->size() == 0)
    return;
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout <<"Local Variable Declarations, Size" << n->localDecs->size()<<endl;
    this->level++;
    for(int i = 0; i < n->localDecs->size(); i++){
        n->localDecs->at(i)->accept(this);
    }
    this->level--;
 
}
void PrintVisitor::Visit(LocalDec *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Local Dec" <<endl;
    this->level++;
    if(n->identlist) n->identlist->accept(this);
    if (n->tp) n->tp->accept(this);
    this->level--;

}

void PrintVisitor::Visit(FuncCall* a)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Function Call: " << a->id->name <<endl;
    this->level++;
    if(a->exps)
        a->exps->accept(this);
    this->level--;
}

void PrintVisitor::Visit(CompStmt *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "CompStmt" << endl;
    this->level++;
    if(n->optitonalStmts) 
        n->optitonalStmts->accept(this);
    this->level--;
}

void PrintVisitor::Visit(OptionalStmts *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "OptionalStmts" << endl;
    this->level++;
    if (n->stmtList) n->stmtList->accept(this);
    this->level--;
}

void PrintVisitor::Visit(StmtList *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "StmtList, Size: " << n->stmts->size() << endl;
    this->level++;
    for (int i = 0; i <  n->stmts->size() ; i++) {
        n->stmts->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(Var *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Var: " << n->id->name <<endl;
}

void PrintVisitor::Visit(Exp *e)
{
    cout << "Exp" << endl;
    e->accept(this);
}

void PrintVisitor::Visit(Assign *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Assign: " << endl;
    this->level++;
    n->var->accept(this);
    n->exp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(ProcStmt *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "ProcStmt: " << n->id->name << endl;
    this->level++;
    if (n->expls) n->expls->accept(this);
    this->level--;
}

void PrintVisitor::Visit(ExpList *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "ExpList, Size: "<< n->expList->size() << endl;
    this->level++;
    for (int i = 0; i < n->expList->size(); i++) {
        n->expList->at(i)->accept(this);
    }
    this->level--;
}

void PrintVisitor::Visit(IfThen *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "If Then: " << endl;
    this->level++;
    n->expr->accept(this);
    n->stmt->accept(this);
    this->level--;
}

void PrintVisitor::Visit(IfThenElse *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "If Then Else: " << endl;
    this->level++;
    n->expr->accept(this);
    n->trueStmt->accept(this);
    n->falseStmt->accept(this);
    this->level--;
}

void PrintVisitor::Visit(While *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "While: " << endl;
    this->level++;
    n->expr->accept(this);
    n->stmt->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Type *t)
{
    cout << "Type" << endl;
    t->accept(this);
}

void PrintVisitor::Visit(StdType *t)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "StdType: " << TypeEnumToString(t->type) << endl;
}

void PrintVisitor::Visit(IdExp *e)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "IdExpr: " << e->id->name << endl;
}
void PrintVisitor::Visit(ArrayExp *e)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Araay '" << e->id->name  <<"' At Index: ";
    int x = this->level;
    this->level = 0;
    if(e->index)
        e->index->accept(this);
    this->level = x;
}

void PrintVisitor::Visit(Integer *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Integer: " << n->val << endl;
}

void PrintVisitor::Visit(Real *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Real: " << n->val << endl;
}

void PrintVisitor::Visit(Bool *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Bool: " << (n->val ? "true" : "false") << endl;
}

void PrintVisitor::Visit(Array *a)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Array, Beg: "<<a->beginIndex << " End: "<< a->endIndex << endl;
    a->stdType->accept(this);
}

void PrintVisitor::Visit(ArrayElement  *a)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Array Element of Array: "<<a->id->name << " at index: ";
    int x = this->level;
    this->level = 0;
    a->index->accept(this);
    this->level = x;
}
void PrintVisitor::Visit(UnaryMinus *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "UnaryMinus: " << endl;
    this->level++;
    n->exp->accept(this);
    this->level--;
}
void PrintVisitor::Visit(BinOp *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "BinOp: " << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Add *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Add: " << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Sub *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Sub: " << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Mult *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Mult:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Divide *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Divide:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(IntDiv *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Integer Divison:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(GT *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "GT:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(LT *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "LT:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(GE *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "GE:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(LE *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "LE:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(ET *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "ET:" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(NE *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "NE" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(And *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "And" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Or *b)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Or" << endl;
    this->level++;
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    this->level--;
}

void PrintVisitor::Visit(Not *n)
{
    for(int i = 0; i < this->level; i++)
        cout << "-- ";
    cout << "Not: " << endl;
    this->level++;
    n->exp->accept(this);
    this->level--;
}