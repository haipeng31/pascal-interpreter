#include "CodeGenerator.h"
#include <iostream>
#include <sstream>
#include <cassert>
using std::cout;
using std::endl;
using std::stringstream;

using namespace zhjcompiler;

CodeGenerator::CodeGenerator()
 :	commands_(),
	funcCommandPos_(),
	function_(true),
	mainIndex_(0)
{
}

void CodeGenerator::genCode(const ZhjSmartPointer<SyntaxNode> syntaxTree)
{
	assert(syntaxTree);
	commands_.clear();
	transferFuncList(syntaxTree);

	stringstream ss;
	ss << "FuncCall " << funcCommandPos("main");
	mainIndex_ = commands_.size();
	commands_.push_back(ss.str());
	commands_.push_back("End");
}

const vector<string> &CodeGenerator::commands() const
{
	return commands_;
}

size_t CodeGenerator::mainIndex() const
{
	return mainIndex_;
}

void CodeGenerator::transferFuncList(const ZhjSmartPointer<SyntaxNode> funcList)
{
	assert(funcList);
	assert(funcList->type() == SyntaxNode::FUNCTIONLIST);
	
	ZhjSmartPointer<NonTerminalNode> funcListNode = funcList;
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = funcListNode->childs();

	for (size_t i = 0; i < childs.size(); i++) {
		nonTerminalNode = childs[i];
		ZhjSmartPointer<TerminalNode> terminalNode = nonTerminalNode->childs()[1];
		string funcName = terminalNode->tokenStr();
		
		size_t pos = commands_.size();

		transferFunc(nonTerminalNode);
		
		setFuncCommandPos(funcName, pos);
		
		cout << "finish transfer func " << funcName << endl;
	}
}

void CodeGenerator::transferFunc(const ZhjSmartPointer<SyntaxNode> func)
{
	assert(func);
	assert(func->type() == SyntaxNode::FUNCTION || func->type() == SyntaxNode::PROCEDURE);

	function_ = true;
	size_t index = 0;
	ZhjSmartPointer<NonTerminalNode> funcNode = func;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = funcNode->childs();
	
	terminalNode = childs[index];
	if (terminalNode->tokenStr() == "procedure") {
		cout << "I'm a procedure" << endl;
		function_ = false;
	}
	index++;

	index++;

	transferIds(childs[index], "Parameter");
	index++;
	
	cout << "finish transfer parameters" << endl;
	if (function_) {
		index++;
	}

	transferIds(childs[index], "Declaration");
	index++;
	
	cout << "finish transfer declarations" << endl;

	transferCompoundStmt(childs[index]);
	
	cout << "finish transfer function body" << endl;
	// deal with return statements;
/*
	stringstream ss;
	if (function) {
		ss << "FuncReturn " << commands_.size();
	} else {
		ss << "ProcReturn " << commands_.size();
	}

	cout << "ss.str() == " << ss.str();
	for (size_t i = 0; i < returnStmtIndexs_.size(); i++) {
		commands_[returnStmtIndexs_[i]] = ss.str();
	}
	returnStmtIndexs_.clear();
*/
}

void CodeGenerator::transferIds(const ZhjSmartPointer<SyntaxNode> idDeclas, string declaType)
{
	assert(idDeclas);
	assert(idDeclas->type() == SyntaxNode::ARGUMENTS || idDeclas->type() == SyntaxNode::DECLARATIONS);

	ZhjSmartPointer<NonTerminalNode> idDeclasNode = idDeclas;
	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = idDeclasNode->childs();
	
	cout << "idDeclas->type() == " << idDeclas->type() << endl;
	cout << "childs size " << childs.size() << endl;
	if (childs.size() == 0) {
		return;
	}
	for (size_t i = childs.size() - 1; i >= 0; i--) {
		nonTerminalNode = childs[i];
		ZhjSmartPointer<NonTerminalNode> idListNonTerminalNode = nonTerminalNode->childs()[0];
		ZhjSmartPointer<TerminalNode> typeTerminalNode = nonTerminalNode->childs()[1];
		vector<ZhjSmartPointer<SyntaxNode> > ids =  idListNonTerminalNode->childs();
		string typeStr = typeTerminalNode->tokenStr();
		
		cout << "ids size = " << ids.size() << endl;
		for (size_t j = ids.size() - 1; j >= 0; j--) {
			cout << "ids[j] type " << ids[j]->type() << endl;
			ZhjSmartPointer<TerminalNode> idTerminalNode = ids[j];
			string idStr = idTerminalNode->tokenStr();
			stringstream ss;
			ss << declaType << " " << idStr << " " << typeStr;
			commands_.push_back(ss.str());

			if (j == 0) {
				break;
			}
		}

		if (i == 0) {
			break;
		}
	}
}

