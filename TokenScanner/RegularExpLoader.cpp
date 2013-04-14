#include "RegularExpLoader.h"
#include <stack>
#include <iostream>
using std::stack;
using std::cout;
using std::endl;
using namespace zhjcompiler;

RegularExpLoader::RegularExpLoader()
{
}

RegularExpLoader::~RegularExpLoader()
{
}

RegularExp *RegularExpLoader::load(string regExpStr)
{
	return genExpTree(posixExp(regExpStr));
}

string RegularExpLoader::posixExp(string regExpStr)
{
	stack<char> helpStack;
	string posixStr = "";
	
	cout << regExpStr.size() << endl;
	if (regExpStr[0] == ' ') {
		cout << "space charactor" << endl;
	}
	regExpStr = addHelpSymbol(regExpStr);
	cout << "add string " << regExpStr << endl;
	helpStack.push('#');
	for (unsigned int i = 0; i < regExpStr.size(); i++) {
		if (regExpStr[i] == ' ' || regExpStr[i] == '\t' || regExpStr[i] == '\n') {
			cout << "space charactor" << endl;
		} else {
			cout << "c == " << regExpStr[i] << endl;
		}
		if (isOperation(regExpStr[i])) {
			while (priority(helpStack.top()) >= priority(regExpStr[i]) 
						&& helpStack.top() != '(' && helpStack.top() != '[') {
				char c = helpStack.top();
				helpStack.pop();
				posixStr.push_back(c);
			}
			
			if (regExpStr[i] == '\\') {
				posixStr.push_back(regExpStr[i]);
				posixStr.push_back(regExpStr[i+1]);
				i++;
				continue;
			}
			if (regExpStr[i] == ')' || regExpStr[i] == ']') {
				helpStack.pop();
			} else {
				helpStack.push(regExpStr[i]);
			}

		} else {
			posixStr.push_back(regExpStr[i]);
		}
	}
	while (helpStack.size() > 1) {
		char c = helpStack.top();
		helpStack.pop();
		posixStr.push_back(c);
	}
	return posixStr;
}

RegularExp *RegularExpLoader::genExpTree(string posixExp)
{
	stack<RegularExp *> helpStack;
	
	RegularExp *exp, *newExp, *start, *end, *lexp, *rexp;
	SymbolRegularExp *symbolExp;
	ConcatenationExp *concateExp;
	AlternationExp *alterExp;
	for (unsigned int i = 0; i < posixExp.size(); i++) {
		if (isOperation(posixExp[i])) {
			switch(posixExp[i]) {
				case '*':
					exp = helpStack.top();
					helpStack.pop();
					newExp = new RepeatationExp(exp, 0, RepeatationExp::MAX);
					helpStack.push(newExp);
					break;

				case '+':
					exp = helpStack.top();
					helpStack.pop();
					newExp = new RepeatationExp(exp, 1, RepeatationExp::MAX);
					helpStack.push(newExp);
					break;

				case '?':
					exp = helpStack.top();
					helpStack.pop();
					newExp = new RepeatationExp(exp, 0, 1);
					helpStack.push(newExp);
					break;
				
				case '-':
					end = helpStack.top();
					helpStack.pop();
					start = helpStack.top();
					helpStack.pop();
					newExp = new SymbolRegularExp(start, end);
					helpStack.push(newExp);
					break;

				case '^':
					symbolExp = (SymbolRegularExp *)helpStack.top();
					symbolExp->reverse();
					helpStack.push(exp);
					break;

				case '&':
					rexp = helpStack.top();
					helpStack.pop();
					lexp = helpStack.top();
					helpStack.pop();
					
					if (lexp->type() == RegularExp::CONCATENATION && rexp->type() == RegularExp::CONCATENATION) {
						concateExp = (ConcatenationExp *)lexp;
						concateExp->merge(rexp);
						helpStack.push(concateExp);
					} else if (lexp->type() == RegularExp::CONCATENATION && rexp->type() != RegularExp::CONCATENATION) {
						concateExp = (ConcatenationExp *)lexp;
						concateExp->addChildExp(rexp);
						helpStack.push(concateExp);
					} else if (lexp->type() != RegularExp::CONCATENATION && rexp->type() == RegularExp::CONCATENATION) {
						concateExp = new ConcatenationExp();
						concateExp->addChildExp(lexp);
						concateExp->merge(rexp);
						helpStack.push(concateExp);
					} else if (lexp->type() != RegularExp::CONCATENATION && rexp->type() != RegularExp::CONCATENATION) {
						concateExp = new ConcatenationExp();
						concateExp->addChildExp(lexp);
						concateExp->addChildExp(rexp);
						helpStack.push(concateExp);
					}
					break;

				case '|':
					rexp = helpStack.top();
					helpStack.pop();
					lexp = helpStack.top();
					helpStack.pop();
					
					if (lexp->type() == RegularExp::SYMBOL && rexp->type() == RegularExp::SYMBOL) {
						symbolExp = (SymbolRegularExp *)lexp;
						symbolExp->merge(rexp);
						helpStack.push(symbolExp);
					} else if (lexp->type() == RegularExp::ALTERNATION && rexp->type() == RegularExp::ALTERNATION) {
						alterExp = (AlternationExp *)lexp;
						alterExp->merge(rexp);
						helpStack.push(alterExp);
					} else if (lexp->type() == RegularExp::ALTERNATION && rexp->type() != RegularExp::ALTERNATION) {
						alterExp = (AlternationExp *)lexp;
						alterExp->addChildExp(rexp);
						helpStack.push(alterExp);
					} else if (lexp->type() != RegularExp::ALTERNATION && rexp->type() == RegularExp::ALTERNATION) {
						alterExp = new AlternationExp();
						alterExp->addChildExp(lexp);
						alterExp->merge(rexp);
						helpStack.push(alterExp);
					} else if (lexp->type() != RegularExp::ALTERNATION && rexp->type() != RegularExp::ALTERNATION) {
						alterExp = new AlternationExp();
						alterExp->addChildExp(lexp);
						alterExp->addChildExp(rexp);
						helpStack.push(alterExp);
					}

					break;
				
				case '\\':
					symbolExp = new SymbolRegularExp(posixExp[i+1]);
					helpStack.push(symbolExp);
					i++;
					break;
				default:
					break;
			}
		} else {
			symbolExp = new SymbolRegularExp(posixExp[i]);
			helpStack.push(symbolExp);
		}
	}
	return helpStack.top();
}

