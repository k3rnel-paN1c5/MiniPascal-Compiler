#ifndef AST_H
#define AST_H

#include <vector>
using namespace std;

class Node;
class Prog;
class Ident;
class Decs;
class ParDec;
class IdentList;
class SubDecs;
class SubDec;
class Func;
class Args;
class ParList;
class Proc;
class CompStmt;
class OptionalStmts;
class StmtList;
class Stmt;
class Var;
class Exp;
class Assign;
class ProcStmt;
class ExpList;
class IfThen;
class IfThenElse;
class While;
class Type;
class StdType;
class IdExpr;
class Integer;
class Real;
class Boolean;
class Array;
class IntNum;
class RealNum;
class BoolVal;
class BinOp;
class Add;
class Sub;
class Mult;
class Divide;
class Mod;
class GT;
class LT;
class GE;
class LE;
class ET;
class NE;
class And;
class Or;
class Not;

enum TypeEnum 
{
    INTTYPE,
    REALTYPE,
    BOOLTYPE
};
class Node 
{
public:
    int line;
    int column;
    Node *father;
    Node(int, int);
};
class Prog : public Node 
{
public: 
    Ident *name;
    Decs *declarations;
    SubDecs *subDeclarations;
    CompStmt *compoundStatment;
    Prog(Ident *, Decs *, SubDecs *, CompStmt *, int, int);
};

class Ident : public Node
{
public:
    string name;
    Ident(string, int, int);
};

class Decs : public Node 
{
public:
    vector<ParDec *> * decs;
    Decs(int, int);
    void AddDec(ParDec*); 
};
class ParDec : public Node
{
public:
    IdentList* identList;
    Type* tp;
    ParDec(IdentList*, Type *, int, int);
    
};
class IdentList : public Node
{
public:
    vector<Ident *> *identLst;
    IdentList(Ident*, int, int);
    void AddIdent(Ident* );
};
class Type : public Node
{
public:
    TypeEnum typ;
    Type(TypeEnum, int, int);
};
class StdType : public Type
{
public:

    StdType(TypeEnum, int, int);
};
class Array : public Type
{
public:
    int beginIndex;
    int endIndex;
    Array(int, int, TypeEnum, int, int);
};

class SubDecs : public Node 
{
public:
    vector<SubDec*>* subdecs;
    SubDecs(int, int);
    void AddDec(SubDec*);
};
class SubDec : public Node 
{
public: 
    SubDec(int, int);
};
class Func : public SubDec 
{
public:
    Ident* id;
    Args* args;
    StdType* typ;
    Func(Ident*, Args*, StdType*, int, int);
};
class Proc : public SubDec 
{
public:
    Ident* id;
    Args* args;
    Proc(Ident*, Args*, int, int);
};
class Args : public Node 
{
public:
    ParList* parList;
    Args(ParList*, int, int);
};
class ParList : public Node
{
public:
    vector<ParDec*> * parList;
    ParList(ParDec*, int, int);
};
class CompStmt : public Stmt 
{
public:
    OptionalStmts* optitonalStmts;
    CompStmt(OptionalStmts*, int, int);
};
class OptionalStmts : public Node
{
public:
    StmtList* stmtList;
    OptionalStmts(StmtList*,int, int);
};
class StmtList : public Node 
{
public:
    vector<Stmt*> * stmts;
    StmtList(Stmt*, int, int);
    void AddStmt(Stmt*);
};
class Stmt : public Node 
{
public:     
    Stmt(int, int);
};
class Assign : public Stmt
{
public:
    Var* var;
    Exp* exp;
    Assign(Var*, Exp*, int, int);
};
class ProcStmt : public Stmt
{
public:
    Ident* id;
    ExpList* expls;
    ProcStmt(Ident*, ExpList*, int, int);
};
class ExpList : public Node
{
public:
    vector<Exp*> * expList;
    ExpList(Exp*, int, int);
    void AddExp(Exp*);
};
class Exp : public Node
{
public:
    Exp(int, int);
};
class IdExp : public Exp
{
public:
    Ident* id;
    IdExp(Ident*, int, int);
};

#endif