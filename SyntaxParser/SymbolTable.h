#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Stack.h"
#include <vector>
#include <string>
#include <map>
#include <utility>
using std::vector;
using std::string;
using std::map;
using std::pair;
namespace zhjcompiler {

class Type {
public:
	enum ConcreteType {
		VOID,
		INTEGER,
		REAL,
		BOOLEAN,
		ERRORTYPE
	};
	
	// simple type
	Type(ConcreteType type = VOID);

	// function type
	Type(const vector<ConcreteType> &paraTypes, ConcreteType returnType);
	
	ConcreteType type() const;
	bool isFunction() const;
	const vector<ConcreteType> &paraTypes() const;
	ConcreteType returnType() const;
	bool isError() const;
	static ConcreteType strToType(string typeStr);
private:
	bool isFunction_;
	ConcreteType type_;
	vector<ConcreteType> paraTypes_;
	ConcreteType returnType_;
};

class SymbolTable {
public:
	SymbolTable();

	void addSymbol(string id, Type type);
	bool searchSymbol(string id, Type &type);

private:
	map<string, Type> symbolTable_;
};	// end of class SymbolTable

class SymbolTableStack {
public:
	SymbolTableStack();

	void addSymbol(string id, Type type);
	bool searchSymbol(string id, Type &type);
	bool searchGlobalSymbol(string id, Type &type);
	
	void pushEmptySymbolTable();
	void popSymbolTable();
	const SymbolTable &topSymbolTable();

	size_t size() const;
private:
	Stack<SymbolTable> symbolTableStack_;
};	// end of class SymbolTableStack

}	// end of namespace 
#endif
