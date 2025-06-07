#include "Visitor.h"
#include "ast.h"
#include "CommonTypes.h"
#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(const string &filename)
{
    labelCount = 0;
    currentFunctionContext = nullptr;
    outFile.open(filename);
    if (!outFile.is_open())
    {
        cerr << "Error: Could not open output file " << filename << endl;
    }
}

CodeGenVisitor::~CodeGenVisitor()
{
    if (outFile.is_open())
    {
        outFile.close();
    }
}

string CodeGenVisitor::newLabel()
{
    return "L" + to_string(labelCount++);
}

void CodeGenVisitor::emit(const string &instruction)
{
    if (outFile.is_open())
    {
        outFile << "    " << instruction << endl;
    }
}

void CodeGenVisitor::emitLabel(const string &label)
{
    if (outFile.is_open())
    {
        outFile << label << ":" << endl;
    }
}

// Visit Methods Implementation
void CodeGenVisitor::Visit(Node *n)
{
    if (n)
        n->accept(this);
}
void CodeGenVisitor::Visit(Stmt *s)
{
    if (s)
        s->accept(this);
}
void CodeGenVisitor::Visit(Exp *e)
{
    if (e)
        e->accept(this);
}
void CodeGenVisitor::Visit(Ident *n) { /* Do nothing */ }
void CodeGenVisitor::Visit(Decs *n) { /* Handled in Prog */ }
void CodeGenVisitor::Visit(ParDec *n) { /* Handled in SubDec */ }
void CodeGenVisitor::Visit(IdentList *n) { /* Do nothing */ }
void CodeGenVisitor::Visit(SubHead *n) { /* Do nothing */ }
void CodeGenVisitor::Visit(Args *n) { /* Do nothing */ }
void CodeGenVisitor::Visit(ParList *n) { /* Do nothing */ }
void CodeGenVisitor::Visit(LocalDecs *n) { /* Handled in SubDec */ }
void CodeGenVisitor::Visit(LocalDec *n) { /* Handled in SubDec */ }
void CodeGenVisitor::Visit(Type *t) { /* Do nothing */ }
void CodeGenVisitor::Visit(StdType *t) { /* Do nothing */ }
void CodeGenVisitor::Visit(Array *a) { /* Do nothing */ }
void CodeGenVisitor::Visit(BinOp *b) { /* Do nothing */ }
void CodeGenVisitor::Visit(Var *v) { /* Handled by children */ }

void CodeGenVisitor::Visit(Prog *n)
{
    emit("START\n");
    if (n->declarations)
    {
        int num_globals = 0;
        for (auto dec : *n->declarations->decs)
        {
            num_globals += dec->identList->identLst->size();
        }
        if (num_globals > 0)
        {
            // Allocate space for globals by pushing N zeros onto the stack
            emit("PUSHN " + to_string(num_globals));
        }
    }
    if (n->compoundStatment)
        n->compoundStatment->accept(this);
    emit("STOP");
    if (n->subDeclarations)
        n->subDeclarations->accept(this);
}

void CodeGenVisitor::Visit(SubDecs *n)
{
    for (auto *subdec : *n->subdecs)
    {
        subdec->accept(this);
    }
}

void CodeGenVisitor::Visit(SubDec *n)
{
    Func *func = dynamic_cast<Func *>(n->subHead);
    Proc *proc = dynamic_cast<Proc *>(n->subHead);
    FunctionSignature *name = func ? func->id->symbol->funcSig : proc->id->symbol->funcSig;
    string label = func ? 'f' + name->getSignatureString() : 'p' + name->getSignatureString();

    emitLabel(label);
    if (func)
    {
        currentFunctionContext = func;
    }

    // Allocate space for local variables
    if (n->localDecs)
    {
        int num_locals = 0;
        for (auto l_dec : *n->localDecs->localDecs)
        {
            num_locals += l_dec->identlist->identLst->size();
        }
        if (num_locals > 0)
        {
            emit("PUSHN " + to_string(num_locals));
        }
    }

    n->compStmt->accept(this);

    emit("RETURN");

    currentFunctionContext = nullptr; // Reset context
}

void CodeGenVisitor::Visit(Func *n) { /* Handled in SubDec */ }
void CodeGenVisitor::Visit(Proc *n) { /* Handled in SubDec */ }

void CodeGenVisitor::Visit(CompStmt *n)
{
    if(n->optitonalStmts)
        n->optitonalStmts->accept(this);
}

void CodeGenVisitor::Visit(OptionalStmts *n)
{
    n->stmtList->accept(this);
}

void CodeGenVisitor::Visit(StmtList *n)
{
    for (auto *stmt : *n->stmts)
    {
        stmt->accept(this);
    }
}

void CodeGenVisitor::Visit(IdExp *e)
{
    if (e->id->symbol)
    {
        Symbol *sym = e->id->symbol;
        if (sym->Kind == GLOBAL_VAR)
        {
            emit("PUSHG " + to_string(sym->Offset));
        }
        else
        { // LOCAL_VAR or PARAM_VAR
            emit("PUSHL " + to_string(sym->Offset));
        }
    }
}

