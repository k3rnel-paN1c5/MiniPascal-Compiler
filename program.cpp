#include "ast.h"
#include "parser.h"
#include <cstdio>    
#include <cstdlib>
#include <iostream>
extern int yydebug;
extern int yyparse();
extern Func* root;
extern FILE* yyin;


int main(int argc, char* argv[]) {
    yydebug = 1;  // Enable debug if needed
    if (argc < 2) {

        std::cerr << "If you want to take input from a file then \n Usage: " << argv[0] << " <input-file>\n";
        yyparse(); 
        return 0;

    }
    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening file");
        return 1;
    }
    yyin = input;

    yyparse(); 
    fclose(input);
    return 0;
}