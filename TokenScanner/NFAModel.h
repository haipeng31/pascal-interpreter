#ifndef NFAMODEL_H
#define NFAMODEL_H

#include <vector>
#include <string>
using std::vector;
using std::string;

namespace zhjcompiler {

class NFANode;

class NFAEdge {
public:
	static const unsigned int SETSIZE;
	NFAEdge(NFANode *desNode);
	NFAEdge(vector<bool> charSet, NFANode *desNode);
	bool isEpsilon();
	vector<bool> &charSet();
	NFANode *desNode();

	// for debug
	string charSetStr();
private:
	vector<bool> charSet_;
	bool epsilon_;
	NFANode *desNode_;
};

class NFANode {
public:
	NFANode();

	void addEpsilonEdge(NFANode *desNode);
	void addEdge(vector<bool> charSet, NFANode *desNode);
	vector<NFAEdge *> &outEdges();
	vector<NFANode *> epsilonDesNodes();
	vector<NFANode *> desNodes(char c);
private:
	vector<NFAEdge *> outEdges_;
};

class NFAModel {
public:
	NFAModel(NFANode *head, NFANode *tail);

	NFANode *head() const;
	NFANode *tail() const;
private:
	NFANode *head_;
	NFANode *tail_;
};

}
#endif
