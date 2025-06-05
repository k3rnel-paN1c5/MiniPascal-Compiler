#include "CommonTypes.h"
#include "ast.h"
#include "Error.h"
#include <iostream>

using namespace std;

extern SymbolTable *symbolTable;
extern Errors *errorStack;

TypeVisitor::TypeVisitor()
{
}

void TypeVisitor::Visit(Node *n)
{
    n->accept(this);
}

void TypeVisitor::Visit(Prog *n)
{

    if (n->declarations)
        n->declarations->accept(this);

    if (n->subDeclarations)
        n->subDeclarations->accept(this);

    if (n->compoundStatment)
        n->compoundStatment->accept(this);
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
    for (int i = 0; i < n->identLst->size(); i++)
    {
        n->identLst->at(i)->accept(this);
    }
}

void TypeVisitor::Visit(SubDecs *n)
{
    for (int i = 0; i < n->subdecs->size(); i++)
    {
        n->subdecs->at(i)->accept(this);
    }
}

void TypeVisitor::Visit(SubDec *n)
{
    // if(n->compStmt)  n->compStmt->accept(this);
    if (auto *funcNode = dynamic_cast<Func *>(n->subHead))
    {
        Func *previousFunction = this->currentFunction;
        bool previousHadReturn = this->currentFunctionHasReturn;

        this->currentFunction = funcNode;
        this->currentFunctionHasReturn = false;

        if (n->localDecs)
            n->localDecs->accept(this);
        if (n->compStmt)
        {
            n->compStmt->accept(this);
        }

        if (!this->currentFunctionHasReturn)
        {
            errorStack->AddError("Function '" + funcNode->id->name +
                                     "' is missing a return assignment (e.g., " +
                                     funcNode->id->name + " := expression).",
                                 funcNode->id->line + 1, funcNode->id->column); // Use Ident's line/col
        }

        this->currentFunction = previousFunction; // Restore previous context
        this->currentFunctionHasReturn = previousHadReturn;
    }
    else
    {

        Func *previousFunction = this->currentFunction;
        bool previousHadReturn = this->currentFunctionHasReturn;
        this->currentFunction = nullptr;

        if (n->localDecs)
            n->localDecs->accept(this);
        if (n->compStmt)
        {
            n->compStmt->accept(this);
        }

        this->currentFunction = previousFunction;
        this->currentFunctionHasReturn = previousHadReturn;
    }
}

void TypeVisitor::Visit(SubHead *n) {}

void TypeVisitor::Visit(Func *n) {}

void TypeVisitor::Visit(Args *n) {}

void TypeVisitor::Visit(ParList *n) {}

void TypeVisitor::Visit(Proc *n) {}

void TypeVisitor::Visit(FuncCall *a)
{
    a->exps->accept(this);
}

void TypeVisitor::Visit(CompStmt *n)
{
    if (n->optitonalStmts)
        n->optitonalStmts->accept(this);
}

void TypeVisitor::Visit(OptionalStmts *n)
{
    if (n->stmtList)
        n->stmtList->accept(this);
}

void TypeVisitor::Visit(StmtList *n)
{
    for (int i = 0; i < n->stmts->size(); i++)
    {
        n->stmts->at(i)->accept(this);
    }
}

void TypeVisitor::Visit(Var *n)
{

    // n->id->accept(this);
    // n->type = n->id->symbol->DataType;

    if (this->currentFunction && n->id->name == this->currentFunction->id->name)
    {
        // Its type is the function's return type.
        n->type = this->currentFunction->typ->type; // typ is StdType*, so typ->type is TypeEnum
        // We assume n->id->symbol is already the function's symbol (Kind=FUNC).
        // If not, it's an earlier stage issue (parser or symbol table linking).
    }
    else // It's a regular variable. Look it up in the symbol table.
    {
        Symbol *sym = symbolTable->LookUpSymbol(n->id); 
        if (sym)
        {
            n->type = sym->DataType;
        }
        else
        {
            // LookUpSymbol(Ident*) adds "Undeclared Variable" error if not found.
            n->type = VOID; // Mark as VOID type if undeclared.
        }
    }
}

void TypeVisitor::Visit(Exp *e)
{

    e->accept(this);
}

