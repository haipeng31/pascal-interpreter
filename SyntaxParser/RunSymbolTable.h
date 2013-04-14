#ifndef RUNSYMBOLTABLE_H
#define RUNSYMBOLTABLE_H

#include "Stack.h"
#include <map>
#include <string>
#include <utility>
using std::map;
using std::string;
using std::pair;

namespace zhjcompiler {
class RunSymbolTable {
public:
	RunSymbolTable(size_t returnAddr);

	size_t returnAddr() const;
	bool symbolExists(const string &symbol) const;
	size_t symbolAddr(const string &symbol) const;
	void setSymbolAddr(const string &symbol, size_t addr);
	size_t size() const;
private:
	size_t returnAddr_;
	map<string, size_t> symbolAddrs_;
};	// end of class

class RunSymbolTableStack {
public:
	RunSymbolTableStack();

	void pushSymbolTable(size_t returnAddr);
	void popSymbolTable();

	void addSymbol(const string &symbol, size_t addr);
	bool symbolExists(const string &symbol) const;
	size_t symbolAddr(const string &symbol) const;
	size_t returnAddr() const;
	size_t symbolSize() const;
private:
	Stack<RunSymbolTable> symbolTableStack_;
};	// end of class
}	// end of namespace 
#endif
