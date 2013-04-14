#include "DFAModel.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace zhjcompiler;

DFAModel::DFAModel(vector<vector<unsigned int> > trans, map<int, string> endStates)
 :	trans_(trans),
	endStates_(endStates)
{
}

vector<pair<string, string> > DFAModel::match(string str)
{
	int lastEndState = -1, lastIndex = -1;
	int headIndex = 0, index = 0;
	vector<pair<string, string> > tokens;
	
	unsigned int state = 1;
	if (endStates_.find(state) != endStates_.end()) {
		lastEndState = state;
		lastIndex = index;
	}	
	while (index < str.size()) {
		state = trans_[state][str[index]];
		index++;
	
		if (endStates_.find(state) != endStates_.end()) {
			lastEndState = state;
			lastIndex = index;
		}

		if ((state == 0 || index == str.size()) && lastEndState == -1) {
			string errorStr = str.substr(headIndex, index - headIndex);
			tokens.push_back(pair<string, string>("ERROR", errorStr));
			headIndex++;
			index = headIndex;
			state = 1;
		} else if ((state == 0 || index == str.size()) && lastEndState != -1) {
			string tokenStr = str.substr(headIndex, lastIndex - headIndex);
			tokens.push_back(pair<string, string>(endStates_[lastEndState], tokenStr));
			headIndex = index = lastIndex;
			state = 1;
			lastEndState = lastIndex = -1;
		} 
	

	}

	return tokens;
}

void DFAModel::traverse()
{
	for (int i = 0; i < trans_.size(); i++) {
		for (int j = 0; j < trans_[i].size(); j++) {
			if ((j >= 'a' && j <= 'z') || (j >= '0' && j <= '9')) {
				cout << " " << trans_[i][j];
			}
		}
		cout << endl;
	}
}
