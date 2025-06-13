#include "ast.h"
#include "Error.h"
#include "parser.h"
#include "CommonTypes.h"
#include <cstdio>    
#include <cstdlib>
#include <iostream>

using namespace std;
extern int yydebug;
extern int yyparse();
extern Prog* root;
extern FILE* yyin;
extern Errors *errorStack;
extern SymbolTable* symbolTable;

void initializeBuiltInFunctions(SymbolTable* st) {
    // Built-in procedure: write(integer)
    {
        Ident* writeIdent = new Ident("write", 0, 0);
        std::vector<Type*>* params = new std::vector<Type*>();
        params->push_back(new StdType(INTTYPE, 0, 0));
        FunctionSignature* sig = new FunctionSignature("write", params, VOID);
        st->AddSymbol(writeIdent, PROC, sig);
        delete writeIdent; // Clean up temporary Ident
    }

    // Built-in procedure: write(real)
    {
        Ident* writeIdent = new Ident("write", 0, 0);
        std::vector<Type*>* params = new std::vector<Type*>();
        params->push_back(new StdType(REALTYPE, 0, 0));
        FunctionSignature* sig = new FunctionSignature("write", params, VOID);
        st->AddSymbol(writeIdent, PROC, sig);
        delete writeIdent;
    }

    // Built-in procedure: write(boolean) - will print 0 or 1
    {
        Ident* writeIdent = new Ident("write", 0, 0);
        std::vector<Type*>* params = new std::vector<Type*>();
        params->push_back(new StdType(BOOLTYPE, 0, 0));
        FunctionSignature* sig = new FunctionSignature("write", params, VOID);
        st->AddSymbol(writeIdent, PROC, sig);
        delete writeIdent;
    }
}


int main(int argc, char* argv[]) {
    yydebug = 0;  // Enable debug if needed
    string output_filename = "build/output.vm";
    if (argc < 2) {

        cerr << "If you want to pass input from a file then \n Usage: " << argv[0] << " <input-file>\n";
        cerr << "Compiling from standard input." << endl;
        yyin = stdin;

    }
    else{
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening input file");
            return 1;
        }
        // Check for output file argument
        if (argc > 3 && string(argv[2]) == "-o") {
            output_filename = argv[3];
        }
    }   
    initializeBuiltInFunctions(symbolTable);
    // Parsing
    yyparse(); 
    if (!root) {
        cerr << "Parsing failed." << endl;
        if (yyin != stdin) fclose(yyin);
        return 1;
    }

    // cout << "--- AST ---" << endl;
    Visitor* printVisitor = new PrintVisitor();
    // root->accept(printVisitor);

    Visitor* typeVisitor = new TypeVisitor();
    root->accept(typeVisitor);


    errorStack->PrintWarnings();
    errorStack->PrintErrors();

    if (errorStack->errorStack->empty()) {
        cout << "No errors found. Generating code to " << output_filename << "..." << endl;
        CodeGenVisitor* codeGen = new CodeGenVisitor(output_filename);
        root->accept(codeGen);

        cout << "Code generation complete." << endl;
    } else {
        if (yyin != stdin) fclose(yyin);
        return 1;
    }
    
    if (yyin != stdin) fclose(yyin);
    return 0;
}