void CodeGenerator::transferStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	
	switch(stmt->type()) {
		case SyntaxNode::COMPOUNDSTMT:
			transferCompoundStmt(stmt);
			break;

		case SyntaxNode::IFSTMT:
			transferIfStmt(stmt);
			break;

		case SyntaxNode::WHILESTMT:
			transferWhileStmt(stmt);
			break;

		case SyntaxNode::DOWHILESTMT:
			transferDoWhileStmt(stmt);
			break;

		case SyntaxNode::PROCEDURESTMT:
			transferProcedureStmt(stmt);
			break;

		case SyntaxNode::ASSIGNSTMT:
			transferAssignStmt(stmt);
			break;

		case SyntaxNode::RETURNSTMT:
			transferReturnStmt(stmt);
			break;

		default:
			break;
	}
}

void CodeGenerator::transferCompoundStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::COMPOUNDSTMT);

	ZhjSmartPointer<NonTerminalNode> compoundNode = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = compoundNode->childs();

	for (size_t i = 0; i < childs.size(); i++) {
		transferStmt(childs[i]);
	}
}

void CodeGenerator::transferIfStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::IFSTMT);

	ZhjSmartPointer<NonTerminalNode> ifStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = ifStmt->childs();
	size_t jmpIndex = 0, jmpDesIndex = 0, goToIndex = 0, goToDesIndex = 0;

	transferExp(childs[0]);
	jmpIndex = commands_.size();
	commands_.push_back("");

	transferStmt(childs[1]);
	if (childs.size() == 3) {
		goToIndex = commands_.size();
		commands_.push_back("");
	}
	
	jmpDesIndex = commands_.size();

	if (childs.size() == 3) {
		transferStmt(childs[2]);
	}

	goToDesIndex = commands_.size();

	stringstream ss;
	ss << "JmpFalse " << jmpDesIndex;
	commands_[jmpIndex] = ss.str();
	ss.str("");
	if (childs.size() == 3) {
		ss.str("");
		ss << "Goto " << goToDesIndex;
		commands_[goToIndex] = ss.str();
		ss.str("");
	}
}

void CodeGenerator::transferWhileStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::WHILESTMT);

	ZhjSmartPointer<NonTerminalNode> whileStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = whileStmt->childs();
	size_t expIndex = 0, jmpIndex = 0, jmpDesIndex = 0;
	stringstream ss;
	
	expIndex = commands_.size();
	transferExp(childs[0]);
	jmpIndex = commands_.size();
	commands_.push_back("");

	transferStmt(childs[1]);
	ss << "Goto " << expIndex;
	commands_.push_back(ss.str());
	jmpDesIndex = commands_.size();
	
	ss.str("");
	ss << "JmpFalse " << jmpDesIndex;
	commands_[jmpIndex] = ss.str();
}

void CodeGenerator::transferDoWhileStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::DOWHILESTMT);

	ZhjSmartPointer<NonTerminalNode> doWhileStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = doWhileStmt->childs();
	size_t jmpDesIndex = 0;
	
	jmpDesIndex = commands_.size();
	transferStmt(childs[0]);

	transferExp(childs[1]);

	stringstream ss;
	ss << "JmpTrue " <<	jmpDesIndex;
	commands_.push_back(ss.str());
}

