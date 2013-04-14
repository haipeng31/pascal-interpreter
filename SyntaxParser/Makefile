OBJ = SyntaxTree.o SyntaxTreeGenerator.o SymbolTable.o SemanticAnalyser.o

CFLAGS = -c -g -Wall
main:$(OBJ) main.o
	g++ $(OBJ) main.o -o main

main.o:main.cpp
	g++ $(CFLAGS) main.cpp

SemanticAnalyser.o:SemanticAnalyser.cpp
	g++ $(CFLAGS) SemanticAnalyser.cpp

SymbolTable.o:SymbolTable.cpp
	g++ $(CFLAGS) SymbolTable.cpp

SyntaxTree.o:SyntaxTree.cpp
	g++ $(CFLAGS) SyntaxTree.cpp

SyntaxTreeGenerator.o:SyntaxTreeGenerator.cpp
	g++ $(CFLAGS) SyntaxTreeGenerator.cpp

clean:
	rm *.o
