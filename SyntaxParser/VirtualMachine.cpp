#include "VirtualMachine.h"
#include <cassert>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;
using namespace zhjcompiler;

VirtualMachine::VirtualMachine(const vector<string> &codes, size_t startIndex)
 :	runStack_(),
	symbolTableStack_(),
	codes_(codes),
	codeIndex_(startIndex),
	nextIndex_(startIndex + 1)
{
}

void VirtualMachine::runCodes()
{
	while (1) {	
		if (codes_[codeIndex_] == "End") {
			printResult();
			break;
		} else {
			cout << "Run Code : " << codes_[codeIndex_] << endl;
			runCode();
		}
		codeIndex_ = nextIndex_;
		nextIndex_++;
	}
}

void VirtualMachine::printResult()
{
	RunStackItem item = runStack_.top();

	if (item.type() == RunStackItem::INT) {
		cout << item.intValue() << endl;
	} else if (item.type() == RunStackItem::FLOAT) {
		cout << item.floatValue() << endl;
	} else if (item.type() == RunStackItem::BOOL) {
		cout << item.boolValue() << endl;
	}
}

void VirtualMachine::runCode()
{
	stringstream ss(codes_[codeIndex_]);
	string op;

	ss >> op;

	if (op == "Parameter") {
		runParameters();
	} else if (op == "Declaration") {
		runDeclaration();
	} else if (op == "FuncReturn") {
		runFuncReturn();
	} else if (op == "ProcReturn") {
		runProcReturn();
	} else if (op == "FuncCall") {
		runFuncCall();
	} else if (op == "JmpTrue") {
		runJmpCond(true);
	} else if (op == "JmpFalse") {
		runJmpCond(false);
	} else if (op == "Goto") {
		runGoto();
	} else if (op == "ID") {
		runPush();
	} else if (op == "INT") {
		runPush();
	} else if (op == "FLOAT") {
		runPush();
	} else if (op == "BOOL") {
		runPush();
	} else if (op == "Assign") {
		runAssign();
	} else if (op == "Not") {
		runNot();
	} else if (op == "Negative") {
		runNegative();
	} else if (op == "Positive") {
		// do nothing
	} else if (op == "+") {
		runNumbericOp();
	} else if (op == "-") {
		runNumbericOp();
	} else if (op == "*") {
		runNumbericOp();
	} else if (op == "/") {
		runNumbericOp();
	} else if (op == "mod") {
		runNumbericOp();
	} else if (op == "div") {
		runNumbericOp();
	} else if (op == "and") {
		runLogicalOp();
	} else if (op == "or") {
		runLogicalOp();
	} else if (op == "xor") {
		runLogicalOp();
	} else if (op == "<") {
		runRelationalOp();
	} else if (op == ">") {
		runRelationalOp();
	} else if (op == "=") {
		runRelationalOp();
	} else if (op == "!=") {
		runRelationalOp();
	} else if (op == "<=") {
		runRelationalOp();
	} else if (op == ">=") {
		runRelationalOp();
	} else {
		cout << "Undefined struction" << endl;
	
	}
}

void VirtualMachine::runParameters()
{
	vector<RunStackItem> helpItemVec;
	vector<string> symbolVec;
	while (1) {
		stringstream ss(codes_[codeIndex_]);
		string op, symbol, type;

		ss >> op >> symbol >> type;
		if (op != "Parameter") {
			break;
		}
		RunStackItem item = runStack_.top();
		runStack_.pop();
		helpItemVec.push_back(item);
		symbolVec.push_back(symbol);
		codeIndex_++;
		nextIndex_++;
	}
	
	for (size_t i = 0; i < helpItemVec.size(); i++) {
		symbolTableStack_.addSymbol(symbolVec[i], runStack_.size());
		runStack_.push(helpItemVec[i]);
	}
	codeIndex_--;
	nextIndex_--;
}

void VirtualMachine::runDeclaration()
{
	stringstream ss(codes_[codeIndex_]);
	string op, symbol, type;

	ss >> op >> symbol >> type;

	symbolTableStack_.addSymbol(symbol, runStack_.size());
	if (type == "integer") {
		runStack_.push(RunStackItem(RunStackItem::INT, 0));
	} else if (type == "real") {
		float initValue = 0.0;
		runStack_.push(RunStackItem(RunStackItem::FLOAT, initValue));
	} else if (type == "boolean") {
		runStack_.push(RunStackItem(RunStackItem::BOOL, true));
	}
}