void CodeGenerator::transferProcedureStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::PROCEDURESTMT);

	ZhjSmartPointer<NonTerminalNode> procedureStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = procedureStmt->childs();
	
	ZhjSmartPointer<NonTerminalNode> paraNonTerminalNode = childs[1];
	ZhjSmartPointer<TerminalNode> funcTerminalNode = childs[0];
	string funcName = funcTerminalNode->tokenStr();
	vector<ZhjSmartPointer<SyntaxNode> > exps = paraNonTerminalNode->childs();
	for (size_t i = 0; i < exps.size(); i++) {
		transferExp(exps[i]);
	}
	
	stringstream ss;
	ss << "FuncCall " << funcCommandPos(funcName);
	commands_.push_back(ss.str());
}

void CodeGenerator::transferAssignStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::ASSIGNSTMT);

	ZhjSmartPointer<NonTerminalNode> assignStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = assignStmt->childs();
	
	transferExp(childs[1]);
	ZhjSmartPointer<TerminalNode> idTerminalNode = childs[0];
	string idStr = idTerminalNode->tokenStr();
	stringstream ss;
	ss << "Assign " << idStr;
	commands_.push_back(ss.str());
}

void CodeGenerator::transferReturnStmt(const ZhjSmartPointer<SyntaxNode> stmt)
{
	assert(stmt);
	assert(stmt->type() == SyntaxNode::RETURNSTMT);

	ZhjSmartPointer<NonTerminalNode> returnStmt = stmt;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = returnStmt->childs();
	
	if (childs.size() == 1) {
		cout << "transfer exp in ReturnStmt" << endl;
		transferExp(childs[0]);
		cout << "finish exp in ReturnStmt" << endl;
	}	
	//returnStmtIndexs_.push_back(commands_.size());
	if (function_) {
		commands_.push_back("FuncReturn");
	} else {
		commands_.push_back("ProcReturn");
	}
}

void CodeGenerator::transferExp(const ZhjSmartPointer<SyntaxNode> exp)
{
	assert(exp);
	cout << "Exp type is " << exp->type() << endl;
	assert(exp->type() == SyntaxNode::EXPRESSION);

	ZhjSmartPointer<NonTerminalNode> expNode = exp;
//	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = expNode->childs();
	
	transferSimpleExp(childs[0]);
	if (childs.size() == 3) {
		transferSimpleExp(childs[2]);
		ZhjSmartPointer<TerminalNode> opTerminalNode = childs[1];
		commands_.push_back(opTerminalNode->tokenStr());
	}
}

void CodeGenerator::transferSimpleExp(const ZhjSmartPointer<SyntaxNode> simpleExp)
{
	assert(simpleExp);
	assert(simpleExp->type() == SyntaxNode::SIMPLEEXPRESSION);

	ZhjSmartPointer<NonTerminalNode> simpleExpNode = simpleExp;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = simpleExpNode->childs();
	size_t index = 0;
	string signStr = "";
	if (childs[index]->type() == SyntaxNode::TERMINAL) {
		ZhjSmartPointer<TerminalNode> signTerminalNode = childs[index];
		signStr = signTerminalNode->tokenStr(); 
		index++;
	}
	transferTerm(childs[index]);
	index++;
	
	if (signStr == "+") {
		commands_.push_back("Positive");
	} else if (signStr == "-") {
		commands_.push_back("Negative");
	}
	while (index < childs.size()) {
		transferTerm(childs[index+1]);
		ZhjSmartPointer<TerminalNode> opTerminalNode = childs[index];
		commands_.push_back(opTerminalNode->tokenStr());
		index += 2;
	}
}

