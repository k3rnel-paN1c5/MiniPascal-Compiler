all:
	bison -dtv -o parser.cpp --define=parser.h parser.y  
	flex -oscanner.cpp ./lexer.lex   
	g++ -std=c++17 ./parser.cpp ./scanner.cpp ./ast.cpp ./PrintVisitor.cpp ./program.cpp -o MiniPasC 