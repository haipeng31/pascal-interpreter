#include "RunSymbolTable.h"
#include <cassert>

using namespace zhjcompiler;

RunSymbolTable::RunSymbolTable(size_t returnAddr)
 :	returnAddr_(returnAddr),
	symbolAddrs_()
{
}

size_t RunSymbolTable::returnAddr() const
{
	return returnAddr_;
}

bool RunSymbolTable::symbolExists(const string &symbol) const
{
	map<string, size_t>::const_iterator mit = symbolAddrs_.find(symbol);
	if (mit == symbolAddrs_.end()) {
		return false;
	} else {
		return true;
	}
}

size_t RunSymbolTable::symbolAddr(const string &symbol) const
{
	assert(symbolExists(symbol) == true);

	map<string, size_t>::const_iterator mit = symbolAddrs_.find(symbol);
	return mit->second;
}

void RunSymbolTable::setSymbolAddr(const string &symbol, size_t addr)
{
	assert(symbolExists(symbol) == false);

	symbolAddrs_.insert(pair<string, size_t>(symbol, addr));
}

size_t RunSymbolTable::size() const
{
	return symbolAddrs_.size();
}

RunSymbolTableStack::RunSymbolTableStack()
 :	symbolTableStack_()
{
}

void RunSymbolTableStack::pushSymbolTable(size_t returnAddr)
{
	symbolTableStack_.push(RunSymbolTable(returnAddr));
}

void RunSymbolTableStack::popSymbolTable()
{
	symbolTableStack_.pop();
}

void RunSymbolTableStack::addSymbol(const string &symbol, size_t addr)
{
	assert(symbolTableStack_.size() > 0);
	assert(!symbolExists(symbol));

	symbolTableStack_[symbolTableStack_.size()-1].setSymbolAddr(symbol, addr);
}

bool RunSymbolTableStack::symbolExists(const string &symbol) const
{
	assert(symbolTableStack_.size() > 0);

	return symbolTableStack_.top().symbolExists(symbol);
}

size_t RunSymbolTableStack::symbolAddr(const string &symbol) const
{
	assert(symbolExists(symbol));

	return symbolTableStack_.top().symbolAddr(symbol);
}

size_t RunSymbolTableStack::returnAddr() const
{
	assert(symbolTableStack_.size() > 0);

	return symbolTableStack_.top().returnAddr();
}
size_t RunSymbolTableStack::symbolSize() const
{
	assert(symbolTableStack_.size() > 0);
	return symbolTableStack_.top().size();
}
