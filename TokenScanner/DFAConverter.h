#ifndef DFACONVERTER_H
#define DFACONVERTER_H

#include <set>
#include <vector>
#include <string>
#include <map>
using std::set;
using std::string;
using std::vector;
using std::map;

namespace zhjcompiler {

class NFAModel;
class DFAModel;
class NFANode;

class DFAConverter {
public:
	DFAConverter();

	DFAModel *convertToDFA(vector<NFAModel *> models, vector<string> types);
private:
	set<NFANode *> closure(set<NFANode *> s);
	set<NFANode *> dfaEdge(set<NFANode *> s, char c);
	NFAModel *mergeNFAModels(vector<NFAModel *> models, vector<string> types, map<NFANode *, string> &nfaNodeToTypes);
	int findState(vector<set<NFANode *> > states, set<NFANode *> state); 	
	/* not implemented
	void normalizeDFA();
	*/
};
}
#endif
