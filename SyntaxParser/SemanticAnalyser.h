#ifndef SEMATICANALYSER_H
#define SEMATICANALYSER_H

#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "ZhjSmartPointer.h"

namespace zhjcompiler {

class SemanticAnalyser {
public:
	SemanticAnalyser();

	Type checkFuncList(ZhjSmartPointer<SyntaxNode> funcList);
	Type checkFunc(ZhjSmartPointer<SyntaxNode> func, string &funcName);
	Type checkStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkExp(ZhjSmartPointer<SyntaxNode> exp);

	Type checkParameters(ZhjSmartPointer<SyntaxNode> paras, vector<Type::ConcreteType> &paraTypes);
	Type checkDeclarations(ZhjSmartPointer<SyntaxNode> declas);

	Type checkCompoundStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkAssignStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkProcedureStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkIfStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkWhileStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkDoWhileStmt(ZhjSmartPointer<SyntaxNode> stmt);
	Type checkReturnStmt(ZhjSmartPointer<SyntaxNode> stmt);
	
	Type checkSimpleExp(ZhjSmartPointer<SyntaxNode> simExp);
	Type checkTerm(ZhjSmartPointer<SyntaxNode> term);
	Type checkFactor(ZhjSmartPointer<SyntaxNode> factor);
	Type checkRelationOperands(Type leftType, Type rightType, string opStr);
	Type checkAddOperands(Type leftType, Type rightType, string opStr);
	Type checkMulOperands(Type leftType, Type rightType, string opStr);
	Type checkFuncFactor(ZhjSmartPointer<SyntaxNode> factor);
private:
	SymbolTableStack symbolStack_;

	// 1 means in function, 2 means in procedure, others 0
	int inFunc_;
	Type::ConcreteType funcReturnType_;

	int loopLevel_;
};
}	// end of namespace 
#endif
