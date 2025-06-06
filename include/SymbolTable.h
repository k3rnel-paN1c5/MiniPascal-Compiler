/**
 * @file SymbolTable.h
 * @brief Symbol table implementation for semantic analysis and scope management
 *
 * This header defines the symbol table infrastructure used during semantic analysis
 * of the Pascal-like programming language. It provides functionality for managing
 * identifiers, their types, and scope relationships throughout the compilation process.
 * The implementation supports nested scopes, symbol lookup across scope boundaries,
 * and function signature management for type checking.
 *
 * Key components include:
 * - Symbol: Individual symbol representation with type and location information
 * - Scope: Single scope container with hash table for efficient symbol lookup
 * - SymbolTable: Main symbol table manager handling scope hierarchy
 * - FunctionSignature: Function signature representation for overload resolution
 */
#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include "CommonTypes.h"
#include "ast.h"
#include "hash_fun.h"
#include "hash_table.h"

using namespace std;
/**
 * @enum SymbolKind
 * @brief Represents The kind of symbols (local variable, global variable, function, pocedure, parameter)
 */
enum SymbolKind
{
    PARAM_VAR = 1,  ///< parameter variable
    GLOBAL_VAR = 2, ///< global variable
    LOCAL_VAR = 3,  ///< local variables for functions
    FUNC = 4,       ///< function symbol
    PROC = 5        ///< procedure symbol
};

/**
 * @class FunctionSignature
 * @brief Represents a function or procedure signature
 *
 * Contains the complete signature information for functions and procedures,
 * including parameter types and return type. Used for function overload
 * resolution and type checking during function calls.
 */
class FunctionSignature
{
public:
    string name;                ///< Function or procedure name
    vector<Type *> *paramTypes; ///< List of parameter types in order
    TypeEnum returnType;        ///< Return type (VOID for procedures)

    /**
     * @brief Constructor for FunctionSignature
     * @param n Function or procedure name
     * @param params List of parameter types
     * @param ret Return type (defaults to VOID for procedures)
     */
    FunctionSignature(string n, vector<Type *> *params, TypeEnum ret = VOID);
    /**
     * @brief Generates a string representation of the function signature
     * @return String containing the complete signature for the key in the symbol table
     */
    string getSignatureString();
};

/**
 * @class Symbol
 * @brief Represents an entry in the symbol table
 *
 * Contains information about a declared identifier such as
 * its name, kind (variable, function, etc.), type, and memory location.
 * This class is used during semantic analysis and code generation phases
 * to track all declared identifiers and their properties.
 */
class Symbol
{
public:
    string Name;                ///< Symbol name (identifier)
    SymbolKind Kind;            ///< Symbol kind (variable, function, etc.)
    TypeEnum DataType;          ///< Data type (int, real, bool.)
    int Offset;                 ///< Memory offset for code generation
    FunctionSignature *funcSig; ///< Function signature (for functions/procedures only)
    /**
     * @brief Constructor for variable symbols
     * @param name Symbol name
     * @param kind Symbol kind (variable type)
     * @param type Data type of the symbol
     */
    Symbol(string name, SymbolKind kind, TypeEnum type);

    /**
     * @brief Constructor for function/procedure symbols
     * @param name Symbol name
     * @param kind Symbol kind (FUNC or PROC)
     * @param sig Function signature containing parameter and return types
     */
    Symbol(string name, SymbolKind kind, FunctionSignature *sig);
};

/**
 * @typedef HashTable
 * @brief Type alias for the hash table used in each scope
 *
 * Uses the CHashTable template class to store Symbol objects,
 * providing efficient symbol lookup within a scope.
 */
typedef CHashTable<Symbol> HashTable;

/**
 * @class Scope
 * @brief Represents a single scope in the program
 *
 * Contains a hash table of symbols defined in this scope and maintains
 * relationships with parent and child scopes for proper scope hierarchy.
 */
class Scope
{
public:
    Scope *Parent;             ///< Parent scope (null for global scope)
    HashTable *hashTab;        ///< Hash table for this scope
    vector<Scope *> *Children; ///< List of child scopes
    int local_offset;          ///< Current offset for local variables in this scope
    int param_offset;          ///< Current offset for parameters in this scope
    /**
     * @brief Constructor for Scope
     *
     * Initializes an empty scope with hash table and child list.
     */
    Scope();
    /**
     * @brief Adds a child scope to this scope
     * @param s The child scope to add
     */
    void AddChildScope(Scope *s);
};

/**
 * @class SymbolTable
 * @brief Manages the symbol tables for different scopes
 *
 * The SymbolTable class is responsible for managing all scopes in the program,
 * including the global scope and nested scopes. It provides functionality for:
 * - Creating new scopes
 * - Adding symbols to scopes
 * - Looking up symbols across scope boundaries
 * - Managing scope hierarchy
 *
 * This class is crucial for semantic analysis and ensuring proper
 * variable scoping rules are followed.
 */
class SymbolTable
{
public:
    Scope *rootScope;        ///< root program scope
    Scope *currentScope;     ///< current scope
    vector<Scope *> *Scopes; ///< List of inner scopes

    /**
     * @brief Constructor for SymbolTable
     *
     * Initializes the symbol table with a global scope.
     */
    SymbolTable();

    /**
     * @brief Adds a variable symbol to the current scope
     * @param ident Identifier node containing name and location info
     * @param kind Symbol kind (variable type)
     * @param type Type information for the symbol
     * @return true if symbol was added successfully, false if already exists
     */
    bool AddSymbol(Ident *ident, SymbolKind kind, Type *type);

    /**
     * @brief Adds a function/procedure symbol to the current scope
     * @param ident Identifier node containing name and location info
     * @param kind Symbol kind (FUNC or PROC)
     * @param sig Function signature containing parameter and return types
     * @return true if symbol was added successfully, false if already exists
     */
    bool AddSymbol(Ident *ident, SymbolKind kind, FunctionSignature *sig);

    /**
     * @brief Looks up a symbol by name across scope hierarchy
     * @param ident Identifier to search for
     * @return Pointer to symbol if found, null otherwise
     */
    Symbol *LookUpSymbol(Ident *ident);

    /**
     * @brief Looks up a function/procedure symbol with specific signature
     * @param ident Identifier to search for
     * @param kind Symbol kind to match
     * @param paramTypes Parameter types for signature matching
     * @return Pointer to symbol if found with matching signature, null otherwise
     */
    Symbol *LookUpSymbol(Ident *ident, SymbolKind kind, vector<TypeEnum> *paramTypes);

    /**
     * @brief Creates a new nested scope
     *
     * Creates a new scope as a child of the current scope and makes it active.
     */
    void NewScope();

    /**
     * @brief Closes the current scope
     *
     * Returns to the parent scope, making it the active scope.
     */
    void CloseScope();
};

#endif