#include "SymbolTable.h"

using namespace zhjcompiler;

Type::Type(ConcreteType type)
 :	isFunction_(false),
	type_(type),
	paraTypes_(),
	returnType_(VOID)
{
}

Type::Type(const vector<ConcreteType> &paraTypes, ConcreteType returnType)
 :	isFunction_(true),
	type_(VOID),
	paraTypes_(paraTypes),
	returnType_(returnType)
{
}

Type::ConcreteType Type::type() const
{
	return type_;
}

bool Type::isFunction() const
{
	return isFunction_;
}

const vector<Type::ConcreteType> &Type::paraTypes() const
{
	return paraTypes_;
}

Type::ConcreteType Type::returnType() const
{
	return returnType_;
}

bool Type::isError() const
{
	return type_ == ERRORTYPE;
}

Type::ConcreteType Type::strToType(string typeStr)
{
	if (typeStr == "integer") {
		return INTEGER;
	} else if (typeStr == "real") {
		return REAL;
	} else if (typeStr == "boolean") {
		return BOOLEAN;
	} else if (typeStr == "void") {
		return VOID;
	} else {
		return ERRORTYPE;
	}
}
SymbolTable::SymbolTable()
 :	symbolTable_()
{
}

void SymbolTable::addSymbol(string id, Type type)
{
	map<string, Type>::iterator mit = symbolTable_.find(id);
	assert(mit == symbolTable_.end());

	symbolTable_.insert(pair<string, Type>(id, type));
}

bool SymbolTable::searchSymbol(string id, Type &type)
{
	map<string, Type>::iterator mit = symbolTable_.find(id);
	if (mit == symbolTable_.end()) {
		return false;
	} else {
		type = mit->second;
		return true;
	}
}

SymbolTableStack::SymbolTableStack()
 :	symbolTableStack_()
{
}

void SymbolTableStack::addSymbol(string id, Type type)
{
	symbolTableStack_[symbolTableStack_.size()-1].addSymbol(id, type);
}

bool SymbolTableStack::searchSymbol(string id, Type &type)
{
	return symbolTableStack_[symbolTableStack_.size()-1].searchSymbol(id, type);
}

bool SymbolTableStack::searchGlobalSymbol(string id, Type &type)
{
	for (int i = symbolTableStack_.size() - 1; i >= 0; i--) {
		if (symbolTableStack_[i].searchSymbol(id, type)) {
			return true;
		}
	}

	return false;
}

void SymbolTableStack::pushEmptySymbolTable()
{
	SymbolTable emptyTable;
	symbolTableStack_.push(emptyTable);
}

void SymbolTableStack::popSymbolTable()
{
	symbolTableStack_.pop();
}

const SymbolTable &SymbolTableStack::topSymbolTable()
{
	return symbolTableStack_.top();
}

size_t SymbolTableStack::size() const
{
	return symbolTableStack_.size();
}
