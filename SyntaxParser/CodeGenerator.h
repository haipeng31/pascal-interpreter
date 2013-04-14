#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "SyntaxTree.h"
#include "ZhjSmartPointer.h"
#include <vector>
#include <string>
#include <utility>
#include <map>
using std::vector;
using std::string;
using std::map;
using std::pair;

namespace zhjcompiler {

class CodeGenerator {
public:
	CodeGenerator();

	void genCode(const ZhjSmartPointer<SyntaxNode> syntaxTree);
	const vector<string> &commands() const;
	size_t mainIndex() const;
private:
	void transferFuncList(const ZhjSmartPointer<SyntaxNode> funcList);
	void transferFunc(const ZhjSmartPointer<SyntaxNode> func);
	void transferIds(const ZhjSmartPointer<SyntaxNode> paras, string declaType);

	void transferStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferCompoundStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferIfStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferWhileStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferDoWhileStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferProcedureStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferAssignStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	void transferReturnStmt(const ZhjSmartPointer<SyntaxNode> stmt);
	
	void transferExp(const ZhjSmartPointer<SyntaxNode> exp);
	void transferSimpleExp(const ZhjSmartPointer<SyntaxNode> simpleExp);
	void transferTerm(const ZhjSmartPointer<SyntaxNode> term);
	void transferFactor(const ZhjSmartPointer<SyntaxNode> factor);
	void transferFuncCall(const ZhjSmartPointer<SyntaxNode> funcFactor);

	/* help functions */
	size_t funcCommandPos(const string &funcName) const;
	bool funcExists(const string &funcName) const;
	void setFuncCommandPos(const string &funcName, size_t pos);

	vector<string> commands_;
	map<string, size_t> funcCommandPos_;
	bool function_;
	size_t mainIndex_;
};

}
#endif
