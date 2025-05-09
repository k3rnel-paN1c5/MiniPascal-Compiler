all:
	flex -oscanner.cpp ./lexer.lex   
	bison -dtv -o parser.cpp --define=parser.h parser.y  
	g++ -std=c++17 ./scanner.cpp ./parser.cpp ./program.cpp -o mini_compiler 