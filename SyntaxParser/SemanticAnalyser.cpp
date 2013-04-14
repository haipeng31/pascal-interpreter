#include "SemanticAnalyser.h"
#include <cassert>
#include <iostream>

using std::cout;
using std::endl;
using namespace zhjcompiler;

SemanticAnalyser::SemanticAnalyser()
 :	symbolStack_(),
	inFunc_(0),
	loopLevel_(0)
{
}

Type SemanticAnalyser::checkFuncList(ZhjSmartPointer<SyntaxNode> funcList)
{
	assert(funcList->type() == SyntaxNode::FUNCTIONLIST);
	ZhjSmartPointer<NonTerminalNode> node = funcList;
	bool hasMain = false;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > funcs = node->childs();
	symbolStack_.pushEmptySymbolTable();
	cout << "there are " << funcs.size() << " functions" << endl;
	for (size_t i = 0; i < funcs.size(); i++) {
		string funcName;
		cout << "stack size is " << symbolStack_.size() << endl;
		Type funcType = checkFunc(funcs[i], funcName);
		cout << "stack size is " << symbolStack_.size() << endl;
		cout << "finish checking function " << funcName << endl;
		if (funcType.isError()) {
			correct = false;
			cout << "errors in function " << funcName << endl;
		} else {
			Type temp;
			bool exist = symbolStack_.searchSymbol(funcName, temp);
			if (!exist) {
				symbolStack_.addSymbol(funcName, funcType);
				cout << "add new func " << funcName << endl;
				assert(symbolStack_.searchSymbol(funcName, temp) == true);
				if (funcName == "main") {
					hasMain = true;
				}
			} else {
				correct = false;
				cout << "dulplicated definition of function " << funcName << endl;
			}
		}
	}
	
	if (!hasMain) {
		cout << "doesn't has main function" << endl;
	}
	if (hasMain && correct) {
		return Type();
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkFunc(ZhjSmartPointer<SyntaxNode> func, string &funcName)
{
	assert(func->type() == SyntaxNode::FUNCTION || func->type() == SyntaxNode::PROCEDURE);
	vector<Type::ConcreteType> paraTypes;
	Type::ConcreteType returnType;
	bool correct = true;

	ZhjSmartPointer<NonTerminalNode> node = func;
	
	symbolStack_.pushEmptySymbolTable();

	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	
	/* check it's function or procedure */
	terminalNode = childs[0];
	if (terminalNode->tokenStr() == "function") {
		inFunc_ = 1;
	} else if (terminalNode->tokenStr() == "procedure") {
		inFunc_ = 2;
	} else {
		inFunc_ = 0;
		// should not go here
		cout << "Error in checkFunc" << endl;
	}

	/* get function id */
	terminalNode = childs[1];
	funcName = terminalNode->tokenStr();

	/* get Paremeters */
	Type paraType = checkParameters(childs[2], paraTypes);
	if (paraType.isError()) {
		correct = false;
	}

	/* get return type */
	size_t index = 3;
	terminalNode = childs[3];
	if (inFunc_ == 1) {
		returnType = Type::strToType(terminalNode->tokenStr());
		funcReturnType_ = returnType;
		index++;
	} else {
		returnType = Type::strToType("void");
		funcReturnType_ = returnType;
	}

	/* get Declarations */
	Type declaType = checkDeclarations(childs[index]);
	if (declaType.isError()) {
		correct = false;
	}
	index++;

	/* get stmt */
	Type stmtType = checkStmt(childs[index]);
	if (stmtType.isError()) {
		correct = false;
	}
	
	symbolStack_.popSymbolTable();
	if (correct) {
		return Type(paraTypes, returnType);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	switch (node->type()) {
		case SyntaxNode::COMPOUNDSTMT:
			return checkCompoundStmt(stmt);
		case SyntaxNode::ASSIGNSTMT:
			return checkAssignStmt(stmt);
		case SyntaxNode::PROCEDURESTMT:
			return checkProcedureStmt(stmt);
		case SyntaxNode::IFSTMT:
			return checkIfStmt(stmt);
		case SyntaxNode::WHILESTMT:
			return checkWhileStmt(stmt);
		case SyntaxNode::DOWHILESTMT:
			return checkDoWhileStmt(stmt);
		case SyntaxNode::RETURNSTMT:
			return checkReturnStmt(stmt);
		default:
			cout << "error in checkStmt" << endl;
			break;
	}
	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkExp(ZhjSmartPointer<SyntaxNode> exp)
{
	assert(exp->type() == SyntaxNode::EXPRESSION);
	bool correct = true;
	ZhjSmartPointer<NonTerminalNode> expNode = exp;
	vector<ZhjSmartPointer<SyntaxNode> > childs = expNode->childs();
	string opStr;

	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	Type leftType, rightType;
	leftType = checkSimpleExp(childs[0]);

	if (childs.size() > 2) {
		terminalNode = childs[1];
		opStr = terminalNode->tokenStr();
		rightType = checkSimpleExp(childs[2]);
	}

	if (childs.size() == 1) {
		if (leftType.isError()) {
			correct = false;
			return Type(Type::ERRORTYPE);
		} else {
			return leftType;
		}
	} else if (childs.size() == 3) {
		if (leftType.isError() || rightType.isError()) {
			correct = false;
			return Type(Type::ERRORTYPE);
		} else {
			Type rtType =  checkRelationOperands(leftType, rightType, opStr);
			if (rtType.isError()) {
				cout << "Relation Op, left and right operand type not suitable" << endl;
			}
			return rtType;
		}
	}

	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkParameters(ZhjSmartPointer<SyntaxNode> paras, vector<Type::ConcreteType> &paraTypes)
{
	assert(paras->type() == SyntaxNode::ARGUMENTS);
	bool correct = true;
	ZhjSmartPointer<NonTerminalNode> parasNode = paras;
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	vector<ZhjSmartPointer<SyntaxNode> > childs = parasNode->childs();

	for (size_t i = 0; i < childs.size(); i++) {
		nonTerminalNode = childs[i];
		vector<ZhjSmartPointer<SyntaxNode> > paraEles = nonTerminalNode->childs();
		ZhjSmartPointer<NonTerminalNode> paraNonTerminalNode = paraEles[0];
		ZhjSmartPointer<TerminalNode> paraTerminalNode = paraEles[1];
		vector<ZhjSmartPointer<SyntaxNode> > ids = paraNonTerminalNode->childs();

		Type::ConcreteType type = Type::strToType(paraTerminalNode->tokenStr());
		for (size_t j = 0; j < ids.size(); j++) {
			ZhjSmartPointer<TerminalNode> idTerminalNode = ids[j];
			string idStr = idTerminalNode->tokenStr();
			Type idType(type);
			if (symbolStack_.searchSymbol(idStr, idType)) {
				cout << "parameter " << idStr << " has been declared" << endl;
				correct = false;
			} else {
				symbolStack_.addSymbol(idStr, idType);
				paraTypes.push_back(type);
			}
		}
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkDeclarations(ZhjSmartPointer<SyntaxNode> declas)
{
	assert(declas->type() == SyntaxNode::DECLARATIONS);
	bool correct = true;
	ZhjSmartPointer<NonTerminalNode> declasNode = declas;
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	vector<ZhjSmartPointer<SyntaxNode> > childs = declasNode->childs();
	cout << "declarations size == " << childs.size() << endl;
	for (size_t i = 0; i < childs.size(); i++) {
		nonTerminalNode = childs[i];
		assert(nonTerminalNode->type() == SyntaxNode::DECLARATION);
		vector<ZhjSmartPointer<SyntaxNode> > declaEles = nonTerminalNode->childs();
		ZhjSmartPointer<NonTerminalNode> declaNonTerminalNode = declaEles[0];
		ZhjSmartPointer<TerminalNode> declaTerminalNode = declaEles[1];
		
		vector<ZhjSmartPointer<SyntaxNode> > ids = declaNonTerminalNode->childs();

		Type::ConcreteType type = Type::strToType(declaTerminalNode->tokenStr());
		for (size_t j = 0; j < ids.size(); j++) {
			ZhjSmartPointer<TerminalNode> idTerminalNode = ids[j];
			string idStr = idTerminalNode->tokenStr();
			//cout << idStr << endl;
			Type idType(type);
			if (symbolStack_.searchSymbol(idStr, idType)) {
				cout << "variable " << idStr << " has been declared" << endl;
				correct = false;
			} else {
				symbolStack_.addSymbol(idStr, idType);
			}
		}
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkCompoundStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::COMPOUNDSTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();

	for (size_t i = 0; i < childs.size(); i++) {
		Type stmtType = checkStmt(childs[i]);
		if (stmtType.isError()) {
			correct = false;
		}
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkAssignStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::ASSIGNSTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	//NonTerminalNode *nonTerminalnNode = NULL;
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	terminalNode = childs[0];
	string idStr = terminalNode->tokenStr();
	Type idType;
	if (symbolStack_.searchSymbol(idStr, idType)) {
		// do nothing
	} else {
		cout << "identifier " << idStr << " is undefined" << endl;
		correct = false;
	}
	
	Type expType = checkExp(childs[1]);
	if (expType.isError()) {
		correct = false;
	}
	if (idType.type() != expType.type()) {
		correct = false;
		cout << "left type is " << idType.type() << endl;
		cout << "right type is " << expType.type() << endl;
		cout << "left value and right value don't have same types" << idStr << endl;
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkProcedureStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::PROCEDURESTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	terminalNode = childs[0];
	string idStr = terminalNode->tokenStr();
	Type idType;
	if (symbolStack_.searchGlobalSymbol(idStr, idType)) {
		if (!idType.isFunction()) {
			correct = false;
			cout << idStr << " is not a procedure" << endl;
		}
	} else {
		correct = false;
		cout << "Procedure " << idStr << " hasn't been defined" << endl;
	}
	
	vector<Type::ConcreteType> paraTypes;
	vector<ZhjSmartPointer<SyntaxNode> > expChilds;
	nonTerminalNode = childs[1];
	expChilds = nonTerminalNode->childs();
	for (size_t i = 0; i < expChilds.size(); i++) {
		Type expType = checkExp(expChilds[i]);
		if (expType.isError()) {
			correct = false;
		}
		paraTypes.push_back(expType.type());
	}

	if (paraTypes != idType.paraTypes()) {
		correct = false;
		cout << "argument types not fit for procedure paramenter types" << endl;
	}
	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkIfStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::IFSTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);

	Type expType = checkExp(childs[0]);
	if (expType.type() != Type::BOOLEAN) {
		correct = false;
		cout << "Exp in If stmt doesn't return Boolean Type" << endl;
	}

	Type stmtType = checkStmt(childs[1]);
	if (stmtType.isError()) {
		correct = false;
	}

	if (childs.size() > 2) {
		Type stmtType = checkStmt(childs[2]);
		if (stmtType.isError()) {
			correct = false;
		}
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkWhileStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::WHILESTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	loopLevel_++;
	Type expType = checkExp(childs[0]);
	if (expType.type() != Type::BOOLEAN) {
		correct = false;
		cout << "Exp in While stmt doesn't return Boolean Type" << endl;
	}

	Type stmtType = checkStmt(childs[1]);
	if (stmtType.isError()) {
		correct = false;
	}
	
	loopLevel_--;
	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkDoWhileStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::DOWHILESTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	loopLevel_++;
	Type stmtType = checkStmt(childs[0]);
	if (stmtType.isError()) {
		correct = false;
	}

	Type expType = checkExp(childs[1]);
	if (expType.type() != Type::BOOLEAN) {
		correct = false;
		cout << "Exp in Do While stmt doesn't return Boolean Type" << endl;
	}
	
	loopLevel_--;
	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkReturnStmt(ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt->type() == SyntaxNode::RETURNSTMT);
	ZhjSmartPointer<NonTerminalNode> node = stmt;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	if (childs.size() == 1) {
		if (inFunc_ == 2) {
			correct = false;
			cout << "procedure should not return something" << endl;
		} else {
			Type expExp = checkExp(childs[0]);
			if (expExp.type() != funcReturnType_) {
				correct = false;
				cout << "function return type is not same with the exp" << endl;
			}
		}
	} else if (childs.size() == 0) {
		if (inFunc_ == 1) {
			correct = false;
			cout << "function should return something" << endl;
		}
	}

	if (correct) {
		return Type(Type::VOID);
	} else {
		return Type(Type::ERRORTYPE);
	}
}

Type SemanticAnalyser::checkSimpleExp(ZhjSmartPointer<SyntaxNode> simExp)
{
	assert(simExp->type() == SyntaxNode::SIMPLEEXPRESSION);
	bool correct = true;
	ZhjSmartPointer<NonTerminalNode> simpleExpNode = simExp;
	vector<ZhjSmartPointer<SyntaxNode> > childs = simpleExpNode->childs();

	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	size_t index = 0;
	if (childs[0]->type() == SyntaxNode::TERMINAL) {
		index = 1;
	} else {
		index = 0;
	}

	Type rtType = checkTerm(childs[index]);
	if (index == 1) {
		if (rtType.type() != Type::INTEGER && rtType.type() != Type::REAL) {
			rtType = Type(Type::ERRORTYPE);
		}
	}
	if (rtType.isError()) {
		correct = false;
	}
	index++;
	while (index < childs.size()) {
		terminalNode = childs[index];
		string opStr = terminalNode->tokenStr();
		index++;
		Type termType = checkTerm(childs[index]);
		if (rtType.isError() || termType.isError()) {
			correct = false;
		} else {
			rtType = checkAddOperands(rtType, termType, opStr);
			if (rtType.isError()) {
				cout << "add Ops type error, left right not suitable types" << endl;
				correct = false;
			}
		}
		index++;
	}

	return rtType;
}

Type SemanticAnalyser::checkTerm(ZhjSmartPointer<SyntaxNode> term)
{
	assert(term->type() == SyntaxNode::TERM);
	bool correct = true;
	ZhjSmartPointer<NonTerminalNode> termNode = term;
	vector<ZhjSmartPointer<SyntaxNode> > childs = termNode->childs();

	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	size_t index = 0;
	Type rtType = checkFactor(childs[index]);
	if (rtType.isError()) {
		correct = false;
	}
	index++;

	while (index < childs.size()) {
		terminalNode = childs[index];
		string opStr = terminalNode->tokenStr();
		index++;
		Type factorType = checkFactor(childs[index]);
		if (rtType.isError() || factorType.isError()) {
			correct = false;
		} else {
			rtType = checkMulOperands(rtType, factorType, opStr);
			if (rtType.isError()) {
				cout << "mul Ops type error, left right not suitable types" << endl;
				correct = false;
			}
		}
		index++;
	}

	return rtType;
}

Type SemanticAnalyser::checkFactor(ZhjSmartPointer<SyntaxNode> factor)
{
	assert(factor->type() == SyntaxNode::FACTOR);
	//bool correct = true;
	ZhjSmartPointer<NonTerminalNode> factorNode = factor;
	vector<ZhjSmartPointer<SyntaxNode> > childs = factorNode->childs();
	
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	if (childs.size() == 1) {
		if (childs[0]->type() == SyntaxNode::TERMINAL) {
			terminalNode = childs[0];
			string tokenType = terminalNode->tokenType();
			string tokenStr = terminalNode->tokenStr();

			if (tokenType == "ID") {
				Type rtType;
				if (symbolStack_.searchSymbol(tokenStr, rtType) == true) {
					return rtType;
				} else {
					return Type(Type::ERRORTYPE);
				}
			} else if (tokenType == "INT") {
				return Type(Type::INTEGER);
			} else if (tokenType == "FLOAT")	{
				return Type(Type::REAL);
			} else if (tokenType == "TRUE" || tokenType == "FALSE") {
				return Type(Type::BOOLEAN);
			} 
		} else if (childs[0]->type() == SyntaxNode::FACTOR) {
			return checkFactor(childs[0]);
		} else if (childs[0]->type() == SyntaxNode::EXPRESSION) {
				return checkExp(childs[0]);
		}
	} else if (childs.size() == 2) {
		return checkFuncFactor(factor);	
	}

	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkRelationOperands(Type leftType, Type rightType, string opStr)
{
	if (opStr == "=" || opStr == "<>") {
		if (leftType.type() == Type::BOOLEAN && rightType.type() == Type::BOOLEAN) {
			return Type(Type::BOOLEAN);
		}
	}
	
	if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) &&
				(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
		return Type(Type::BOOLEAN);
	}

	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkAddOperands(Type leftType, Type rightType, string opStr)
{
	if (opStr == "or" || opStr == "xor") {
		if (leftType.type() == Type::BOOLEAN && rightType.type() == Type::BOOLEAN) {
			return Type(Type::BOOLEAN);
		}
		if (leftType.type() == Type::INTEGER && rightType.type() == Type::INTEGER) {
			return Type(Type::INTEGER);
		} else if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) && 
					(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
			return Type(Type::REAL);
		}
	} else {
		if (leftType.type() == Type::INTEGER && rightType.type() == Type::INTEGER) {
			return Type(Type::INTEGER);
		} else if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) && 
					(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
			return Type(Type::REAL);
		}
	}
	
	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkMulOperands(Type leftType, Type rightType, string opStr)
{
	if (opStr == "div" || opStr == "mod") {
		if (leftType.type() == Type::INTEGER && rightType.type() == Type::INTEGER) {
			return Type(Type::INTEGER);
		}
	} else if (opStr == "and") {
		if (leftType.type() == Type::BOOLEAN && rightType.type() == Type::BOOLEAN) {
			return Type(Type::BOOLEAN);
		}
		if (leftType.type() == Type::INTEGER && rightType.type() == Type::INTEGER) {
			return Type(Type::INTEGER);
		} else if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) && 
					(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
			return Type(Type::REAL);
		}
	} else if (opStr == "/") {
		if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) && 
					(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
			return Type(Type::REAL);
		}
	} else {
		if (leftType.type() == Type::INTEGER && rightType.type() == Type::INTEGER) {
			return Type(Type::INTEGER);
		} else if ((leftType.type() == Type::INTEGER || leftType.type() == Type::REAL) && 
					(rightType.type() == Type::INTEGER || rightType.type() == Type::REAL)) {
			return Type(Type::REAL);
		}
	}
	
	return Type(Type::ERRORTYPE);
}

Type SemanticAnalyser::checkFuncFactor(ZhjSmartPointer<SyntaxNode> factor)
{
	ZhjSmartPointer<NonTerminalNode> node = factor;
	bool correct = true;
	vector<ZhjSmartPointer<SyntaxNode> > childs = node->childs();
	
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);

	terminalNode = childs[0];
	string idStr = terminalNode->tokenStr();
	Type idType;
	if (symbolStack_.searchGlobalSymbol(idStr, idType)) {
		if (!idType.isFunction()) {
			correct = false;
			cout << idStr << " is not a procedure" << endl;
		}
	} else {
		correct = false;
		cout << "Function " << idStr << " hasn't been defined" << endl;
	}
	
	vector<Type::ConcreteType> paraTypes;
	vector<ZhjSmartPointer<SyntaxNode> > expChilds;
	nonTerminalNode = childs[1];
	expChilds = nonTerminalNode->childs();
	for (size_t i = 0; i < expChilds.size(); i++) {
		Type expType = checkExp(expChilds[i]);
		if (expType.isError()) {
			correct = false;
		}
		paraTypes.push_back(expType.type());
	}

	if (paraTypes != idType.paraTypes()) {
		correct = false;
		cout << "argument types not fit for procedure paramenter types" << endl;
	}
	if (correct) {
		return idType.returnType();
	} else {
		return Type(Type::ERRORTYPE);
	}
}
