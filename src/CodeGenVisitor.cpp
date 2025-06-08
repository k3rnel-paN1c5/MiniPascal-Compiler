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

void CodeGenVisitor::emitBoundsCheck(Symbol* arraySymbol) {
    string lowerOkLabel = newLabel();
    string upperOkLabel = newLabel();

    // Lower bound check: index < beginIndex?
    emit("// --- Array Bounds Check ---\n");
    emit("DUP 1");
    emit("PUSHI " + to_string(arraySymbol->beginIndex));
    emit("INF"); 

    emit("JZ " + lowerOkLabel);
    emit("ERR \"Runtime Error: Array index out of bounds.\"");
    emit("STOP");
    emitLabel(lowerOkLabel);

    // Upper bound check:  index > endIndex?
    emit("DUP 1");
    emit("PUSHI " + to_string(arraySymbol->endIndex));
    emit("SUP");

    emit("JZ " + upperOkLabel);
    emit("ERR \"Runtime Error: Array index out of bounds.\"");
    emit("STOP");
    emitLabel(upperOkLabel);
    emit("// --- End Bounds Check ---\n");

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
        emit("// --- Global Variables ---\n");
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

        for (auto *dec : *n->declarations->decs)
        {
            if (Array *arr = dynamic_cast<Array *>(dec->tp))
            {
                int size = arr->endIndex - arr->beginIndex + 1;
                for (auto *id : *dec->identList->identLst)
                {
                    emit("PUSHI " + to_string(size));
                    emit("ALLOCN");                                  // Allocates block, pushes base address
                    emit("STOREG " + to_string(id->symbol->Offset)); // Store base address in global var slot
                }
            }
        }
        emit("// --- End of Global Variables ---\n");
    }

    emit("\n\n");
    emit("// --- Main ---\n");
    if (n->compoundStatment)
        n->compoundStatment->accept(this);
    emit("STOP");
    if (n->subDeclarations)
        n->subDeclarations->accept(this);
        


    if (outFile.is_open())
    {
        outFile.close();
    }
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
    emit("// --- Sub Declaration Definition ---\n");
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

        for (auto *l_dec : *n->localDecs->localDecs)
        {
            if (Array *arr = dynamic_cast<Array *>(l_dec->tp))
            {
                int size = arr->endIndex - arr->beginIndex + 1;
                for (auto *id : *l_dec->identlist->identLst)
                {
                    emit("PUSHI " + to_string(size));
                    emit("ALLOCN");
                    emit("STOREL " + to_string(id->symbol->Offset));
                }
            }
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
    if (n->optitonalStmts)
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

    Symbol *sym = e->id->symbol;
    if (!sym)
        return;

    if (sym->Kind == GLOBAL_VAR)
    {
        emit("PUSHG " + to_string(sym->Offset));
    }
    else
    { // LOCAL_VAR or PARAM_VAR
        emit("PUSHL " + to_string(sym->Offset));
    }
}
void CodeGenVisitor::Visit(ArrayExp *a)
{

    Symbol *sym = a->id->symbol;
    if (!sym)
        return;

    if (sym->Kind == GLOBAL_VAR)
    {
        emit("PUSHG " + to_string(sym->Offset));
    }
    else
    {
        emit("PUSHL " + to_string(sym->Offset));
    }
    // stack: [xxxx, base_address]

    a->index->accept(this); // push index
     emitBoundsCheck(sym); // * check the index to be in range;
    emit("PUSHI " + to_string(sym->beginIndex));
    emit("SUB"); // reall index (k) = index - begIndex
    // Stack: [xxxx, base_address, k]

    emit("LOADN");
    // Stack: [xxxx, value]
}
void CodeGenVisitor::Visit(Assign *n)
{
    n->exp->accept(this);

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
        // return value at fp[-(1 + num_params)]
        emit("STOREL " + to_string(-(1 + num_params)));
    }
    else if (n->var->id->symbol)
    {
        Symbol *sym = n->var->id->symbol;
        if (ArrayElement *a = dynamic_cast<ArrayElement *>(n->var))
        {
            //? Stack [xxx, val]
            if (sym->Kind == GLOBAL_VAR)
            {
                emit("PUSHG " + to_string(sym->Offset));
            }
            else
            {
                emit("PUSHL " + to_string(sym->Offset));
            }

            //? Stack [xxx, val, ArrayAddress]
            emit("SWAP");

            //? Stack [xxx, ArrayAddress,val]

            a->index->accept(this);
            //? Stack [xxx, ArrayAddress,val, indexAccess]

            emitBoundsCheck(sym); // * check the index to be in range;


            emit("PUSHI " + to_string(sym->beginIndex));
            //? Stack [xxx, ArrayAddress,val, indexAccess, begIndex]
            emit("SUB");
            //? Stack [xxx, ArrayAddress,val, k]  // k is the real index

            emit("SWAP");

            //? Stack [xxx, ArrayAddress, val, k]
            //* that's what storn needs

            emit("STOREN");
        }
        else if (sym->Kind == GLOBAL_VAR)
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
    emit("// --- If Then Statement ---\n");
    string endLabel = newLabel();
    n->expr->accept(this);
    emit("JZ " + endLabel);
    n->stmt->accept(this);
    emitLabel(endLabel);
}

