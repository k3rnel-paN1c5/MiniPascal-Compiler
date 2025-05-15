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
class IdExp;
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
class Visitor;

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
    virtual void  accept(Visitor* ); 
};
class Prog : public Node 
{
public: 
    Ident *name;
    Decs *declarations;
    SubDecs *subDeclarations;
    CompStmt *compoundStatment;
    Prog(Ident *, Decs *, SubDecs *, CompStmt *, int, int);
    virtual void  accept(Visitor* );
 };

class Ident : public Node
{
public:
    string name;
    Ident(string, int, int);
    virtual void  accept(Visitor* ); 
};

class Decs : public Node 
{
public:
    vector<ParDec *> * decs;
    Decs(int, int);
    void AddDec(ParDec*); 
    virtual void  accept(Visitor* ); 
};
class ParDec : public Node
{
public:
    IdentList* identList;
    Type* tp;
    ParDec(IdentList*, Type *, int, int);
    
    virtual void  accept(Visitor* ); 
};
class IdentList : public Node
{
public:
    vector<Ident *> *identLst;
    IdentList(Ident*, int, int);
    void AddIdent(Ident* );
    virtual void  accept(Visitor* ); 
};
class Type : public Node
{
public:
    Type(int, int);
    virtual void  accept(Visitor* ); 
};
class StdType : public Type
{
public:
    TypeEnum type;
    StdType(TypeEnum, int, int);
    virtual void  accept(Visitor* ); 
};
class Array : public Type
{
public:
    int beginIndex;
    int endIndex;
    StdType* stdType;
    Array(int, int, StdType* , int, int);
    virtual void  accept(Visitor* ); 
};

