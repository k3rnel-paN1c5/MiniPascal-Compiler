#include "CommonTypes.h"
#include "ast.h"
#include "Error.h"
#include <iostream>

using namespace std;

extern SymbolTable *symbolTable;
extern Errors *errorStack;

TypeVisitor::TypeVisitor()
{
    this->currentFunction = nullptr;
    this->currentFunctionHasReturn = false;
}

void TypeVisitor::Visit(Node *n)
{
    if (n)
        n->accept(this);
}

void TypeVisitor::Visit(Prog *n)
{
    if (n->declarations) // variable  declaration
        n->declarations->accept(this);

    if (n->subDeclarations) // functions/procedures
        n->subDeclarations->accept(this);

    if (n->compoundStatment) // program body
        n->compoundStatment->accept(this);
}

void TypeVisitor::Visit(Decs *n)
{
    for (ParDec *pd : *(n->decs))
    {
        for (Ident *id : *(pd->identList->identLst))
        {
            symbolTable->AddSymbol(id, GLOBAL_VAR, pd->tp);
        }
    }
}

void TypeVisitor::Visit(SubDecs *n)
{
    for (SubDec *sd : *(n->subdecs))
    {
        sd->accept(this);
    }
}

void TypeVisitor::Visit(SubDec *n)
{
    Func *funcNode = dynamic_cast<Func *>(n->subHead);
    Proc *procNode = dynamic_cast<Proc *>(n->subHead);
    Ident *subId = nullptr;
    FunctionSignature *sig = nullptr;
    SymbolKind kind;
    vector<Type *> *astParamTypes = new vector<Type *>();
    if (funcNode)
    {
        subId = funcNode->id;
        kind = FUNC;
        if (funcNode->args && funcNode->args->parList)
        {
            for (ParDec *pd : *(funcNode->args->parList->parList))
            {
                for (size_t i = 0; i < pd->identList->identLst->size(); ++i)
                {
                    astParamTypes->push_back(pd->tp);
                }
            }
        }
        // funcNode->typ is StdType*, its ->type is the TypeEnum return type
        sig = new FunctionSignature(subId->name, astParamTypes, funcNode->typ->type);
    }
    else if (procNode)
    {
        subId = procNode->id;
        kind = PROC;
        
        if (procNode->args)
        {
            if (procNode->args->parList && procNode->args->parList->parList)
            {
                for (ParDec *pd : *(procNode->args->parList->parList))
                {
                    for (size_t i = 0; i < pd->identList->identLst->size(); ++i)
                    {
                        astParamTypes->push_back(pd->tp);
                    }
                }
            }
        }
        sig = new FunctionSignature(subId->name, astParamTypes, VOID); // Procedures return VOID
    }
    else
    {
        // Should not happen
        delete astParamTypes;
        return;
    }

    symbolTable->AddSymbol(subId, kind, sig);

    // New Scope for local variables and function/pocedure body
    symbolTable->NewScope();

    Func *previousFunctionContext = this->currentFunction;
    bool previousFunctionHadReturn = this->currentFunctionHasReturn;

    if (funcNode)
    {
        this->currentFunction = funcNode; // Set context for return checking
        this->currentFunctionHasReturn = false;
    }
    else
    {
        this->currentFunction = nullptr;
    }

    // Add parameters to the new (current) scope
    if (funcNode && funcNode->args && funcNode->args->parList)
    {
        funcNode->args->parList->accept(this);
    }
    else if (procNode && procNode->args && procNode->args->parList)
    {
        procNode->args->parList->accept(this);
    }

    // Add local variables to the new (current) scope
    if (n->localDecs)
    {
        n->localDecs->accept(this);
    }

    // Visit the body
    if (n->compStmt)
    {
        n->compStmt->accept(this);
    }

    // Check for missing return in functions
    if (funcNode && !this->currentFunctionHasReturn)
    {
        errorStack->AddError("Function '" + funcNode->id->name +
                                 "' is missing a return assignment (e.g., " +
                                 funcNode->id->name + " := expression).",
                             funcNode->id->line + 1, funcNode->id->column);
    }

    symbolTable->CloseScope();

    this->currentFunction = previousFunctionContext;
    this->currentFunctionHasReturn = previousFunctionHadReturn;
}

void TypeVisitor::Visit(ParList *n)
{
    for (ParDec *pd : *(n->parList))
    {
        for (Ident *id : *(pd->identList->identLst))
        {
            symbolTable->AddSymbol(id, PARAM_VAR, pd->tp);
        }
    }
}