void VirtualMachine::runFuncReturn()
{
	nextIndex_ = symbolTableStack_.returnAddr();
	size_t symbolSize = symbolTableStack_.symbolSize();

	RunStackItem returnItem = runStack_.top();
	runStack_.pop();

	for (size_t i = 0; i < symbolSize; i++) {
		runStack_.pop();
	}

	symbolTableStack_.popSymbolTable();
	runStack_.push(returnItem);
}

void VirtualMachine::runProcReturn()
{
	nextIndex_ = symbolTableStack_.returnAddr();
	size_t symbolSize = symbolTableStack_.symbolSize();

	for (size_t i = 0; i < symbolSize; i++) {
		runStack_.pop();
	}

	symbolTableStack_.popSymbolTable();
}

void VirtualMachine::runFuncCall()
{
	stringstream ss(codes_[codeIndex_]);
	string op;
	size_t index, savedNextIndex;

	ss >> op >> index;
	savedNextIndex = nextIndex_;
	nextIndex_ = index;

	symbolTableStack_.pushSymbolTable(savedNextIndex);
}

void VirtualMachine::runJmpCond(bool cond)
{
	stringstream ss(codes_[codeIndex_]);
	string op;
	size_t jmpIndex;

	ss >> op >> jmpIndex;

	RunStackItem item = runStack_.top();
	runStack_.pop();

	assert(item.type() == RunStackItem::BOOL);
	if (item.boolValue() == cond) {
		nextIndex_ = jmpIndex;
	}
}

void VirtualMachine::runGoto()
{
	stringstream ss(codes_[codeIndex_]);
	string op;
	size_t gotoIndex;
	
	ss >> op >> gotoIndex;
	
	nextIndex_= gotoIndex;
}

void VirtualMachine::runPush()
{
	stringstream ss(codes_[codeIndex_]);
	string valueType;

	ss >> valueType;

	if (valueType == "ID") {
		string id;
		ss >> id;
		
		assert(symbolTableStack_.symbolExists(id) == true);
		size_t symbolAddr = symbolTableStack_.symbolAddr(id);
		RunStackItem item = runStack_[symbolAddr];
		runStack_.push(item);
	} else if (valueType == "INT") {
		int intValue;
		ss >> intValue;
		runStack_.push(RunStackItem(RunStackItem::INT, intValue));
	} else if (valueType == "FLOAT") {
		float floatValue;
		ss >> floatValue;
		runStack_.push(RunStackItem(RunStackItem::FLOAT, floatValue));
	} else if (valueType == "BOOL") {
		bool boolValue;
		ss >> boolValue;
		runStack_.push(RunStackItem(RunStackItem::BOOL, boolValue));
	}
}

void VirtualMachine::runAssign()
{
	stringstream ss(codes_[codeIndex_]);
	string op, id;

	ss >> op >> id;
	assert(symbolTableStack_.symbolExists(id) == true);
	size_t symbolAddr = symbolTableStack_.symbolAddr(id);
	runStack_[symbolAddr] = runStack_.top();
	runStack_.pop();
}

void VirtualMachine::runNot()
{
	RunStackItem item = runStack_.top();
	runStack_.pop();
	
	if (item.type() == RunStackItem::INT) {
		int value = item.intValue();
		value = !value;
		runStack_.push(RunStackItem(RunStackItem::INT, value));
	} else if (item.type() == RunStackItem::BOOL) {
		bool value = item.boolValue();
		value = !value;
		runStack_.push(RunStackItem(RunStackItem::BOOL, value));
	} else {
		cout << "error in runNot" << endl;
	}
}

void VirtualMachine::runNegative()
{
	RunStackItem item = runStack_.top();
	runStack_.pop();
	
	if (item.type() == RunStackItem::INT) {
		int value = item.intValue();
		value = -value;
		runStack_.push(RunStackItem(RunStackItem::INT, value));
	} else if (item.type() == RunStackItem::FLOAT) {
		float value = item.floatValue();
		value = !value;
		runStack_.push(RunStackItem(RunStackItem::FLOAT, value));
	} else {
		cout << "error in runNot" << endl;
	}
}

