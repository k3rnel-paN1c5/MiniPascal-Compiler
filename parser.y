%{
    #include <iostream>
    using std::endl;
    using std::cout;
    extern int yylex();
    extern int yyerror(const char*);
    extern int yydebug;
    extern int lin, col;
%}

%token KPROGRAM
%token KVAR
%token KINT
%token KREAL
%token KFUNC
%token KPROC
%token KWHILE
%token KDO
%token KBEGIN
%token KEND
%token KIF
%token KTHEN
%token KELSE
%token KARRAY
%token KOF
%token KDIV
%token KNOT
%token KOR
%token KAND
%token KIDENT
%token KINTNUM

%%
st: KPROGRAM {
    cout << "KIDENT KINT KREAL";
} 
;
%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s << endl;
    return 0; 
}
