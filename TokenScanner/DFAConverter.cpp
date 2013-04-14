#include "DFAConverter.h"
#include "NFAModel.h"
#include "DFAModel.h"
#include <map>
#include <iostream>
using std::map;
using std::cout;
using std::endl;

using namespace zhjcompiler;

DFAConverter::DFAConverter()
{
}


/* for debug */
void printState(int index, set<NFANode *>state)
{
	cout << "State " << index;
	set<NFANode *>::iterator sit;
	for (sit = state.begin(); sit != state.end(); sit++) {
		cout << " " << (int *)(*sit);
	}
	cout << endl;
}
DFAModel *DFAConverter::convertToDFA(vector<NFAModel *> models, vector<string> types)
{
	map<NFANode *, string>	nfaNodeToTypes;
	NFAModel *model = mergeNFAModels(models, types, nfaNodeToTypes);

	vector<set<NFANode *> > states;
	vector<vector<unsigned int> >trans;

	/* add { } to states */
	set<NFANode *> nullState;
	states.push_back(nullState);
	printState(0, nullState);

	/* add closure{s1} to states */
	set<NFANode *> startNodeSet;
	startNodeSet.insert(model->head());
	set<NFANode *> startState = closure(startNodeSet);
	states.push_back(startState);
	printState(1, startState);
	
	vector<unsigned int> tempTran;
	tempTran.resize(NFAEdge::SETSIZE, 0);
	trans.push_back(tempTran);
	trans.push_back(tempTran);

	unsigned int i = 1;
	while (i < states.size()) {
		for (unsigned int j = 0; j < NFAEdge::SETSIZE; j++) {
			//cout << "state " << i << " " << j << endl;
			set<NFANode *> state = dfaEdge(states[i], (char)j);
			/*
			if (state.size() > 0) {
				cout << "state size > 0" << endl;
			}
			*/
			int index = findState(states, state);
			if (index == -1) {
				states.push_back(state);
	//			printState(states.size()-1, state);			
				vector<unsigned int> tempTran;
				tempTran.resize(NFAEdge::SETSIZE, 0);
				trans.push_back(tempTran);
				trans[i][j] = states.size() - 1;
			} else {
				trans[i][j] = index;
			}
			
		}
		i++;
	}
	
	map<int, string> dfaNodeToTypes;
	for (unsigned int i = 0; i < states.size(); i++) {
		map<NFANode *, string>::iterator mit;
		for (mit = nfaNodeToTypes.begin(); mit != nfaNodeToTypes.end(); mit++) {
			if (states[i].find(mit->first) != states[i].end()) {
				dfaNodeToTypes.insert(pair<int, string>(i, mit->second));
				break;
			}
		}
	}
	return new DFAModel(trans, dfaNodeToTypes);
}

set<NFANode *> DFAConverter::closure(set<NFANode *> s)
{
	vector<NFANode *> newNodes;

	set<NFANode *>::iterator sit;
	for (sit = s.begin(); sit != s.end(); sit++) {
		newNodes.push_back(*sit);
	}

	while (newNodes.size()) {
		vector<NFANode *> tempNodes;
		for (unsigned int i = 0; i < newNodes.size(); i++) {
			vector<NFANode *> epsilonNodes = newNodes[i]->epsilonDesNodes();
			for (unsigned int j = 0; j < epsilonNodes.size(); j++) {
				if (s.find(epsilonNodes[j]) == s.end()) {
					s.insert(epsilonNodes[j]);
					tempNodes.push_back(epsilonNodes[j]);
				}
			}
		}
		newNodes = tempNodes;
	}

	return s;
}

set<NFANode *> DFAConverter::dfaEdge(set<NFANode *> s, char c)
{
	set<NFANode *> nodes;

	set<NFANode *>::iterator sit;
	for (sit = s.begin(); sit != s.end(); sit++) {
		vector<NFANode *> tempNodes = (*sit)->desNodes(c);
		for (unsigned int i = 0; i < tempNodes.size(); i++) {
			nodes.insert(tempNodes[i]);
		}
	}

	return closure(nodes);
}

NFAModel *DFAConverter::mergeNFAModels(vector<NFAModel *> models, vector<string> types, map<NFANode *, string> &nfaNodeToTypes)
{
	for (unsigned int i = 0; i < models.size(); i++) {
		NFANode *tail = models[i]->tail();
		nfaNodeToTypes.insert(pair<NFANode *, string>(tail, types[i]));
	}

	NFANode *head = new NFANode();
	NFANode *tail = new NFANode();
	for (unsigned int i = 0; i < models.size(); i++) {
		NFANode *childHead = models[i]->head();
		NFANode *childTail = models[i]->tail();
		head->addEpsilonEdge(childHead);
		childTail->addEpsilonEdge(tail);
	}
	
	return new NFAModel(head, tail);
}

int DFAConverter::findState(vector<set<NFANode *> > states, set<NFANode *> state)
{
	for (unsigned int i = 0; i < states.size(); i++) {
		if (states[i] == state) {
			return i;	
		}
	}

	return -1;
}