void CodeGenerator::transferTerm(const ZhjSmartPointer<SyntaxNode> term)
{
	assert(term);
	cout << "Term type is " << term->type() << endl;
	if (term->type() == 21) {
		//cout << "tokenStr is " << (term)->tokenStr() << endl;
	}
	assert(term->type() == SyntaxNode::TERM);

	ZhjSmartPointer<NonTerminalNode> termNode = term;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	//ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = termNode->childs();
	size_t index = 0;

	transferFactor(childs[index]);
	index++;

	while (index < childs.size()) {
		transferFactor(childs[index+1]);
		ZhjSmartPointer<TerminalNode> opTerminalNode = childs[index];
		commands_.push_back(opTerminalNode->tokenStr());
		index += 2;
	}

}

void CodeGenerator::transferFactor(const ZhjSmartPointer<SyntaxNode> factor)
{
	assert(factor);
	assert(factor->type() == SyntaxNode::FACTOR);

	ZhjSmartPointer<NonTerminalNode> factorNode = factor;
	//ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
	ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = factorNode->childs();
	//size_t index = 0;
	stringstream ss;
	if (childs.size() == 1) {
		if (childs[0]->type() == SyntaxNode::TERMINAL) {
			terminalNode = childs[0];
			string tokenType = terminalNode->tokenType();
			string tokenStr = terminalNode->tokenStr();

			if (tokenType == "ID") {
				ss << "ID " << tokenStr;
				commands_.push_back(ss.str());
			} else if (tokenType == "INT") {
				ss << "INT " << tokenStr;
				commands_.push_back(ss.str());
			} else if (tokenType == "FLOAT") {
				ss << "FLOAT " << tokenStr;
				commands_.push_back(ss.str());
			} else if (tokenType == "TRUE" || tokenType == "FALSE") {
				if (tokenType == "TRUE") {
					ss << "BOOL " << true;
				} else {
					ss << "BOOL " << false;
				}
				commands_.push_back(ss.str());
			} 
		} else if (childs[0]->type() == SyntaxNode::FACTOR) {
			transferFactor(childs[0]);
			commands_.push_back("not");
		} else if (childs[0]->type() == SyntaxNode::EXPRESSION) {
			transferExp(childs[0]);
		}
	} else if (childs.size() == 2) {
		transferFuncCall(factor);
	}
}

void CodeGenerator::transferFuncCall(const ZhjSmartPointer<SyntaxNode> funcFactor)
{
	assert(funcFactor);
	assert(funcFactor->type() == SyntaxNode::FACTOR);

	ZhjSmartPointer<NonTerminalNode> funcFactorNode = funcFactor;
//	ZhjSmartPointer<NonTerminalNode> nonTerminalNode(NULL);
//	ZhjSmartPointer<TerminalNode> terminalNode(NULL);
	vector<ZhjSmartPointer<SyntaxNode> > childs = funcFactorNode->childs();
	
	ZhjSmartPointer<NonTerminalNode> paraNonTerminalNode = childs[1];
	ZhjSmartPointer<TerminalNode> funcTerminalNode = childs[0];
	string funcName = funcTerminalNode->tokenStr();
	vector<ZhjSmartPointer<SyntaxNode> > exps = paraNonTerminalNode->childs();
	for (size_t i = 0; i < exps.size(); i++) {
		transferExp(exps[i]);
	}
	
	stringstream ss;
	ss << "FuncCall " << funcCommandPos(funcName);
	commands_.push_back(ss.str());
}

size_t CodeGenerator::funcCommandPos(const string &funcName) const
{
	assert(funcExists(funcName));
	map<string, size_t>::const_iterator mit = funcCommandPos_.find(funcName);
	return mit->second;

	// return funcCommandPos_[funcName]; why wrong?
}

bool CodeGenerator::funcExists(const string &funcName) const
{
	map<string, size_t>::const_iterator mit = funcCommandPos_.find(funcName);
	if (mit == funcCommandPos_.end()) {
		return false;
	} else {
		return true;
	}
}

void CodeGenerator::setFuncCommandPos(const string &funcName, size_t pos)
{
	assert(!funcExists(funcName));
	funcCommandPos_.insert(pair<string, size_t>(funcName, pos));
}
