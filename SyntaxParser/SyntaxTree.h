#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <string>
#include <utility>
#include <vector>
#include "ZhjSmartPointer.h"
using std::string;
using std::pair;
using std::vector;

namespace zhjcompiler {
class SyntaxNode {
public:
	enum SyntaxNodeType {
		FUNCTIONLIST,
		FUNCTION,
		PROCEDURE,
		ARGUMENTS,
		ARGUMENT,
		DECLARATIONS,
		DECLARATION,
		IDENTIFIERLIST,
		TYPE,
		COMPOUNDSTMT,
		ASSIGNSTMT,
		PROCEDURESTMT,
		IFSTMT,
		WHILESTMT,
		DOWHILESTMT,
		RETURNSTMT,
		EXPRESSIONLIST,
		EXPRESSION,
		SIMPLEEXPRESSION,
		TERM,
		FACTOR,
		TERMINAL
	};
	virtual	SyntaxNodeType type() const = 0;
	virtual ~SyntaxNode();
};

class NonTerminalNode : public SyntaxNode
{
public:
	NonTerminalNode(SyntaxNodeType type);
	SyntaxNodeType type() const;
	void addChild(ZhjSmartPointer<SyntaxNode> child);
	vector<ZhjSmartPointer<SyntaxNode> > childs();
	int childCnt();
private:
	SyntaxNodeType type_;
	vector<ZhjSmartPointer<SyntaxNode> > childs_;
};

class TerminalNode : public SyntaxNode
{
public:
	typedef pair<string, string> Token;
	typedef pair<Token, int> TokenInfo;
	TerminalNode(TokenInfo token);
	SyntaxNodeType type() const;
	TokenInfo tokenInfo();
	string tokenStr();
	string tokenType();
	int line();
private:
	TokenInfo tokenInfo_;
};
}
#endif
