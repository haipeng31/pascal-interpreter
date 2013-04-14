#include "TokenScanner/RegularExp.h"
#include "TokenScanner/RegularExpLoader.h"
#include "TokenScanner/NFAModel.h"
#include "TokenScanner/NFAConverter.h"
#include "TokenScanner/DFAModel.h"
#include "TokenScanner/DFAConverter.h"
#include "TokenScanner/TokenScanner.h"
#include "SyntaxParser/SyntaxTree.h"
#include "SyntaxParser/SyntaxTreeGenerator.h"
#include "SyntaxParser/SymbolTable.h"
#include "SyntaxParser/SemanticAnalyser.h"
#include "SyntaxParser/CodeGenerator.h"
#include "SyntaxParser/VirtualMachine.h"

#include <iostream>
#include <vector>
#include <set>
#include <assert.h>
using namespace zhjcompiler;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::set;

typedef pair<string, string> Token;
typedef pair<Token, int> TokenInfo;
int main() {
	TokenScanner scanner("TokenScanner/TokenDef.in");
	vector<TokenInfo> tokenInfos = scanner.scanTokens("TokenScanner/code.temp");
	TokenInfo endInfo;
	endInfo.first.second = "$";
	endInfo.first.first = "EOF";
	endInfo.second = 0;
	tokenInfos.push_back(endInfo);

	cout << "finish Lexical Analysis" << endl;
	SyntaxTreeGenerator treeGenerator(tokenInfos);
	ZhjSmartPointer<SyntaxNode> node = treeGenerator.getFunctionList();
	assert(node);
	cout << "finish Syntax Analysis" << endl;

	SemanticAnalyser semanticAnalyser;
	Type rtType = semanticAnalyser.checkFuncList(node);
	assert(rtType.type() == Type::VOID);
	cout << "finish Semantic Analysis" << endl;

	CodeGenerator codeGenerator;
	codeGenerator.genCode(node);
	const vector<string> commands = codeGenerator.commands();

	for (size_t i = 0; i < commands.size(); i++) {
		cout << i << ": " << commands[i] << endl;
	}

	VirtualMachine machine(commands, codeGenerator.mainIndex());
	machine.runCodes();

	return 0;
}