void CodeGenVisitor::Visit(IfThenElse *n)
{
    emit("// --- If Then Else Statement ---\n");
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
    emit("// --- While Statement ---\n");
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
    emit("// --- Calling a Function ---\n");
    // allocate space for the return value
    emit("PUSHN 1");
    // Push arguments
    if (n->exps)
    {
        int siz = n->exps->expList->size();
        for (int i = siz - 1; i >= 0; i--)
        {
            n->exps->expList->at(i)->accept(this);
        }
    }
    emit("PUSHA f" + n->id->symbol->funcSig->getSignatureString());
    emit("CALL");

    int num_params = 0;
    if (n->id->symbol && n->id->symbol->funcSig && n->id->symbol->funcSig->paramTypes)
    {
        num_params = n->id->symbol->funcSig->paramTypes->size();
    }
    if (num_params > 0)
    {
        emit("POP " + to_string(num_params));
    }
}

void CodeGenVisitor::Visit(ProcStmt *n)
{
    emit("// --- Calling a Procedure Statement ---\n");
    // Push arguments
    if (n->expls)
    {
        int siz = n->expls->expList->size();
        for (int i = siz - 1; i >= 0; i--)
        {
            n->expls->expList->at(i)->accept(this);
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
                break;
            }
        }
    }
    else
    {
        emit("PUSHA p" + n->id->symbol->funcSig->getSignatureString());
        emit("CALL");

        int num_params = 0;
        if (n->id->symbol && n->id->symbol->funcSig && n->id->symbol->funcSig->paramTypes)
        {
            num_params = n->id->symbol->funcSig->paramTypes->size();
        }
        if (num_params > 0)
        {
            emit("POP " + to_string(num_params));
        }
    }
}

void CodeGenVisitor::Visit(Integer *n) { emit("PUSHI " + to_string(n->val)); }
void CodeGenVisitor::Visit(Real *n) { emit("PUSHF " + to_string(n->val)); }
void CodeGenVisitor::Visit(Bool *n) { emit("PUSHI " + to_string(n->val)); }

void CodeGenVisitor::Visit(Add *b)
{
    emit("// --- Addition Op ---\n");

    b->leftExp->accept(this);
    if (b->type == REALTYPE && b->leftExp->type == INTTYPE) {
        emit("ITOF");
    }
    b->rightExp->accept(this);
    if (b->type == REALTYPE && b->rightExp->type == INTTYPE) {
        emit("ITOF");
    }

    emit(b->type == REALTYPE ? "FADD" : "ADD");
}

void CodeGenVisitor::Visit(Sub *b)
{
    emit("// --- Subtraction Op ---\n");

    b->leftExp->accept(this);
    if (b->type == REALTYPE && b->leftExp->type == INTTYPE) {
        emit("ITOF");
    }
    b->rightExp->accept(this);
    if (b->type == REALTYPE && b->rightExp->type == INTTYPE) {
        emit("ITOF");
    }

    emit(b->type == REALTYPE ? "FSUB" : "SUB");
}

void CodeGenVisitor::Visit(Mult *b)
{
    emit("// --- Multiplication Op ---\n");
    
    b->leftExp->accept(this);
    if (b->type == REALTYPE && b->leftExp->type == INTTYPE) {
        emit("ITOF");
    }
    b->rightExp->accept(this);
    if (b->type == REALTYPE && b->rightExp->type == INTTYPE) {
        emit("ITOF");
    }

    emit(b->type == REALTYPE ? "FMUL" : "MUL");
}

