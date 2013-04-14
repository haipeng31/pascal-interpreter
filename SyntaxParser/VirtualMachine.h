#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "RunStackItem.h"
#include "RunSymbolTable.h"
#include "Stack.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace zhjcompiler {
class VirtualMachine {
public:
	VirtualMachine(const vector<string> &codes, size_t startIndex);

	void runCodes();
private:
	void runCode();
	void printResult();

	void runParameters();
	void runDeclaration();
	void runFuncReturn();
	void runProcReturn();
	void runFuncCall();
	void runJmpCond(bool);
	void runGoto();
	void runPush();
	void runAssign();
	void runNot();
	void runNegative();
	void runNumbericOp();
	void runLogicalOp();
	void runRelationalOp();

	Stack<RunStackItem> runStack_;
	RunSymbolTableStack symbolTableStack_;
	vector<string> codes_;
	size_t codeIndex_;
	size_t nextIndex_;
};	// end of class

}	// end of namespace
#endif