void TypeVisitor::Visit(LocalDecs *n)
{
    if (!n || !n->localDecs)
        return;
    for (LocalDec *ld : *(n->localDecs))
    {
        ld->accept(this);
    }
}
void TypeVisitor::Visit(LocalDec *n)
{
    if (!n || !n->identlist || !n->tp)
        return;
    for (Ident *id : *(n->identlist->identLst))
    {
        symbolTable->AddSymbol(id, LOCAL_VAR, n->tp);
    }
}
void TypeVisitor::Visit(ParDec *n) {}

void TypeVisitor::Visit(Stmt *s)
{
    s->accept(this);
}

void TypeVisitor::Visit(Ident *n) {}

void TypeVisitor::Visit(IdentList *n) {}

void TypeVisitor::Visit(SubHead *n) {}

void TypeVisitor::Visit(Func *n) {}

void TypeVisitor::Visit(Args *n) {}

void TypeVisitor::Visit(Proc *n) {}

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

    if (this->currentFunction && n->id->name == this->currentFunction->id->name)
    {
        // Function Return
        n->type = this->currentFunction->typ->type;

        if (!n->id->symbol)
        {
            n->id->symbol = this->currentFunction->id->symbol;
        }
    }
    else
    {
        // Regular variable
        Symbol *sym = symbolTable->LookUpSymbol(n->id);
        if (sym)
        {
            n->type = sym->DataType;
        }
        else
        {
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

    if (n->var->type == VOID || n->exp->type == VOID)
    {
        return;
    }

    if (this->currentFunction != nullptr && n->var->id->name == this->currentFunction->id->name)
    {
        // This is a return statement
        TypeEnum expectedReturnType = this->currentFunction->typ->type;

        if (n->exp->type == expectedReturnType)
        {

            this->currentFunctionHasReturn = true;
        }
        else if (expectedReturnType == REALTYPE && n->exp->type == INTTYPE)
        {
            errorStack->AddWarning("Implicitly casting integer expression to real for return value of function '" + this->currentFunction->id->name + "'.", n->line + 1, n->column);
            cout << "Warning:" << n->line + 1 << ":" << n->column << " Implicitly casting integer expression to real for return value of function '" << this->currentFunction->id->name << "'." << endl;
            this->currentFunctionHasReturn = true;
        }
        else
        {
            errorStack->AddError("Type mismatch in return assignment for function '" + this->currentFunction->id->name +
                                     "'. Expected " + TypeEnumToString(expectedReturnType) +
                                     " but got " + TypeEnumToString(n->exp->type) + ".",
                                 n->line + 1, n->column);
        }
    }
    else
    {

        if (n->var->type != n->exp->type)
        {

            if (n->var->type == REALTYPE && n->exp->type == INTTYPE)
            {
                errorStack->AddWarning("Implicitly casting integer to real in assignment to '" + n->var->id->name + "'.", n->line + 1, n->column);
                cout << "Warning:" << n->line + 1 << ":" << n->column << " Implicitly casting integer to real in assignment to '" << n->var->id->name << "'." << endl;
            }
            else
            {
                errorStack->AddError("Cannot assign expression of type " + TypeEnumToString(n->exp->type) +
                                         " to variable '" + n->var->id->name + "' of type " + TypeEnumToString(n->var->type) + ".",
                                     n->line + 1, n->column);
            }
        }
    }
}

void TypeVisitor::Visit(FuncCall *a)
{

    vector<TypeEnum> *argTypes = new vector<TypeEnum>();
    if (a->exps)
    {
        a->exps->accept(this);
        for (Exp *argExp : *(a->exps->expList))
        {
            argTypes->push_back(argExp->type);
        }
    }

    Symbol *funcSym = symbolTable->LookUpSymbol(a->id, FUNC, argTypes);

    delete argTypes;

    if (funcSym)
    {
        a->type = funcSym->DataType; // DataType of FUNC symbol is its return type
        if (a->id->symbol == nullptr)
            a->id->symbol = funcSym;
    }
    else
    {
        // todo: Suggest Similar Functions

        a->type = VOID;
    }
}

void TypeVisitor::Visit(ProcStmt *n)
{
    vector<TypeEnum> *argTypes = new vector<TypeEnum>();
    if (n->expls)
    {
        n->expls->accept(this);
        for (Exp *argExp : *(n->expls->expList))
        {
            argTypes->push_back(argExp->type);
        }
    }

    Symbol *procSym = symbolTable->LookUpSymbol(n->id, PROC, argTypes);
    delete argTypes;
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
    if (n->expr->type != BOOLTYPE && n->expr->type != VOID)
    {
        errorStack->AddError("IF condition must be a boolean expression, but got " +
                                 TypeEnumToString(n->expr->type) + ".",
                             n->expr->line + 1, n->expr->column);
    }
    n->stmt->accept(this);
}

void TypeVisitor::Visit(IfThenElse *n)
{
    n->expr->accept(this);
    if (n->expr->type != BOOLTYPE && n->expr->type != VOID)
    {
        errorStack->AddError("IF-ELSE condition must be a boolean expression, but got " +
                                 TypeEnumToString(n->expr->type) + ".",
                             n->expr->line + 1, n->expr->column);
    }
    n->trueStmt->accept(this);
    n->falseStmt->accept(this);
}

void TypeVisitor::Visit(While *n)
{
    n->expr->accept(this);
    if (n->expr->type != BOOLTYPE && n->expr->type != VOID)
    {
        errorStack->AddError("WHILE condition must be a boolean expression, but got " +
                                 TypeEnumToString(n->expr->type) + ".",
                             n->expr->line + 1, n->expr->column);
    }
    n->stmt->accept(this);
}

void TypeVisitor::Visit(Type *t) {}

void TypeVisitor::Visit(StdType *t) {}

void TypeVisitor::Visit(IdExp *e)
{
    Symbol *sym = symbolTable->LookUpSymbol(e->id);
    if (sym)
    {

        e->type = sym->DataType;
        if (e->id->symbol == nullptr)
            e->id->symbol = sym;
    }
    else
    {
        e->type = VOID;
    }
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
    Symbol *arraySym = symbolTable->LookUpSymbol(a->id);

    if (!arraySym)
    {
        a->type = VOID;
        // Visit index to catch potential errors there too, though main issue is undeclared array
        if (a->index)
            a->index->accept(this);
        return;
    }
    if (a->id->symbol == nullptr)
        a->id->symbol = arraySym;

    switch (arraySym->DataType)
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
        errorStack->AddError("Identifier '" + a->id->name + "' is not an array.", a->line + 1, a->column);
        a->type = VOID;

        // Still visit the index to find errors there if any
        if (a->index)
            a->index->accept(this);
        return;
    }
    if (a->index)
    {
        a->index->accept(this);
        if (a->index->type != INTTYPE)
        {
            errorStack->AddError("Array index for '" + a->id->name + "' must be an integer expression, but got " + TypeEnumToString(a->index->type) + ".",
                                 a->index->line + 1, a->index->column);
        }
    }
    else
    {
        // Should not happen, an array element access needs an index.
        errorStack->AddError("Missing index for array '" + a->id->name + "'.", a->line + 1, a->column);
    }
}
void TypeVisitor::Visit(UnaryMinus *n)
{
    n->exp->accept(this);
    
    if (n->exp->type == INTTYPE)
    {
        n->type = INTTYPE;
    }
    else if (n->exp->type == REALTYPE)
    {
        n->type = REALTYPE;
    }
    else if (n->exp->type != VOID) 
    {
        errorStack->AddError("Unary minus operator requires an integer or real expression, but got " +
                                 TypeEnumToString(n->exp->type) + ".",
                             n->line + 1, n->column);
        n->type = VOID;
    }
    else
    {
        n->type = VOID;
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

    TypeEnum lType = b->leftExp->type;
    TypeEnum rType = b->rightExp->type;

    if (lType == VOID || rType == VOID)
    { // Error in operands already reported
        b->type = VOID;
        return;
    }

    if (lType == INTTYPE && rType == INTTYPE)
    {
        b->type = INTTYPE;
    }
    else if ((lType == REALTYPE && rType == REALTYPE) ||
             (lType == REALTYPE && rType == INTTYPE) ||
             (lType == INTTYPE && rType == REALTYPE))
    {
        b->type = REALTYPE;
        if (lType == INTTYPE || rType == INTTYPE)
        { // Only one is int
            errorStack->AddWarning("Implicitly casting integer to real in addition", b->line + 1, b->column);
        }
    }
    else
    {
        errorStack->AddError("Type mismatch in addition: Cannot add " + TypeEnumToString(lType) +
                                 " and " + TypeEnumToString(rType) + ".",
                             b->line + 1, b->column);
        b->type = VOID;
    }
}

void TypeVisitor::Visit(Sub *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    TypeEnum lType = b->leftExp->type;
    TypeEnum rType = b->rightExp->type;

    if (lType == VOID || rType == VOID)
    { // Error in operands already reported
        b->type = VOID;
        return;
    }

    if (lType == INTTYPE && rType == INTTYPE)
    {
        b->type = INTTYPE;
    }
    else if ((lType == REALTYPE && rType == REALTYPE) ||
             (lType == REALTYPE && rType == INTTYPE) ||
             (lType == INTTYPE && rType == REALTYPE))
    {
        b->type = REALTYPE;
        if (lType == INTTYPE || rType == INTTYPE)
        { // Only one is int
            errorStack->AddWarning("Implicitly casting integer to real in subtraction", b->line + 1, b->column);
        }
    }
    else
    {
        errorStack->AddError("Type mismatch in addition: Cannot subtact " + TypeEnumToString(rType) +
                                 " from " + TypeEnumToString(lType) + ".",
                             b->line + 1, b->column);
        b->type = VOID;
    }
}

void TypeVisitor::Visit(Mult *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    TypeEnum lType = b->leftExp->type;
    TypeEnum rType = b->rightExp->type;

    if (lType == VOID || rType == VOID)
    { // Error in operands already reported
        b->type = VOID;
        return;
    }

    if (lType == INTTYPE && rType == INTTYPE)
    {
        b->type = INTTYPE;
    }
    else if ((lType == REALTYPE && rType == REALTYPE) ||
             (lType == REALTYPE && rType == INTTYPE) ||
             (lType == INTTYPE && rType == REALTYPE))
    {
        b->type = REALTYPE;
        if (lType == INTTYPE || rType == INTTYPE)
        { // Only one is int
            errorStack->AddWarning("Implicitly casting integer to real in multiplication", b->line + 1, b->column);
        }
    }
    else
    {
        errorStack->AddError("Type mismatch in addition: Cannot multiply " + TypeEnumToString(lType) +
                                 " and " + TypeEnumToString(rType) + ".",
                             b->line + 1, b->column);
        b->type = VOID;
    }
}

void TypeVisitor::Visit(Divide *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    TypeEnum lType = b->leftExp->type;
    TypeEnum rType = b->rightExp->type;

    if (lType == VOID || rType == VOID)
    { // Error in operands already reported
        b->type = VOID;
        return;
    }

    if ((lType == INTTYPE && rType == INTTYPE) ||
        (lType == REALTYPE && rType == REALTYPE) ||
        (lType == REALTYPE && rType == INTTYPE) ||
        (lType == INTTYPE && rType == REALTYPE))
    {
        b->type = REALTYPE;
        if (lType == INTTYPE || rType == INTTYPE)
        { //  one is int
            errorStack->AddWarning("Implicitly casting integer to real in divison", b->line + 1, b->column);
        }
    }
    else
    {
        errorStack->AddError("Type mismatch in addition: Cannot divide " + TypeEnumToString(lType) +
                                 " on " + TypeEnumToString(rType) + ".",
                             b->line + 1, b->column);
        b->type = VOID;
    }
}

void TypeVisitor::Visit(IntDiv *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);

    TypeEnum lType = b->leftExp->type;
    TypeEnum rType = b->rightExp->type;

    if (lType == VOID || rType == VOID)
    { // Error in operands already reported
        b->type = VOID;
        return;
    }

    if (lType == INTTYPE && rType == INTTYPE)
    {
        b->type = INTTYPE;
    }
    else
    {
        errorStack->AddError("Can't calculate  Integer Divison for types : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (greater than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (Less than) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (greater than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (less than or equal) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (equals) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
            errorStack->AddWarning("Implicitly casting integer to real", b->line, b->column);
        }
        else
        {
            errorStack->AddError("Can't evaluate (not equal tp) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
        errorStack->AddError("Can't evaluate (Logical And) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
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
        errorStack->AddError("Can't evaluate (Logical Or) operator for expressions of type : " + TypeEnumToString(b->rightExp->type) + " and: " + TypeEnumToString(b->leftExp->type), b->line + 1, b->column);
        return;
    }
}

void TypeVisitor::Visit(Not *n)
{
    n->exp->accept(this);
    if (n->exp->type == BOOLTYPE)
    {
        n->type = BOOLTYPE;
    }
    else if (n->exp->type != VOID)
    {
        errorStack->AddError("Logical NOT operator requires a boolean expression, but got " +
                                 TypeEnumToString(n->exp->type) + ".",
                             n->line + 1, n->column);
        n->type = VOID;
    }
    else
    {
        n->type = VOID;
    }
}