void VirtualMachine::runNumbericOp()
{
	string op = codes_[codeIndex_];

	RunStackItem rItem = runStack_.top();
	runStack_.pop();
	RunStackItem lItem = runStack_.top();
	runStack_.pop();
	
	if (lItem.type() == RunStackItem::INT) {
		int lValue = lItem.intValue();
		int rValue = rItem.intValue();

		if (op == "+") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue + rValue));
		} else if (op == "-") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue - rValue));
		} else if (op == "*") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue * rValue));
		} else if (op == "/") {
			runStack_.push(RunStackItem(RunStackItem::FLOAT, (float)(lValue / rValue)));
		} else if (op == "div") {
runStack_.push(RunStackItem(RunStackItem::INT, lValue / rValue));
		} else if (op == "mod") {
runStack_.push(RunStackItem(RunStackItem::INT, lValue % rValue));
		}
	} else if (lItem.type() == RunStackItem::FLOAT) {
		float lValue = lItem.floatValue();
		float rValue = rItem.floatValue();

		if (op == "+") {
			runStack_.push(RunStackItem(RunStackItem::FLOAT, lValue + rValue));
		} else if (op == "-") {
			runStack_.push(RunStackItem(RunStackItem::FLOAT, lValue - rValue));
		} else if (op == "*") {
			runStack_.push(RunStackItem(RunStackItem::FLOAT, lValue * rValue));
		} else if (op == "/") {
			runStack_.push(RunStackItem(RunStackItem::FLOAT, lValue / rValue));
		} else if (op == "div" || op == "mod") {
			cout << "error in runNumbericOp" << endl;
		} 

	} else if (lItem.type() == RunStackItem::BOOL) {
		cout << "error in Numberic Op" << endl;
	}
}

void VirtualMachine::runLogicalOp()
{
	string op = codes_[codeIndex_];

	RunStackItem rItem = runStack_.top();
	runStack_.pop();
	RunStackItem lItem = runStack_.top();
	runStack_.pop();
	
	if (lItem.type() == RunStackItem::INT) {
		int lValue = lItem.intValue();
		int rValue = rItem.intValue();

		if (op == "and") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue & rValue));
		} else if (op == "or") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue | rValue));
		} else if (op == "xor") {
			runStack_.push(RunStackItem(RunStackItem::INT, lValue ^ rValue));
		}
	} else if (lItem.type() == RunStackItem::FLOAT) {
		cout << "error in Logical Op" << endl; 
	} else if (lItem.type() == RunStackItem::BOOL) {
		bool lValue = lItem.boolValue();
		bool rValue = rItem.boolValue();

		if (op == "and") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue && rValue));
		} else if (op == "or") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue || rValue));
		} else if (op == "xor") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue != rValue));
		}
	}
}

void VirtualMachine::runRelationalOp()
{
	string op = codes_[codeIndex_];

	RunStackItem rItem = runStack_.top();
	runStack_.pop();
	RunStackItem lItem = runStack_.top();
	runStack_.pop();
	
	if (lItem.type() == RunStackItem::INT) {
		int lValue = lItem.intValue();
		int rValue = rItem.intValue();

		if (op == ">") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue > rValue));
		} else if (op == "=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue == rValue));
		} else if (op == "<") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue < rValue));
		} else 	if (op == ">=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue >= rValue));
		} else if (op == "<=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue <= rValue));
		} else if (op == "!=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue != rValue));
		}
 	} else if (lItem.type() == RunStackItem::FLOAT) {
		float lValue = lItem.floatValue();
		float rValue = rItem.floatValue();
		
		cout << "lValue = " << lValue << ", rValue = " << rValue << endl;
		if (op == ">") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue > rValue));
		} else if (op == "=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue == rValue));
		} else if (op == "<") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue < rValue));
		} else 	if (op == ">=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue >= rValue));
		} else if (op == "<=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue <= rValue));
		} else if (op == "!=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue != rValue));
		} 
	} else if (lItem.type() == RunStackItem::BOOL) {
		bool lValue = lItem.boolValue();
		bool rValue = rItem.boolValue();
		
		if (op == "=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue == rValue));
		} else if (op < "!=") {
			runStack_.push(RunStackItem(RunStackItem::BOOL, lValue != rValue));
		} else {
			cout << "error in runRelationalOp" << endl;
		}
	}
}