void CodeGenVisitor::Visit(Divide *b)
{
    emit("// --- Division Op ---\n");
    string okLabel = newLabel();
    string ProbLabel = newLabel();
    
    b->leftExp->accept(this);
    if (b->type == REALTYPE && b->leftExp->type == INTTYPE) {
        emit("ITOF");
    }
    b->rightExp->accept(this);
    if (b->type == REALTYPE && b->rightExp->type == INTTYPE) {
        emit("ITOF");
    }
    

    // emit("// --- Division by Zero Check ---\n");
    emit("DUP 1");
    emit("FTOI");
    emit("JZ " + ProbLabel);
    emit("JUMP " + okLabel);
    
    emitLabel(ProbLabel);
    emit("ERR \"Runtime Error: Division by zero.\"");
    emit("STOP");
    emitLabel(okLabel);
    emit("// --- End Check ---\n");

    emit("FDIV"); 
}

void CodeGenVisitor::Visit(IntDiv *b)
{
    emit("// --- Integer Division Op ---\n");
    string ProbLabel = newLabel();
    string okLabel = newLabel();
    
    b->leftExp->accept(this);

    b->rightExp->accept(this);


    emit("// --- Division by Zero Check ---\n");
    emit("DUP 1");
    emit("JZ " + ProbLabel);
    emit("JUMP " + okLabel);
    
    emitLabel(ProbLabel);
    emit("ERR \"Runtime Error: Division by zero.\"");
    emit("STOP");
    emitLabel(okLabel);
    emit("// --- End Check ---\n");

    emit("DIV"); 
}

void CodeGenVisitor::Visit(GT *b)
{
    emit("// --- Greater Than Op ---\n");
    
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FSUP" : "SUP");
}

void CodeGenVisitor::Visit(LT *b)
{
    emit("// --- Less Than Op ---\n");
    
   b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FINF" : "INF");
}

void CodeGenVisitor::Visit(GE *b)
{
    emit("// --- Greater Than or Equal Op ---\n");
    
   b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FSUPEQ" : "SUPEQ");
}

void CodeGenVisitor::Visit(LE *b)
{
    emit("// --- Less Than or Equal Op ---\n");
    
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit(b->leftExp->type == REALTYPE || b->rightExp->type == REALTYPE ? "FINFEQ" : "INFEQ");
}

void CodeGenVisitor::Visit(ET *b)
{
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit("EQUAL");
}

void CodeGenVisitor::Visit(NE *b)
{
    emit("// --- Not Equal Op ---\n");
    
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    if (b->rightExp->type == INTTYPE && b->leftExp->type == REALTYPE) {
        emit("ITOF");
    }
    else if(b->rightExp->type == REALTYPE && b->leftExp->type == INTTYPE){
        emit("SWAP");
        emit("ITOF");
        emit("SWAP");
    }

    emit("EQUAL");
    emit("NOT");
}

void CodeGenVisitor::Visit(And *b)
{
    emit("// --- And Op ---\n");
    string falseLabel = newLabel();
    string endLabel = newLabel();

    // b->leftExp->accept(this);
    // b->rightExp->accept(this);
    // emit("ADD"); 
    // emit("PUSHI 2");
    // emit("EQUAL");

    // Short Cirtuting the And operation
    b->leftExp->accept(this);
    emit("JZ " + falseLabel);

    b->rightExp->accept(this); 
    emit("JZ " + falseLabel);

    emit("PUSHI 1");           
    emit("JUMP " + endLabel);

    emitLabel(falseLabel);
    emit("PUSHI 0");        

    emitLabel(endLabel);

}

void CodeGenVisitor::Visit(Or *b)
{
    emit("// --- Or Op ---\n");
    b->leftExp->accept(this);
    b->rightExp->accept(this);
    emit("ADD");
    emit("PUSHI 0");
    emit("SUP");
}

void CodeGenVisitor::Visit(Not *n)
{
    emit("// --- Not Op ---\n");
    n->exp->accept(this);
    emit("NOT");
}

void CodeGenVisitor::Visit(UnaryMinus *n)
{
    emit("// --- Negation Op ---\n");
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

void CodeGenVisitor::Visit(ArrayElement *a)
{
}
void CodeGenVisitor::Visit(ExpList *n) {}
