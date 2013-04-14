#include "SyntaxTree.h"
#include "SyntaxTreeGenerator.h"
#include "Stack.h"
#include "SymbolTable.h"
#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::stringstream;
using namespace zhjcompiler;

int main()
{
	SymbolTableStack tableStack;
	
	tableStack.pushEmptySymbolTable();
	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << i;
		Type type(Type::INTEGER);
		tableStack.addSymbol(ss.str(), type); 
	}

	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << i;
		Type type;
		tableStack.searchSymbol(ss.str(), type);
		assert(type.type() == Type::INTEGER);
	}

	vector<Type::ConcreteType> paraTypes;
	paraTypes.push_back(Type::INTEGER);
	paraTypes.push_back(Type::REAL);

	Type funcType(paraTypes, Type::BOOLEAN);
	tableStack.addSymbol("funcid", funcType);
	Type searchType;
	tableStack.searchSymbol("funcid", searchType);
	assert(searchType.returnType() == Type::BOOLEAN);
	assert(searchType.paraTypes() == paraTypes);

	cout << "past all test" << endl;
	cout << "past all test" << endl;
	return 0;
}