class SubDecs : public Node 
{
public:
    vector<SubDec*>* subdecs;
    SubDecs(int, int);
    void AddDec(SubDec*);
    virtual void  accept(Visitor* ); 
};
class SubDec : public Node 
{
public: 
    SubHead* subHead;
    CompStmt* compStmt;
    SubDec(SubHead*, CompStmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class SubHead : public Node
{
public:
    SubHead(int, int);
    virtual void  accept(Visitor* ); 
};
class Func : public SubHead 
{
public:
    Ident* id;
    Args* args;
    StdType* typ;
    Func(Ident*, Args*, StdType*, int, int);
    virtual void  accept(Visitor* ); 
};
class Proc : public SubHead 
{
public:
    Ident* id;
    Args* args;
    Proc(Ident*, Args*, int, int);
    virtual void  accept(Visitor* ); 
};
class Args : public Node 
{
public:
    ParList* parList;
    Args(ParList*, int, int);
    virtual void  accept(Visitor* ); 
};
class ParList : public Node
{
public:
    vector<ParDec*> * parList;
    ParList(ParDec*, int, int);
    void AddDec(ParDec*);
    virtual void  accept(Visitor* ); 
};
class OptionalStmts : public Node
{
    public:
    StmtList* stmtList;
    OptionalStmts(StmtList*,int, int);
    virtual void  accept(Visitor* ); 
};
class StmtList : public Node 
{
    public:
    vector<Stmt*> * stmts;
    StmtList(Stmt*, int, int);
    void AddStmt(Stmt*);
    virtual void  accept(Visitor* ); 
};
class Stmt : public Node 
{
    public:     
    Stmt(int, int);
    virtual void  accept(Visitor* ); 
};
class CompStmt : public Stmt 
{
public:
    OptionalStmts* optitonalStmts;
    CompStmt(OptionalStmts*, int, int);
    virtual void  accept(Visitor* ); 
};
class Assign : public Stmt
{
public:
    Var* var;
    Exp* exp;
    Assign(Var*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class ProcStmt : public Stmt
{
public:
    Ident* id;
    ExpList* expls;
    ProcStmt(Ident*, ExpList*, int, int);
    virtual void  accept(Visitor* ); 
};
class ExpList : public Node
{
public:
    vector<Exp*> * expList;
    ExpList(Exp*, int, int);
    void AddExp(Exp*);
    virtual void  accept(Visitor* ); 
};
class Exp : public Node
{
public:
    Exp(int, int);
    virtual void  accept(Visitor* ); 
};
class IdExp : public Exp
{
public:
    Ident* id;
    IdExp(Ident*, int, int);
    virtual void accept(Visitor* ); 
};
class Integer : public Exp
{
public:
    int val;
    Integer(int, int, int);
    virtual void  accept(Visitor* ); 
};
class Real : public Exp
{
public:
    float val;
    Real(float, int, int);
    virtual void  accept(Visitor* ); 
};
class Bool : public Exp
{
public:
    bool val;
    Bool(bool, int, int);
    virtual void  accept(Visitor* ); 
};
class FuncCall : public Exp
{
public:
    Ident* id;
    ExpList* exps;
    FuncCall(Ident*, ExpList*, int, int);
    virtual void  accept(Visitor* ); 
};
class Not: public Exp
{
public:
    Exp* exp;
    Not(Exp* , int, int);
    virtual void  accept(Visitor* ); 
};
class BinOp : public Exp 
{
public:
    Exp* leftExp;
    Exp* rightExp;
    BinOp(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Add : public BinOp 
{
public:
    Add(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Sub : public BinOp 
{
public:
    Sub(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Mult : public BinOp 
{
public:
    Mult(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Divide : public BinOp 
{
public:
    Divide(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Mod : public BinOp 
{
public:
    Mod(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class GT : public BinOp 
{
public:
    GT(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class LT : public BinOp 
{
public:
    LT(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class GE : public BinOp 
{
public:
    GE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class LE : public BinOp 
{
public:
    LE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class ET : public BinOp 
{
public:
    ET(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class NE : public BinOp 
{
public:
    NE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class And : public BinOp 
{
public:
    And(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Or : public BinOp 
{
public:
    Or(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class IfThen : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    IfThen(Exp* , Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class IfThenElse : public Stmt
{
public: 
    Exp* expr;
    Stmt* trueStmt;
    Stmt* falseStmt;
    IfThenElse(Exp* , Stmt*, Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class While : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    While(Exp* , Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};

class Var : public Node
{
public:
    Ident* id;
    Var(Ident*, int, int);
    virtual void  accept(Visitor* ); 
};
class ArrayElement : public Var
{
public:
    Exp* index;
    ArrayElement(Ident*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};

class Visitor {
public:
    virtual void Visit(Node*) = 0;
    virtual void Visit(Stmt*) = 0;
    virtual void Visit(Prog*) = 0;
    virtual void Visit(Ident*) = 0;
    virtual void Visit(Decs*) = 0;
    virtual void Visit(ParDec*) = 0;
    virtual void Visit(IdentList*) = 0;
    virtual void Visit(SubDecs*) = 0;
    virtual void Visit(SubDec*) = 0;
    virtual void Visit(SubHead*) = 0;
    virtual void Visit(Func*) = 0;
    virtual void Visit(Args*) = 0;
    virtual void Visit(ParList*) = 0;
    virtual void Visit(Proc*) = 0;
    virtual void Visit(FuncCall*) = 0;
    virtual void Visit(CompStmt*) = 0;
    virtual void Visit(OptionalStmts*) = 0;
    virtual void Visit(StmtList*) = 0;
    virtual void Visit(Var*) = 0;
    virtual void Visit(Exp*) = 0;
    virtual void Visit(Assign*) = 0;
    virtual void Visit(ProcStmt*) = 0;
    virtual void Visit(ExpList*) = 0;
    virtual void Visit(IfThen*) = 0;
    virtual void Visit(IfThenElse*) = 0;
    virtual void Visit(While*) = 0;
    virtual void Visit(Type*) = 0;
    virtual void Visit(StdType*) = 0;
    virtual void Visit(IdExp*) = 0;
    virtual void Visit(Integer*) = 0;
    virtual void Visit(Real*) = 0;
    virtual void Visit(Bool*) = 0;
    virtual void Visit(Array*) = 0;
    virtual void Visit(ArrayElement*) = 0;
    virtual void Visit(BinOp*) = 0;
    virtual void Visit(Add*) = 0;
    virtual void Visit(Sub*) = 0;
    virtual void Visit(Mult*) = 0;
    virtual void Visit(Divide*) = 0;
    virtual void Visit(Mod*) = 0;
    virtual void Visit(GT*) = 0;
    virtual void Visit(LT*) = 0;
    virtual void Visit(GE*) = 0;
    virtual void Visit(LE*) = 0;
    virtual void Visit(ET*) = 0;
    virtual void Visit(NE*) = 0;
    virtual void Visit(And*) = 0;
    virtual void Visit(Or*) = 0;
    virtual void Visit(Not*) = 0;
};


class PrintVisitor : public Visitor {
public:
    virtual void Visit(Node*);
    virtual void Visit(Stmt*);
    virtual void Visit(Prog*);
    virtual void Visit(Ident*);
    virtual void Visit(Decs*);
    virtual void Visit(ParDec*);
    virtual void Visit(IdentList*);
    virtual void Visit(SubDecs*);
    virtual void Visit(SubDec*);
    virtual void Visit(SubHead*);
    virtual void Visit(Func*);
    virtual void Visit(Args*);
    virtual void Visit(ParList*);
    virtual void Visit(Proc*);
    virtual void Visit(FuncCall*);
    virtual void Visit(CompStmt*);
    virtual void Visit(OptionalStmts*);
    virtual void Visit(StmtList*);
    virtual void Visit(Var*);
    virtual void Visit(Exp*);
    virtual void Visit(Assign*);
    virtual void Visit(ProcStmt*);
    virtual void Visit(ExpList*);
    virtual void Visit(IfThen*);
    virtual void Visit(IfThenElse*);
    virtual void Visit(While*);
    virtual void Visit(Type*);
    virtual void Visit(StdType*);
    virtual void Visit(IdExp*);
    virtual void Visit(Integer*);
    virtual void Visit(Real*);
    virtual void Visit(Bool*);
    virtual void Visit(Array*);
    virtual void Visit(ArrayElement*);
    virtual void Visit(BinOp*);
    virtual void Visit(Add*);
    virtual void Visit(Sub*);
    virtual void Visit(Mult*);
    virtual void Visit(Divide*);
    virtual void Visit(Mod*);
    virtual void Visit(GT*);
    virtual void Visit(LT*);
    virtual void Visit(GE*);
    virtual void Visit(LE*);
    virtual void Visit(ET*);
    virtual void Visit(NE*);
    virtual void Visit(And*);
    virtual void Visit(Or*);
    virtual void Visit(Not*);
};
#endif