int RegularExpLoader::priority(char c)
{
	int nice = 0;
	switch(c) {
		case '\\':
			nice = 6;
			break;

		case '(':
		case '[':
			nice = 5;
			break;

		case ')':
		case ']':
			nice = 0;
			break;

		case '#':
			nice = 0;
			break;

		case '*':
		case '+':
		case '?':
			nice = 4;
			break;

		case '-':  // only exist in [], so the nice value is not quite important
		case '&':  // represent concatenation
			nice = 3;
			break;

		case '|':
			nice = 2;
			break;

		case '^':
			nice = 1;
			break;

		default:
			break;
	}

	return nice;
}

bool RegularExpLoader::isOperation(char c)
{
	bool rt = false;

	switch(c) {
		case '(':
		case ')':
		case '[':
		case ']':
		case '+':
		case '*':
		case '?':
		case '&':
		case '^':
		case '-':
		case '|':
		case '\\':
			rt = true;
			break;

		default:
			break;
	}

	return rt;
}

/* add & and | if necessary */
string RegularExpLoader::addHelpSymbol(string regExpStr)
{
	bool in = false;       // in []
	unsigned cnt = 0;      // count of '\\'
	string resultStr;

	resultStr.push_back(regExpStr[0]);
	if (regExpStr[0] == '\\') {
		cnt++;
	}
	if (regExpStr[0] == '[') {
		in = true;
	}
	for (unsigned int i = 1; i < regExpStr.size(); i++) {
		if (regExpStr[i] != '*' && regExpStr[i] != '+' && regExpStr[i] != '?' &&
					regExpStr[i] != ')' && regExpStr[i] != ']' && regExpStr[i] != '|' && ((regExpStr[i-1] != '(' &&
					regExpStr[i-1] != '[' && regExpStr[i-1] != '|' && regExpStr[i-1] != '\\') || (i-2 >= 0 && regExpStr[i-2] == '\\' && cnt % 2 == 0)) && !in) {
			resultStr.push_back('&');
		}
		if (in && ((!isOperation(regExpStr[i-1]) || (i-2>= 0 && regExpStr[i-2] == '\\')) && (!isOperation(regExpStr[i]) || regExpStr[i] == '\\') )) {
			resultStr.push_back('|');
		}
	
		if (regExpStr[i] == '[') {
			in = true;
		} else if (regExpStr[i] == ']') {
			in = false;
		}


		if (regExpStr[i] == '\\' || regExpStr[i-1] == '\\') {
			cnt++;
		}
		resultStr.push_back(regExpStr[i]);
	}

	return resultStr;
}