void TypeVisitor::Visit(Assign *n)
{
    n->var->accept(this);
    n->exp->accept(this);
    
    if (this->currentFunction != nullptr && n->var->id->name == this->currentFunction->id->name){
        // This is a return statement 
        TypeEnum expectedReturnType = this->currentFunction->typ->type;

         if (n->exp->type == expectedReturnType) {

            this->currentFunctionHasReturn = true; 

        } else if (expectedReturnType == REALTYPE && n->exp->type == INTTYPE) {
            errorStack->AddWarning("Implicitly casting integer expression to real for return value of function '" + this->currentFunction->id->name + "'." , n->line+1, n->column);
            cout << "Warning:" << n->line + 1 << ":" << n->column << " Implicitly casting integer expression to real for return value of function '" << this->currentFunction->id->name << "'." << endl;
            this->currentFunctionHasReturn = true;
        } else {
            errorStack->AddError("Type mismatch in return assignment for function '" + this->currentFunction->id->name +
                                 "'. Expected " + TypeEnumToString(expectedReturnType) +
                                 " but got " + TypeEnumToString(n->exp->type) + ".",
                                 n->line + 1, n->column);
        }
    }
    else {
        if (n->var->type != VOID){
            if (n->var->type != n->exp->type) {

                if (n->var->type == REALTYPE && n->exp->type == INTTYPE) {
                    errorStack->AddWarning("Implicitly casting integer to real in assignment to '" + n->var->id->name  +  "'." , n->line+1, n->column);
                    cout << "Warning:" << n->line + 1 << ":" << n->column << " Implicitly casting integer to real in assignment to '" << n->var->id->name << "'." << endl;
                } else {
                    errorStack->AddError("Cannot assign expression of type " + TypeEnumToString(n->exp->type) +
                                         " to variable '" + n->var->id->name + "' of type " + TypeEnumToString(n->var->type) + ".",
                                         n->line + 1, n->column);
                }
            }
        }
    }
    if (n->var->type != n->exp->type)
    {
        if (n->var->type == REALTYPE && n->exp->type == INTTYPE)
        {
            cout << "Warning:" << n->line << ":" << n->column << " Implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't Assign expression of type : " + TypeEnumToString(n->exp->type) + " to variable of type: " + TypeEnumToString(n->var->type), n->line, n->column);
            return;
        }
    }
}

void TypeVisitor::Visit(ProcStmt *n)
{
    if (n->expls)
        n->expls->accept(this);
}

void TypeVisitor::Visit(ExpList *n)
{
    for (int i = 0; i < n->expList->size(); i++)
    {
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

void TypeVisitor::Visit(ArrayElement *a)
{
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
        a->type = VOID;
        break;
    }
    a->index->accept(this);
    if (a->index->type != INTTYPE)
    {
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
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else
        {
            errorStack->AddError("Can't Add expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Sub *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else
        {
            errorStack->AddError("Can't subtract expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Mult *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else
        {
            errorStack->AddError("Can't Multiply expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Divide *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
            b->type = REALTYPE;
        }
        else
        {
            errorStack->AddError("Can't Divide expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(Mod *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = b->leftExp->type;
    if (b->leftExp->type != INTTYPE || b->rightExp->type != INTTYPE)
    {
        errorStack->AddError("Can't calculate  Modulo expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    }
}

void TypeVisitor::Visit(GT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (greater than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(LT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;
    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (Less than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(GE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (greater than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(LE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (less than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(ET *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    b->type = BOOLTYPE;

    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (equals) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(NE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = BOOLTYPE;

    if (b->leftExp->type != b->rightExp->type || b->leftExp->type == BOOLTYPE)
    {
        if ((b->leftExp->type == REALTYPE && b->rightExp->type == INTTYPE) || (b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE))
        {
            cout << "Warning:" << b->line << ":" << b->column << " implicitly casting integer to real\n";
        }
        else
        {
            errorStack->AddError("Can't evaluate (not equal tp) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
            return;
        }
    }
}

void TypeVisitor::Visit(And *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = BOOLTYPE;

    if (b->leftExp->type != BOOLTYPE || b->rightExp->type != BOOLTYPE)
    {
        errorStack->AddError("Can't evaluate (Logical And) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    }
}

void TypeVisitor::Visit(Or *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    b->type = BOOLTYPE;

    if (b->leftExp->type != BOOLTYPE || b->rightExp->type != BOOLTYPE)
    {
        errorStack->AddError("Can't evaluate (Logical Or) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line, b->column);
        return;
    }
}

void TypeVisitor::Visit(Not *n)
{
    n->exp->accept(this);

    n->type = BOOLTYPE;
    if (n->exp->type != BOOLTYPE)
    {
        errorStack->AddError("Can't evaluate (Logical NOT) operator for expressions of type : " + TypeEnumToString(n->exp->type), n->line, n->column);
        return;
    }
}
