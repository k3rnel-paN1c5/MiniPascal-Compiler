#include "CommonTypes.h"
#include "ast.h"
#include "Error.h"
#include <iostream>

using namespace std;

extern SymbolTable* symbolTable;
extern Errors* errorStack;


TypeVisitor::TypeVisitor()
{

}

void TypeVisitor::Visit(Node *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Prog *n)
{

    if (n->declarations) n->declarations->accept(this);

    if (n->subDeclarations) n->subDeclarations->accept(this);

    if (n->compoundStatment) n->compoundStatment->accept(this);
}

void TypeVisitor::Visit(Stmt *s)
{
    s->accept(this);
}

void TypeVisitor::Visit(Ident *n) {}

void TypeVisitor::Visit(Decs *n) {}

void TypeVisitor::Visit(ParDec *n) {}

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
    if(n->compStmt)  n->compStmt->accept(this);
}

void TypeVisitor::Visit(SubHead *n) {}

void TypeVisitor::Visit(Func *n) {}

void TypeVisitor::Visit(Args *n) {}

void TypeVisitor::Visit(ParList *n) {}

void TypeVisitor::Visit(Proc *n) {}

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

    n->id->accept(this);
    if(n->id->symbol)
        n->type = n->id->symbol->DataType;
}

void TypeVisitor::Visit(Exp *e)
{

    e->accept(this);
}

void TypeVisitor::Visit(Assign *n)
{
    n->var->accept(this);
    n->exp->accept(this);
    if(n->var->type != n->exp->type){
        if(n->var->type == REALTYPE && n->exp->type == INTTYPE){
            cout << "Warning:"<<n->line<<":"<<n->column << " Implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't Assign expression of type : " + TypeEnumToString(n->exp->type) + " to variable of type: " +   TypeEnumToString(n->var->type), n->line, n->column);
            return;
        }
    }

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


void TypeVisitor::Visit(Type *t) {}

void TypeVisitor::Visit(StdType *t) {}

void TypeVisitor::Visit(IdExp *e)
{
    if(e->id->symbol)
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
    if(!a->id->symbol)
        return;
    a->type = a->id->symbol->DataType;
    switch (a->id->symbol->DataType)
    {
    case INT_ARRAY:
        a->type = INTTYPE;
        break;
    case REAL_ARRAY:
        a->type = REALTYPE;
        break;
    case BOOL_ARRAY:
        a->type = BOOLTYPE;
        break;
        
    default:
        errorStack->AddError(a->id->name + " is not an array", a->line, a->column);
        return;
    }
    a->index->accept(this);
    if(a->index->type != INTTYPE){
        errorStack->AddError("array index must evaluate to integer type ", a->line, a->column);
    }
}

void TypeVisitor::Visit(BinOp *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
}

void TypeVisitor::Visit(Add *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = b->leftExp->type;
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else{
            errorStack->AddError("Can't Add expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Sub *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
     if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else{
            errorStack->AddError("Can't subtract expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Mult *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
                b->type = REALTYPE;
        }
        else{
            errorStack->AddError("Can't Multiply expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Divide *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else{
            errorStack->AddError("Can't Divide expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Mod *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if(b->leftExp->type != INTTYPE || b->rightExp->type != INTTYPE){
        errorStack->AddError("Can't calculate  Modulo expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    }
}

void TypeVisitor::Visit(GT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (greater than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(LT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (Less than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(GE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (greater than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(LE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (less than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}


void TypeVisitor::Visit(ET *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (equals) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(NE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = BOOLTYPE;
    
    if(b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE){
        if((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE)){
            cout << "Warning:"<<b->line<<":"<<b->column << " implicitly casting integer to real\n";
            
        }
        else{
            errorStack->AddError("Can't evaluate (not equal tp) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(And *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = BOOLTYPE;
    
    if(b->leftExp->type != BOOLTYPE || b->rightExp->type != BOOLTYPE ){
        errorStack->AddError("Can't evaluate (Logical And) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    
    }
}

void TypeVisitor::Visit(Or *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

     b->type = BOOLTYPE;
    
    if(b->leftExp->type != BOOLTYPE || b->rightExp->type != BOOLTYPE ){
        errorStack->AddError("Can't evaluate (Logical Or) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " +   TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    
    }
}

void TypeVisitor::Visit(Not *n)
{
    n->exp->accept(this);

    n->type = BOOLTYPE;
    if(n->exp->type !=  BOOLTYPE){
        errorStack->AddError("Can't evaluate (Logical NOT) operator for expressions of type : " + TypeEnumToString(n->exp->type), n->line, n->column);
        return;
    }
}
