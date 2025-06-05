#include <iostream>
#include "CommonTypes.h"
#include "SymbolTable.h"
#include "Error.h"

using namespace std;


Errors *errorStack = new Errors();

// Helper to get TypeEnum from Type*
TypeEnum GetTypeEnumFromTypeNode(Type* typeNode) {
    if (!typeNode) return VOID; // Should not happen if grammar is correct

    if (auto* st = dynamic_cast<StdType*>(typeNode)) {
        return st->type;
    } else if (auto* at = dynamic_cast<Array*>(typeNode)) {
        if (at->stdType) {
            switch (at->stdType->type) {
                case INTTYPE: return INT_ARRAY;
                case REALTYPE: return REAL_ARRAY;
                case BOOLTYPE: return BOOL_ARRAY;
                default: return VOID; // Should not happen
            }
        }
    }
    return VOID; // Fallback
}


FunctionSignature::FunctionSignature(string n, vector<Type*>* params, TypeEnum ret)
{
    this->name = n;
    this->returnType = ret;
    this->paramTypes = params;
}

string FunctionSignature::getSignatureString()
{
    string res = this->name + '@';
    if(this->paramTypes==NULL)
        return res;
    for (int i = 0; i < this->paramTypes->size(); i++)
    {
        if (i > 0)
            res += ",";
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
}

Symbol::Symbol(string name, SymbolKind kind, FunctionSignature *sig)
{
    this->Name = name;
    this->Kind = kind;
    this->DataType = sig->returnType;
    this->funcSig = sig;
}

Scope::Scope()
{
    this->hashTab = new HashTable();
    this->Parent = NULL;
    this->Children = new vector<Scope *>;
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
    switch (kind)
    {
    case PARAM_VAR:
        key = "par" + ident->name; // parameter var for function or procedure
        break;
    case GLOBAL_VAR:
        key = 'g' + ident->name; // global variables
        break;
    case LOCAL_VAR:
        key = 'l' + ident->name; // local variable
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
    // first look if there exist a local variable with that name in the currrent scope
    key = "l" + ident->name;
    sym = this->currentScope->hashTab->GetMember(key);
    if (sym)
    {
        ident->symbol = sym;
        return sym;
    }
    // now look if there exist a parameter variable with that name in the currrent scope (function/procedure)
    key = "par" + ident->name;
    sym = this->currentScope->hashTab->GetMember(key);
    if (sym)
    {
        ident->symbol = sym;
        return sym;
    }
    Scope *temp = this->currentScope;
    while (temp->Parent != NULL)
    {
        temp = temp->Parent;
        key = "l" + ident->name;
        sym = temp->hashTab->GetMember(key);
        if (sym)
        {
            ident->symbol = sym;
            return sym;
        }
        // now look if there exist a parameter variable with that name in the currrent scope (function/procedure)
        key = "par" + ident->name;
        sym = temp->hashTab->GetMember(key);
        if (sym)
        {
            ident->symbol = sym;
            return sym;
        }
    }
    // if there  were  no local variables or parameters
    key = 'g' + ident->name;
    sym = this->rootScope->hashTab->GetMember(key);
    if (sym != NULL)
    {
        ident->symbol = sym;
        return sym;
    }
    errorStack->AddError("Undeclared Variable: " + ident->name, ident->line, ident->column);
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
    key += ident->name + '@';
    if(paramTypes != NULL)
        for(int i = 0; i <  paramTypes->size(); i++){
            if(i > 0) key +=',';
            key += TypeEnumToString(paramTypes->at(i));
        }
    sym = this->rootScope->hashTab->GetMember(key);
    if(sym != NULL){
        ident->symbol = sym;
        return sym;
    }

    if (kind == FUNC)
        errorStack->AddError("Undeclared Function: " + key, ident->line, ident->column);
    else
        errorStack->AddError("Undeclared Procedure: " + key, ident->line, ident->column);
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