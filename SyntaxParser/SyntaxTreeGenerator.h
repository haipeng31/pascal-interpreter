#ifndef SYNTAXTREEGENERATOR_H
#define SYNTAXTREEGENERATOR_H

#include "SyntaxTree.h"
#include "ZhjSmartPointer.h"
#include <utility>
#include <string>
#include <vector>
using std::vector;
using std::string;
using std::pair;

namespace zhjcompiler {
class SyntaxTreeGenerator {
public:
	typedef pair<string, string> Token;
	typedef pair<Token, int> TokenInfo;

	SyntaxTreeGenerator(vector<TokenInfo> tokenInfos);
	ZhjSmartPointer<SyntaxNode> getFunctionList();
	ZhjSmartPointer<SyntaxNode> getFunction();
	ZhjSmartPointer<SyntaxNode> getProcedure();
	ZhjSmartPointer<SyntaxNode> getArguments();
	ZhjSmartPointer<SyntaxNode> getArgument();
	ZhjSmartPointer<SyntaxNode> getDeclarations();
	ZhjSmartPointer<SyntaxNode> getDeclaration();
	ZhjSmartPointer<SyntaxNode> getIdentifierList();
	ZhjSmartPointer<SyntaxNode> getType();
	ZhjSmartPointer<SyntaxNode> getCompoundStmt();
	ZhjSmartPointer<SyntaxNode> getAssignStmt();
	ZhjSmartPointer<SyntaxNode> getProcedureStmt();
	ZhjSmartPointer<SyntaxNode> getIfStmt();
	ZhjSmartPointer<SyntaxNode> getWhileStmt();
	ZhjSmartPointer<SyntaxNode> getDoWhileStmt();
	ZhjSmartPointer<SyntaxNode> getReturnStmt();
	ZhjSmartPointer<SyntaxNode> getExpressionList();
	ZhjSmartPointer<SyntaxNode> getExpression();
	ZhjSmartPointer<SyntaxNode> getSimpleExpression();
	ZhjSmartPointer<SyntaxNode> getTerm();
	ZhjSmartPointer<SyntaxNode> getFactor();
	ZhjSmartPointer<SyntaxNode> getRelationOp();
	ZhjSmartPointer<SyntaxNode> getSign();
	ZhjSmartPointer<SyntaxNode> getAddOp();
	ZhjSmartPointer<SyntaxNode> getMulOp();
	
private:
	string tokenStr(int index);
	string tokenType(int index);
	int tokenLine(int index);
	Token token(int index);
	TokenInfo tokenInfo(int index);
	void error(string functionName);

	bool inStmtFirst();
	ZhjSmartPointer<SyntaxNode> getStmt();
	bool inExpressionFirst();
	bool isRelationOp();
	bool isAddOp();
	bool isMulOp();

	ZhjSmartPointer<SyntaxNode> getLeftOver(string str);
	ZhjSmartPointer<SyntaxNode> getId();
	ZhjSmartPointer<SyntaxNode> getInt();
	ZhjSmartPointer<SyntaxNode> getFloat();
	ZhjSmartPointer<SyntaxNode> getBoolean();
	ZhjSmartPointer<SyntaxNode> getOp(string opStr);
	ZhjSmartPointer<SyntaxNode> getLBracket();
	ZhjSmartPointer<SyntaxNode> getRBracket();
	ZhjSmartPointer<SyntaxNode> getSemicolon();
	ZhjSmartPointer<SyntaxNode> getColon();
	ZhjSmartPointer<SyntaxNode> getComma();
	vector<TokenInfo> tokenInfos_;
	int index_;
};

}
#endif
