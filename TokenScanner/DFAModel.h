#ifndef DFAMODEL_H
#define DFAMODEL_H

#include <vector>
#include <utility>
#include <map>
#include <string>

using std::string;
using std::vector;
using std::pair;
using std::map;
namespace zhjcompiler {

class DFAModel {
public:
	DFAModel(vector<vector<unsigned int> > trans, map<int, string> endStates);
	
	vector<pair<string, string> > match(string str);
	void traverse();
private:
	vector<vector<unsigned int> > trans_;
	map<int, string> endStates_;
};
}
#endif
