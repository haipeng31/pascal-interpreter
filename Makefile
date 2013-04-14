OBJ = VirtualMachine.o RunSymbolTable.o RunStackItem.o CodeGenerator.o SemanticAnalyser.o SymbolTable.o SyntaxTree.o SyntaxTreeGenerator.o RegularExpLoader.o RegularExp.o NFAModel.o NFAConverter.o DFAConverter.o DFAModel.o TokenScanner.o
CFLAGS = -c -g -Wall -I TokenScanner -I SyntaxParser

main:$(OBJ) main.o
	g++ $(OBJ) main.o -o main

main.o:main.cpp
	g++ $(CFLAGS) main.cpp
VirtualMachine.o:SyntaxParser/VirtualMachine.cpp
	g++ $(CFLAGS) SyntaxParser/VirtualMachine.cpp
RunSymbolTable.o:SyntaxParser/RunSymbolTable.cpp
	g++ $(CFLAGS) SyntaxParser/RunSymbolTable.cpp
RunStackItem.o:SyntaxParser/RunStackItem.cpp
	g++ $(CFLAGS) SyntaxParser/RunStackItem.cpp
CodeGenerator.o:SyntaxParser/CodeGenerator.cpp
	g++ $(CFLAGS) SyntaxParser/CodeGenerator.cpp
SemanticAnalyser.o:SyntaxParser/SemanticAnalyser.cpp
	g++ $(CFLAGS) SyntaxParser/SemanticAnalyser.cpp
SymbolTable.o:SyntaxParser/SymbolTable.cpp
	g++ $(CFLAGS) SyntaxParser/SymbolTable.cpp

SyntaxTree.o:SyntaxParser/SyntaxTree.cpp
	g++ $(CFLAGS) SyntaxParser/SyntaxTree.cpp

SyntaxTreeGenerator.o:SyntaxParser/SyntaxTreeGenerator.cpp
	g++ $(CFLAGS) SyntaxParser/SyntaxTreeGenerator.cpp

TokenScanner.o:TokenScanner/TokenScanner.cpp
	g++ $(CFLAGS) TokenScanner/TokenScanner.cpp

DFAConverter.o:TokenScanner/DFAConverter.cpp
	g++ $(CFLAGS) TokenScanner/DFAConverter.cpp

DFAModel.o:TokenScanner/DFAModel.cpp
	g++ $(CFLAGS) TokenScanner/DFAModel.cpp

NFAConverter.o:TokenScanner/NFAConverter.cpp
	g++ $(CFLAGS) TokenScanner/NFAConverter.cpp

NFAModel.o:TokenScanner/NFAModel.cpp
	g++ $(CFLAGS) TokenScanner/NFAModel.cpp

RegularExpLoader.o:TokenScanner/RegularExpLoader.cpp
	g++ $(CFLAGS) TokenScanner/RegularExpLoader.cpp

RegularExp.o:TokenScanner/RegularExp.cpp
	g++ $(CFLAGS) TokenScanner/RegularExp.cpp

clean:
	rm *.o
