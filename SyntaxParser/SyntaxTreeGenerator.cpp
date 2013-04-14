#include "SyntaxTreeGenerator.h"
#include <iostream>
#include <assert.h>
#include <stdlib.h>
using std::cout;
using std::endl;
using namespace zhjcompiler;

SyntaxTreeGenerator::SyntaxTreeGenerator(vector<TokenInfo> tokenInfos)
 :	tokenInfos_(tokenInfos),
	index_(0)
{
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getFunctionList()
{
	ZhjSmartPointer<NonTerminalNode> functionListNode(new NonTerminalNode(SyntaxNode::FUNCTIONLIST));
	while (1) {
		if (tokenStr(index_) == "function") {
			ZhjSmartPointer<SyntaxNode> functionNode = getFunction();
			functionListNode->addChild(functionNode);
		} else if (tokenStr(index_) == "procedure") {
			ZhjSmartPointer<SyntaxNode> procedureNode = getProcedure();
			functionListNode->addChild(procedureNode);
		}else if (tokenStr(index_) == "$") {			// end token
			return functionListNode;
		} else {
			error("getFunctionList");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getFunction()
{
	ZhjSmartPointer<NonTerminalNode> functionNode(new NonTerminalNode(SyntaxNode::FUNCTION));

	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("function");
	assert(node);
	functionNode->addChild(node);
	
	node = getId();
	assert(node);
	functionNode->addChild(node);
		
	/* ignore ( */
	node = getLBracket();

	node = getArguments();
	assert(node);
	functionNode->addChild(node);

	/* ignore ) */
	node = getRBracket();
	
	node = getColon();

	node = getType();
	assert(node);
	functionNode->addChild(node);

	node = getDeclarations();
	assert(node);
	functionNode->addChild(node);

	node = getCompoundStmt();
	assert(node);
	functionNode->addChild(node);
	
	return functionNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getProcedure()
{
	ZhjSmartPointer<NonTerminalNode> procedureNode(new NonTerminalNode(SyntaxNode::PROCEDURE));

	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("procedure");
	assert(node);
	procedureNode->addChild(node);
	
	node = getId();
	assert(node);
	procedureNode->addChild(node);
		
	/* ignore ( */
	node = getLBracket();
	
	node = getArguments();
	assert(node);
	procedureNode->addChild(node);
	
	/* ignore ) */
	node = getRBracket();
	
	node = getDeclarations();
	assert(node);
	procedureNode->addChild(node);

	node = getCompoundStmt();
	assert(node);
	procedureNode->addChild(node);
	
	return procedureNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getArguments()
{
	ZhjSmartPointer<NonTerminalNode> argumentsNode(new NonTerminalNode(SyntaxNode::ARGUMENTS));
	ZhjSmartPointer<SyntaxNode> node;
	while (1) {
		if (tokenType(index_) == "ID" && argumentsNode->childCnt() == 0) {
			node = getArgument();
			assert(node);
			argumentsNode->addChild(node);
		} else if (tokenStr(index_) == ";" && argumentsNode->childCnt() > 0) {
			node = getSemicolon();
			node = getArgument();
			assert(node);
			argumentsNode->addChild(node);
		} else if (tokenStr(index_) == ")") {
			return argumentsNode;
		} else {
			error("getArguments");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}			
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getArgument()
{
	ZhjSmartPointer<NonTerminalNode> argumentNode(new NonTerminalNode(SyntaxNode::ARGUMENT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getIdentifierList();
	assert(node);
	argumentNode->addChild(node);

	node = getColon();
	assert(node);

	node = getType();
	assert(node);
	argumentNode->addChild(node);

	return argumentNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getDeclarations()
{
	ZhjSmartPointer<NonTerminalNode> declarationsNode(new NonTerminalNode(SyntaxNode::DECLARATIONS));
	ZhjSmartPointer<SyntaxNode> node;
	
	while (1) {
		if (tokenStr(index_) == "var") {
			node = getDeclaration();
			assert(node);
			declarationsNode->addChild(node);
		} else if (tokenStr(index_) == "begin") {
			return declarationsNode;
		} else {
			error("getDeclarations");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}

	return ZhjSmartPointer<SyntaxNode>(NULL);
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getDeclaration()
{
	ZhjSmartPointer<NonTerminalNode> declarationNode(new NonTerminalNode(SyntaxNode::DECLARATION));
	ZhjSmartPointer<SyntaxNode> node;
	
	node = getLeftOver("var");
	assert(node);
	//declarationNode->addChild(node);

	node = getIdentifierList();
	assert(node);
	declarationNode->addChild(node);

	node = getColon();
	assert(node);

	node = getType();
	assert(node);
	declarationNode->addChild(node);

	node = getSemicolon();
	assert(node);

	return declarationNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getIdentifierList()
{
	ZhjSmartPointer<NonTerminalNode> identifierListNode(new NonTerminalNode(SyntaxNode::IDENTIFIERLIST));
	ZhjSmartPointer<SyntaxNode> node;
	while (1) {
		if (tokenType(index_) == "ID" && identifierListNode->childCnt() == 0) {
			node = getId();
			assert(node);
			identifierListNode->addChild(node);
		} else if (tokenStr(index_) == "," && identifierListNode->childCnt() > 0) {
			node = getComma();
			node = getId();
			assert(node);
			identifierListNode->addChild(node);
		} else if (tokenStr(index_) == ":" && identifierListNode->childCnt() > 0) {
			return identifierListNode;
		} else {
			error("getIdentifierList");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}			
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getType()
{
	ZhjSmartPointer<SyntaxNode> typeNode(NULL);
	string str = tokenStr(index_);
	if (str == "integer") {
		typeNode = getLeftOver("integer");
		assert(typeNode);
	} else if (str ==  "real") {
		typeNode = getLeftOver("real");
		assert(typeNode);
	} else if (str == "boolean") {
		typeNode = getLeftOver("boolean");
		assert(typeNode);
	} else {
		error("getType");
	}

	return typeNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getCompoundStmt()
{
	ZhjSmartPointer<NonTerminalNode> compoundStmtNode(new NonTerminalNode(SyntaxNode::COMPOUNDSTMT));
	ZhjSmartPointer<SyntaxNode> node(NULL);
	
	node = getLeftOver("begin");
	assert(node);

	while (1) {
		if (inStmtFirst() && compoundStmtNode->childCnt() == 0) {
			node = getStmt();
			assert(node);
			compoundStmtNode->addChild(node);
		} else if (tokenStr(index_) == ";" && compoundStmtNode->childCnt() > 0) {
			getSemicolon();

			node = getStmt();
			assert(node);
			compoundStmtNode->addChild(node);
		} else if (tokenStr(index_) == "end") {
			break;
		} else {
			error("getCompoundStmt");
			break;
		}
	}
	node = getLeftOver("end");

	return compoundStmtNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getAssignStmt()
{
	ZhjSmartPointer<NonTerminalNode> assignStmt(new NonTerminalNode(SyntaxNode::ASSIGNSTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getId();
	assert(node);
	assignStmt->addChild(node);

	node = getOp(":=");
	assert(node);
//	assignStmt->addChild(node);

	node = getExpression();
	assert(node);
	assignStmt->addChild(node);

	return assignStmt;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getIfStmt()
{
	ZhjSmartPointer<NonTerminalNode> ifStmt(new NonTerminalNode(SyntaxNode::IFSTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("if");
	assert(node);
//	ifStmt->addChild(node);
	
	node = getLBracket();
	assert(node);

	node = getExpression();
	assert(node);
	ifStmt->addChild(node);

	node = getRBracket();
	assert(node);

	node = getLeftOver("then");
	assert(node);
//	ifStmt->addChild(node);

	node = getStmt();
	assert(node);
	ifStmt->addChild(node);
	
	/* pay attention here doesn't have "else" */
	if (tokenStr(index_) == ";" || tokenStr(index_) == "end" ||
				tokenStr(index_) == "while") {
		return ifStmt;
	} else if (tokenStr(index_) == "else") {
		node = getLeftOver("else");
		assert(node);
//		ifStmt->addChild(node);

		node = getStmt();
		assert(node);
		ifStmt->addChild(node);
		return ifStmt;
	} else {
		error("getIfStmt");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getProcedureStmt()
{
	ZhjSmartPointer<NonTerminalNode> procedureStmt(new NonTerminalNode(SyntaxNode::PROCEDURESTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getId();
	assert(node);
	procedureStmt->addChild(node);

	node = getLBracket();
	assert(node);

	node = getExpressionList();
	assert(node);
	procedureStmt->addChild(node);

	node = getRBracket();
	assert(node);

	return procedureStmt;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getWhileStmt()
{
	ZhjSmartPointer<NonTerminalNode> whileStmt(new NonTerminalNode(SyntaxNode::WHILESTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("while");
	assert(node);
//	whileStmt->addChild(node);
	
	node = getLBracket();
	assert(node);

	node = getExpression();
	assert(node);
	whileStmt->addChild(node);

	node = getRBracket();
	assert(node);

	node = getLeftOver("do");
	assert(node);
//	whileStmt->addChild(node);

	node = getStmt();
	assert(node);
	whileStmt->addChild(node);
	
	return whileStmt;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getDoWhileStmt()
{
	ZhjSmartPointer<NonTerminalNode> doWhileStmt(new NonTerminalNode(SyntaxNode::DOWHILESTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("do");
	assert(node);
//	doWhileStmt->addChild(node);

	node = getStmt();
	assert(node);
	doWhileStmt->addChild(node);

	node = getLeftOver("while");
	assert(node);
//	doWhileStmt->addChild(node);
	
	node = getLBracket();
	assert(node);

	node = getExpression();
	assert(node);
	doWhileStmt->addChild(node);

	node = getRBracket();
	assert(node);
	
	return doWhileStmt;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getReturnStmt()
{
	ZhjSmartPointer<NonTerminalNode> returnStmt(new NonTerminalNode(SyntaxNode::RETURNSTMT));
	ZhjSmartPointer<SyntaxNode> node;

	node = getLeftOver("return");
	assert(node);
//	returnStmt->addChild(node);

	if (tokenStr(index_) == ";" || tokenStr(index_) == "end" || 
				tokenStr(index_) == "while" || tokenStr(index_) == "else") {
		return returnStmt;
	} else if (inExpressionFirst()) {
		node = getExpression();
		assert(node);
		returnStmt->addChild(node);
		return returnStmt;
	} else {
		error("getReturnStmt");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getExpressionList()
{
	ZhjSmartPointer<NonTerminalNode> expressionListNode(new NonTerminalNode(SyntaxNode::EXPRESSIONLIST));
	ZhjSmartPointer<SyntaxNode> node;

	node = getExpression();
	assert(node);
	expressionListNode->addChild(node);

	while (1) {
		if (tokenStr(index_) == ",") {
			node = getComma();
			assert(node);

			node = getExpression();
			assert(node);
			expressionListNode->addChild(node);
		} else if (tokenStr(index_) == ")") {
			return expressionListNode;
		} else {
			error("getExpressionList");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}

	return ZhjSmartPointer<SyntaxNode>(NULL);
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getExpression()
{
	ZhjSmartPointer<NonTerminalNode> expressionNode(new NonTerminalNode(SyntaxNode::EXPRESSION));
	ZhjSmartPointer<SyntaxNode> node;

	node = getSimpleExpression();
	assert(node);
	expressionNode->addChild(node);

	while (1) {
		if (isRelationOp()) {
			node = getRelationOp();
			assert(node);
			expressionNode->addChild(node);

			node = getSimpleExpression();
			assert(node);
			expressionNode->addChild(node);
		} else if (tokenStr(index_) == ")" || tokenStr(index_) == ";" || 
					tokenStr(index_) == "end" || tokenStr(index_) == "," ||
					tokenStr(index_) == "while" || tokenStr(index_) == "else") {
			return expressionNode;
		} else {
			error("getExpression");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}
	return ZhjSmartPointer<SyntaxNode>(NULL);
}


ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getSimpleExpression()
{
	ZhjSmartPointer<NonTerminalNode> simpleExpressionNode(new NonTerminalNode(SyntaxNode::SIMPLEEXPRESSION));
	ZhjSmartPointer<SyntaxNode> node;

	if (tokenStr(index_) == "+" || tokenStr(index_) == "-") {
		node = getSign();
		assert(node);
		simpleExpressionNode->addChild(node);
	}
	node = getTerm();
	assert(node);
	simpleExpressionNode->addChild(node);

	while (1) {
		if (isAddOp()) {
			node = getAddOp();
			assert(node);
			simpleExpressionNode->addChild(node);

			node = getTerm();
			assert(node);
			simpleExpressionNode->addChild(node);
		} else if (tokenStr(index_) == ")" || tokenStr(index_) == ";" || 
					tokenStr(index_) == "end" || tokenStr(index_) == "," ||
					tokenStr(index_) == "while" || tokenStr(index_) == "else" || isRelationOp()) {
			return simpleExpressionNode;
		} else {
			error("getSimpleExpression");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}
	return ZhjSmartPointer<SyntaxNode>(NULL);

}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getTerm()
{
	ZhjSmartPointer<NonTerminalNode> termNode(new NonTerminalNode(SyntaxNode::TERM));
	ZhjSmartPointer<SyntaxNode> node;
	
	node = getFactor();
	assert(node);
	termNode->addChild(node);

	while (1) {
		if (isMulOp()) {
			node = getMulOp();
			assert(node);
			termNode->addChild(node);

			node = getFactor();
			assert(node);
			termNode->addChild(node);
		} else if (tokenStr(index_) == ")" || tokenStr(index_) == ";" || 
					tokenStr(index_) == "end" || tokenStr(index_) == "," ||
					tokenStr(index_) == "while" || tokenStr(index_) == "else" ||
					isRelationOp() || isAddOp()) {
			return termNode;
		} else {
			error("getTerm");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	}
	return ZhjSmartPointer<SyntaxNode>(NULL);

}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getFactor()
{
	ZhjSmartPointer<NonTerminalNode> factorNode(new NonTerminalNode(SyntaxNode::FACTOR));
	ZhjSmartPointer<SyntaxNode> node;
	string typeStr = tokenType(index_);
	if (typeStr == "ID") {
		node = getId();
		assert(node);
		factorNode->addChild(node);

		if (tokenStr(index_) == "(") {
			node = getLBracket();
			assert(node);
			node = getExpressionList();
			assert(node);
			factorNode->addChild(node);
			node = getRBracket();
			assert(node);
		} else if (tokenStr(index_) == ")" || tokenStr(index_) == ";" || 
				tokenStr(index_) == "end" || tokenStr(index_) == "," || 
				tokenStr(index_) == "while" || tokenStr(index_) == "else" ||
				isRelationOp() || isAddOp() || isMulOp()) {
			// do nothing
		} else {
			error("getFactor");
			return ZhjSmartPointer<SyntaxNode>(NULL);
		}
	} else if (typeStr ==  "INT") {
		node = getInt();
		assert(node);
		factorNode->addChild(node);
	} else if (typeStr == "FLOAT") {
		node = getFloat();
		assert(node);
		factorNode->addChild(node);
	} else if (typeStr == "TRUE" || typeStr == "FALSE") {
		node = getBoolean();
		assert(node);
		factorNode->addChild(node);
	}else if (typeStr == "NOT") {
		node = getFactor();
		assert(node);
		factorNode->addChild(node);
	} else if (typeStr == "LBRACKET") {
		node = getLBracket();
		assert(node);
		node = getExpression();
		assert(node);
		factorNode->addChild(node);
		node = getRBracket();
		assert(node);
	} else {
		error("getFactor");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
	return factorNode;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getRelationOp()
{
	if (isRelationOp()) {
		return getOp(tokenStr(index_));
	} else {
		error("getRelationOp");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getSign()
{
	if (tokenStr(index_) == "+" || tokenStr(index_) == "-") {
		return getOp(tokenStr(index_));
	} else {
		error("getSign");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getAddOp()
{
	if (isAddOp()) {
		return getOp(tokenStr(index_));
	} else {
		error("getAddOp");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getMulOp()
{
	if (isMulOp()) {
		return getOp(tokenStr(index_));
	} else {
		error("getMulOp");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

string SyntaxTreeGenerator::tokenStr(int index)
{
	return tokenInfos_[index].first.second;
}

string SyntaxTreeGenerator::tokenType(int index)
{
	return tokenInfos_[index].first.first;
}

int SyntaxTreeGenerator::tokenLine(int index)
{
	return tokenInfos_[index].second;
}

SyntaxTreeGenerator::Token SyntaxTreeGenerator::token(int index)
{
	return tokenInfos_[index].first;
}

SyntaxTreeGenerator::TokenInfo SyntaxTreeGenerator::tokenInfo(int index)
{
	return tokenInfos_[index];
}

void SyntaxTreeGenerator::error(string funcName)
{
	cout << "error in " << funcName << "tokenStr " << tokenStr(index_) << "tokenType " << tokenType(index_) << "tokenLine " << tokenLine(index_) << endl;
	abort();
}

bool SyntaxTreeGenerator::inStmtFirst()
{
	string typeStr = tokenType(index_);
	bool stmtFirst = false;
	if (typeStr == "ID" || typeStr == "IF" || typeStr == "WHILE" ||
			typeStr == "DO" || typeStr == "RETURN" || typeStr == "BEGIN") {
		stmtFirst = true;
	}
	
	return stmtFirst;
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getStmt()
{
	ZhjSmartPointer<SyntaxNode> node(NULL);
	string typeStr = tokenType(index_);
	if (typeStr == "ID") {
		if (tokenStr(index_+1) == ":=") {
			node = getAssignStmt();
		} else if (tokenStr(index_ + 1) == "(") {
			node = getProcedureStmt();
		} else {
			error("getStmt");
		}
	} else if (typeStr == "IF") {
		node = getIfStmt();
	} else if (typeStr == "WHILE") {
		node = getWhileStmt();
	} else if (typeStr == "DO") {
		node = getDoWhileStmt();
	} else if (typeStr == "RETURN") {
		node = getReturnStmt();
	} else if (typeStr == "BEGIN") {
		node = getCompoundStmt();
	} else {
		error("getStmt");
	}
	return node;
}

bool SyntaxTreeGenerator::inExpressionFirst()
{
	bool inFirst = false;
	if (tokenType(index_) == "ID" || tokenType(index_) == "FLOAT" ||
				tokenType(index_) == "INT") {
		inFirst = true;
	} else if (tokenStr(index_) == "not" || tokenStr(index_) == "(" || tokenStr(index_) == "+" 
				|| tokenStr(index_) == "-") {
		inFirst = true;
	}
	return inFirst;
}

bool SyntaxTreeGenerator::isRelationOp()
{
	string str = tokenStr(index_);
	if (str == "<" || str == ">" || str == "=" || str == "!=" || str == "<=" || str == ">=") {
		return true;
	} else {
		return false;
	}
}

bool SyntaxTreeGenerator::isAddOp()
{
	string str = tokenStr(index_);
	if (str == "+" || str == "-" || str == "or" || str == "xor") {
		return true;
	} else {
		return false;
	}
}

bool SyntaxTreeGenerator::isMulOp()
{
	string str = tokenStr(index_);
	if (str == "*" || str == "/" || str == "mod" || str == "div" || str == "and") {
		return true;
	} else {
		return false;
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getLeftOver(string str)
{
	if (tokenStr(index_) == str) {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		cout << str << endl;
		error("getLeftOver");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getId()
{
	if (tokenType(index_) == "ID") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getId");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getInt()
{
	if (tokenType(index_) == "INT") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getInt");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}


ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getFloat()
{
	if (tokenType(index_) == "FLOAT") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getFLOAT");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getBoolean()
{
	if (tokenType(index_) == "TRUE" || tokenType(index_) == "FALSE") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getBoolean");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getOp(string opStr)
{
	if (tokenStr(index_) == opStr) {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getOp");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getLBracket()
{
	if (tokenType(index_) == "LBRACKET") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getLBracket");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getRBracket()
{
	if (tokenType(index_) == "RBRACKET") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getRBracket");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}

ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getSemicolon()
{
	if (tokenType(index_) == "SEMICOLON") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getSemicolon");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}
ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getColon()
{
	if (tokenType(index_) == "COLON") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getColon");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}
ZhjSmartPointer<SyntaxNode> SyntaxTreeGenerator::getComma()
{
	if (tokenType(index_) == "COMMA") {
		ZhjSmartPointer<SyntaxNode> node(new TerminalNode(tokenInfo(index_)));
		index_++;
		return node;
	} else {
		error("getComma");
		return ZhjSmartPointer<SyntaxNode>(NULL);
	}
}
