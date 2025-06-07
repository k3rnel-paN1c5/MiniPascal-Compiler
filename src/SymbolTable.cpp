#include <iostream>
#include "CommonTypes.h"
#include "SymbolTable.h"
#include "Error.h"

using namespace std;


Errors *errorStack = new Errors();




FunctionSignature::FunctionSignature(string n, vector<Type*>* params, TypeEnum ret)
{
    this->name = n;
    this->returnType = ret;
    this->paramTypes = params;
}

string FunctionSignature::getSignatureString()
{
    string res = this->name + 'D';
    if(this->paramTypes==NULL)
        return res;
    for (int i = 0; i < this->paramTypes->size(); i++)
    {
        if (i > 0)
            res += 'D';
            TypeEnum x;
            if(dynamic_cast<StdType*>(this->paramTypes->at(i))){
                StdType* s = dynamic_cast<StdType*>(this->paramTypes->at(i));
                x = s->type;
            }
            else{
                Array* s = dynamic_cast<Array*>(this->paramTypes->at(i));
                x = s->stdType->type;

            }
        res += TypeEnumToString(x);
    }
    return res;
}

Symbol::Symbol(string name, SymbolKind kind, TypeEnum type)
{
    this->Name = name;
    this->Kind = kind;
    this->DataType = type;
    this->funcSig = NULL;
    this->Offset = 0; // Initialize offset
}

Symbol::Symbol(string name, SymbolKind kind, FunctionSignature *sig)
{
    this->Name = name;
    this->Kind = kind;
    this->DataType = sig->returnType;
    this->funcSig = sig;
    this->Offset = 0; // Initialize offset
}

Scope::Scope()
{
    this->hashTab = new HashTable();
    this->Parent = NULL;
    this->Children = new vector<Scope *>;
    //todo : check these
    // Initialize offsets. Parameters are at negative offsets from FP.
    this->param_offset = -1;
    // Locals are at positive offsets from FP.
    this->local_offset = 0;
}

void Scope::AddChildScope(Scope *s)
{
    this->Children->push_back(s);
    s->Parent = this;
}

SymbolTable::SymbolTable()
{
    this->Scopes = new vector<Scope *>();
    this->Scopes->push_back(new Scope());
    this->currentScope = this->Scopes->at(0);
    this->rootScope = this->currentScope;
}

bool SymbolTable::AddSymbol(Ident *ident, SymbolKind kind, Type* type)
{

    string key;
    int offset = 0;
    switch (kind)
    {
    case PARAM_VAR:
        key = "par" + ident->name; // parameter var for function or procedure
        offset = this->currentScope->param_offset--;
        break;
    case GLOBAL_VAR:
        key = 'g' + ident->name; // global variables
        offset = this->rootScope->local_offset++;
        break;
    case LOCAL_VAR:
        key = 'l' + ident->name; // local variable
        offset = this->currentScope->local_offset++;
        break;
    default:
        cout << "Error in symbol table, invalid kind for a variable \n";
        return false;
        break;
    }

    Symbol *temp = this->currentScope->hashTab->GetMember(key);
    if (temp)
    {
        errorStack->AddError( "Redifintion Of Variable: " + ident->name, ident->line+1, ident->column);
        return false;
    }
    TypeEnum typ;
    if(dynamic_cast<StdType*>(type)){
        StdType* x = dynamic_cast<StdType*>(type);
        typ = x->type;
    }
    else{
        Array* x = dynamic_cast<Array*>(type);
        switch (x->stdType->type)
        {
        case INTTYPE:
            typ = INT_ARRAY;
            break;
        case REALTYPE:
            typ = REAL_ARRAY;
            break;
        case BOOLTYPE:
            typ = BOOL_ARRAY;
            break;
        default:
            cout <<" Error in add symbol\n";
            break;
        }
    }
    Symbol *newSymbol = new Symbol(ident->name, kind, typ);
    newSymbol->Offset = offset;
    this->currentScope->hashTab->AddKey(key, newSymbol);
    ident->symbol = newSymbol;
    return true;
}

bool SymbolTable::AddSymbol(Ident *ident, SymbolKind kind, FunctionSignature *sig)
{
    string key;
    switch (kind)
    {
    case FUNC:
        key = "f";
        break;
    case PROC:
        key = "p";
        break;
    default:
        cout << "Error in symbol table, invalid kind for a function/procedure \n";
        return false;
        break;
    }

    key = key + sig->getSignatureString();
    Symbol *temp = this->rootScope->hashTab->GetMember(key);

    if (temp)
    {
        if (kind == PROC)
            errorStack->AddError("Redifinition of Procedure: " + ident->name, ident->line+1, ident->column);
        else
            errorStack->AddError("Redifinition of Function: " + ident->name, ident->line+1, ident->column);
        return false;
    }
    Symbol *newSymbol = new Symbol(ident->name, kind, sig);
    this->rootScope->hashTab->AddKey(key, newSymbol);
    ident->symbol = newSymbol;
    return true;
}

Symbol *SymbolTable::LookUpSymbol(Ident *ident)
{
    string key;
    Symbol *sym;
    
    Scope *scope_to_check = this->currentScope;
    while (scope_to_check != NULL)
    {
        key = "l" + ident->name;
        sym = scope_to_check->hashTab->GetMember(key);
        if (sym)
        {
            ident->symbol = sym;
            return sym;
        }
        // now look if there exist a parameter variable with that name in the currrent scope (function/procedure)
        key = "par" + ident->name;
        sym = scope_to_check->hashTab->GetMember(key);
        if (sym)
        {
            ident->symbol = sym;
            return sym;
        }
        scope_to_check = scope_to_check->Parent;
    }
    // if there  were  no local variables or parameters
    key = 'g' + ident->name;
    sym = this->rootScope->hashTab->GetMember(key);
    if (sym != NULL)
    {
        ident->symbol = sym;
        return sym;
    }
    errorStack->AddError("Undeclared Variable: " + ident->name, ident->line+1, ident->column);
    return NULL;
}

Symbol *SymbolTable::LookUpSymbol(Ident *ident, SymbolKind kind, vector<TypeEnum>* paramTypes)
{
    string key;
    Symbol *sym;
    switch (kind)
    {
    case FUNC:
        key = 'f';
        break;
    case PROC:
        key = 'p';
        break;
    default:
        cout << "Error in lookup function/procedure, invalid kind\n";
        break;
    }
    key += ident->name + 'D';
    if(paramTypes != NULL)
        for(int i = 0; i <  paramTypes->size(); i++){
            if(i > 0) key +='D';
            key += TypeEnumToString(paramTypes->at(i));
        }
    sym = this->rootScope->hashTab->GetMember(key);
    if(sym != NULL){
        ident->symbol = sym;
        return sym;
    }

    if (kind == FUNC)
        errorStack->AddError("Undeclared Function: " + key, ident->line+1, ident->column);
    else
        errorStack->AddError("Undeclared Procedure: " + key, ident->line+1, ident->column);
    return NULL;
}

void SymbolTable::NewScope()
{
    Scope *newScope = new Scope();
    this->Scopes->push_back(newScope);
    this->currentScope->AddChildScope(newScope);
    this->currentScope = newScope;
}

void SymbolTable::CloseScope()
{
    this->currentScope = this->currentScope->Parent;
}