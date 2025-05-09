#ifndef AST_H
#define AST_H

#include <vector>
using namespace std;

class Node;
class Stmt;
class Prog;
class Ident;
class Decs;
class ParDec;
class IdentList;
class SubDecs;
class SubDec;
class SubHead;
class Func;
class Args;
class ParList;
class Proc;
class CompStmt;
class OptionalStmts;
class StmtList;
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
class Bool;
class Array;
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
enum OpType {
    OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD,
    OP_GT, OP_GE, OP_LT, OP_LE, OP_ET, OP_NE,
    OP_AND, OP_OR
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
    Type(int, int);
};
class StdType : public Type
{
public:
    TypeEnum type;
    StdType(TypeEnum, int, int);
};
class Array : public Type
{
public:
    int beginIndex;
    int endIndex;
    StdType* stdType;
    Array(int, int, StdType* , int, int);
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
    SubHead* subHead;
    CompStmt* compStmt;
    SubDec(SubHead*, CompStmt*, int, int);
};
class SubHead : public Node
{
public:
    SubHead(int, int);
};
class Func : public SubHead 
{
public:
    Ident* id;
    Args* args;
    StdType* typ;
    Func(Ident*, Args*, StdType*, int, int);
};
class Proc : public SubHead 
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
    void AddDec(ParDec*);
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
class CompStmt : public Stmt 
{
public:
    OptionalStmts* optitonalStmts;
    CompStmt(OptionalStmts*, int, int);
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
class Integer : public Exp
{
public:
    int val;
    Integer(int, int, int);
};
class Real : public Exp
{
public:
    float val;
    Real(float, int, int);
};
class Bool : public Exp
{
public:
    bool val;
    Bool(bool, int, int);
};
class FuncCall : public Exp
{
public:
    Ident* id;
    ExpList* exps;
    FuncCall(Ident*, ExpList*, int, int);
};
class Not: public Exp
{
public:
    Exp* exp;
    Not(Exp* , int, int);
};
class BinOp : public Exp 
{
public:
    Exp* leftExp;
    Exp* rightExp;
    BinOp(Exp*, Exp*, int, int);
};
class Add : public BinOp 
{
public:
    Add(Exp*, Exp*, int, int);
};
class Sub : public BinOp 
{
public:
    Sub(Exp*, Exp*, int, int);
};
class Mult : public BinOp 
{
public:
    Mult(Exp*, Exp*, int, int);
};
class Divide : public BinOp 
{
public:
    Divide(Exp*, Exp*, int, int);
};
class Mod : public BinOp 
{
public:
    Mod(Exp*, Exp*, int, int);
};
class GT : public BinOp 
{
public:
    GT(Exp*, Exp*, int, int);
};
class LT : public BinOp 
{
public:
    LT(Exp*, Exp*, int, int);
};
class GE : public BinOp 
{
public:
    GE(Exp*, Exp*, int, int);
};
class LE : public BinOp 
{
public:
    LE(Exp*, Exp*, int, int);
};
class ET : public BinOp 
{
public:
    ET(Exp*, Exp*, int, int);
};
class NE : public BinOp 
{
public:
    NE(Exp*, Exp*, int, int);
};
class And : public BinOp 
{
public:
    And(Exp*, Exp*, int, int);
};
class Or : public BinOp 
{
public:
    Or(Exp*, Exp*, int, int);
};
class IfThen : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    IfThen(Exp* , Stmt*, int, int);
};
class IfThenElse : public Stmt
{
public: 
    Exp* expr;
    Stmt* trueStmt;
    Stmt* falseStmt;
    IfThenElse(Exp* , Stmt*, Stmt*, int, int);
};
class While : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    While(Exp* , Stmt*, int, int);
};

class Var : public Node
{
public:
    Ident* id;
    Var(Ident*, int, int);
};
class ArrayElement : public Var
{
public:
    Exp* index;
    ArrayElement(Ident*, Exp*, int, int);
};
#endif