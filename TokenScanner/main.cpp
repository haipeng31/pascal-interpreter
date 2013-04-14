#include "RegularExp.h"
#include "RegularExpLoader.h"
#include "NFAModel.h"
#include "NFAConverter.h"
#include "DFAModel.h"
#include "DFAConverter.h"
#include "TokenScanner.h"
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
void traverse(RegularExp *exp) 
{
	switch(exp->type()) {
		case RegularExp::CONCATENATION:
			{
			cout << "Concatenation node ";
			ConcatenationExp *concateExp = (ConcatenationExp *)exp;
			vector<RegularExp *> childs = concateExp->childExps();
			cout << "Has " << childs.size() << " child exps" << endl;
			for (unsigned int i = 0; i < childs.size(); i++) {
				traverse(childs[i]);
			}
			}
			break;

		case RegularExp::SYMBOL:
			{
			cout << "Symbom node ";
			SymbolRegularExp *symbolExp = (SymbolRegularExp *)exp;
			vector<bool> charSet = symbolExp->charSet();
			int cnt = 0;
			for (unsigned int i = 0; i < charSet.size(); i++) {
				if (charSet[i] == true) {
					cnt++;
				}
			}
			cout << "has " << cnt << " char." << endl;
			}
			break;
		
		case RegularExp::ALTERNATION:
			{
			AlternationExp *alterExp = (AlternationExp *)exp;
			vector<RegularExp *> childExps = alterExp->childExps();
			cout << "Alternation Node has " << childExps.size() << endl;
			for (unsigned int i = 0; i < childExps.size(); i++) {
				traverse(childExps[i]);
			}
			}
			break;

		case RegularExp::REPEATATION:
			{
			RepeatationExp *repeatExp = (RepeatationExp *)exp;
			cout << "Repeatation Node, min = " << repeatExp->min() << " max = " << repeatExp->max() << endl;
			
			traverse(repeatExp->exp());
			}
			break;
		default:
			break;

	}
}

set<NFANode *> nodes;
void readNFA(NFANode *node)
{
	nodes.insert(node);
	vector<NFAEdge *> outEdges = node->outEdges();
	cout << "Node " << (int *)node << " has " << outEdges.size() << " edges: " << endl;
	for (unsigned int i = 0; i < outEdges.size(); i++) {
		NFANode *desNode = outEdges[i]->desNode();
		if (outEdges[i]->isEpsilon()) {
			cout << "Epsilon edge, desNode " << (int *)desNode << endl;
		} else {
			cout << "Normal edge " << outEdges[i]->charSetStr() << "desNode " << (int *)desNode << endl;
		}
	}
	for (unsigned int i = 0; i < outEdges.size(); i++) {
		NFANode *desNode = outEdges[i]->desNode();
		if (nodes.find(desNode) == nodes.end()) {
			readNFA(desNode);
		}
	}
}

int main()
{
	/*
	RegularExpLoader loader;
	DFAConverter dfaConverter;
	DFAModel *dfaModel;

	vector<NFAModel *> models;
	vector<string> types;
	string regExpStr, type;
	int cnt;
	cin >> cnt;
	while (cnt--) {
		cin >> regExpStr;
		cin >> type;

	//	string addStr = loader.addConcateSymbol(regExpStr);
	//	cout << addStr << endl;
		RegularExp *exp = loader.load(regExpStr);
		traverse(exp);
		NFAConverter converter;
		NFAModel *model = converter.convert(exp);
		assert(model != NULL);
		NFANode *head = model->head();
		NFANode *tail = model->tail();
		cout << "model with head " << (int *)head << " with tail " << (int *)tail << endl;
		readNFA(head);
		models.push_back(model);
		types.push_back(type);
	}
	
	dfaModel = dfaConverter.convertToDFA(models, types);
	cout << "finishing generation DFA table" << endl;
	dfaModel->traverse();	
	string teststr;
	while (1) {
		cin >> teststr;
		vector<pair<string, string> > tokens = dfaModel->match(teststr);
		
		for (unsigned int i = 0; i < tokens.size(); i++) {
			cout << tokens[i].first << "\t" << "\"" << tokens[i].second << "\"" << endl; 
		}
	}
	*/

	TokenScanner scanner("TokenDef.in");
	scanner.scanTokens("code.in");
	return 0;
}