void CodeGenVisitor::Visit(Assign *n)
{
    n->exp->accept(this); // The value to be assigned is now on top of the stack

    // Check if this is a function return assignment
    if (currentFunctionContext && n->var->id->name == currentFunctionContext->id->name)
    {
        int num_params = 0;
        if (currentFunctionContext->args && currentFunctionContext->args->parList)
        {
            for (auto p_dec : *currentFunctionContext->args->parList->parList)
            {
                num_params += p_dec->identList->identLst->size();
            }
        }
        // Return value is at fp[-(2 + num_params)]
        emit("STOREL " + to_string(-(1 + num_params)));
    }
    else if (n->var->id->symbol)
    {
        Symbol *sym = n->var->id->symbol;
        if (sym->Kind == GLOBAL_VAR)
        {
            emit("STOREG " + to_string(sym->Offset));
        }
        else
        { // LOCAL_VAR or PARAM_VAR
            emit("STOREL " + to_string(sym->Offset));
        }
    }
}

void CodeGenVisitor::Visit(IfThen *n)
{
    string endLabel = newLabel();
    n->expr->accept(this);
    emit("JZ " + endLabel);
    n->stmt->accept(this);
    emitLabel(endLabel);
}

void CodeGenVisitor::Visit(IfThenElse *n)
{
    string elseLabel = newLabel();
    string endLabel = newLabel();
    n->expr->accept(this);
    emit("JZ " + elseLabel);
    n->trueStmt->accept(this);
    emit("JUMP " + endLabel);
    emitLabel(elseLabel);
    n->falseStmt->accept(this);
    emitLabel(endLabel);
}

void CodeGenVisitor::Visit(While *n)
{
    string startLabel = newLabel();
    string endLabel = newLabel();
    emitLabel(startLabel);
    n->expr->accept(this);
    emit("JZ " + endLabel);
    n->stmt->accept(this);
    emit("JUMP " + startLabel);
    emitLabel(endLabel);
}

void CodeGenVisitor::Visit(FuncCall *n)
{
    // Allocate space for the return value
    emit("PUSHN 1");
    // Push arguments
    if (n->exps)
    {
        for (auto *exp : *n->exps->expList)
        {
            exp->accept(this);
        }
    }
    emit("PUSHA f" + n->id->symbol->funcSig->getSignatureString());
    emit("CALL");

    int num_params = 0;
    if (n->id->symbol && n->id->symbol->funcSig && n->id->symbol->funcSig->paramTypes) {
        num_params = n->id->symbol->funcSig->paramTypes->size();
    }
    if (num_params > 0) {
        emit("POP " + to_string(num_params));
    }

}

void CodeGenVisitor::Visit(ProcStmt *n)
{
    // Push arguments
    if (n->expls)
    {
        for (auto *exp : *n->expls->expList)
        {
            exp->accept(this);
        }
    }
    //? Built in write method
    if (n->id->name == "write")
    {
        if (n->expls && !n->expls->expList->empty())
        {
            Exp *argExp = n->expls->expList->at(0);

            switch (argExp->type)
            {
            case INTTYPE:
            case BOOLTYPE: 
                emit("WRITEI");
                break;
            case REALTYPE:
                emit("WRITEF");
                break;
            default:
                //! Should Not Happen
                // No code is emitted if the type is not printable.
                break;
            }
        }
    }
    else
    {
        emit("PUSHA p" + n->id->symbol->funcSig->getSignatureString());
        emit("CALL");

        int num_params = 0;
        if (n->id->symbol && n->id->symbol->funcSig && n->id->symbol->funcSig->paramTypes) {
            num_params = n->id->symbol->funcSig->paramTypes->size();
        }
        if (num_params > 0) {
            emit("POP " + to_string(num_params));
        }
    }
}

void CodeGenVisitor::Visit(Integer *n) { emit("PUSHI " + to_string(n->val)); }
void CodeGenVisitor::Visit(Real *n) { emit("PUSHF " + to_string(n->val)); }
void CodeGenVisitor::Visit(Bool *n) { emit("PUSHI " + to_string(n->val)); }

void CodeGenVisitor::Visit(Add *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->type == REALTYPE ? "FADD" : "ADD");
}

void CodeGenVisitor::Visit(Sub *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->type == REALTYPE ? "FSUB" : "SUB");
}

void CodeGenVisitor::Visit(Mult *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->type == REALTYPE ? "FMUL" : "MUL");
}

void CodeGenVisitor::Visit(Divide *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("FDIV"); // Division always results in a real
}

void CodeGenVisitor::Visit(IntDiv *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("DIV"); // Integer division
}

void CodeGenVisitor::Visit(GT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FSUP" : "SUP");
}

void CodeGenVisitor::Visit(LT *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FINF" : "INF");
}

void CodeGenVisitor::Visit(GE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FSUPEQ" : "SUPEQ");
}

void CodeGenVisitor::Visit(LE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FINFEQ" : "INFEQ");
}

void CodeGenVisitor::Visit(ET *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("EQUAL");
}

void CodeGenVisitor::Visit(NE *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("EQUAL");
    emit("NOT");
}

void CodeGenVisitor::Visit(And *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("ADD"); // A short-circuiting AND would be better
    emit("PUSHI 2");
    emit("EQUAL");
}

void CodeGenVisitor::Visit(Or *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("ADD"); // A short-circuiting OR would be better
    emit("PUSHI 0");
    emit("SUP");
}

void CodeGenVisitor::Visit(Not *n)
{
    n->exp->accept(this);
    emit("NOT");
}

void CodeGenVisitor::Visit(UnaryMinus *n)
{
    n->exp->accept(this);
    if (n->type == REALTYPE)
    {
        emit("PUSHF -1.0");
        emit("FMUL");
    }
    else
    { // INTTYPE
        emit("PUSHI -1");
        emit("MUL");
    }
}

// Stubs for remaining visits
void CodeGenVisitor::Visit(ArrayElement *n) { /* Stub */ }
void CodeGenVisitor::Visit(ExpList *n) { /* Stub */ }
