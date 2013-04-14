#include "NFAModel.h"

using namespace zhjcompiler;

/* implementation of NFAEdge */
const unsigned int NFAEdge::SETSIZE = 128;

NFAEdge::NFAEdge(NFANode *desNode)
 :  epsilon_(true),
	desNode_(desNode)
{
}

NFAEdge::NFAEdge(vector<bool> charSet, NFANode *desNode)
 :	charSet_(charSet),
	epsilon_(false),
	desNode_(desNode)
{
}

bool NFAEdge::isEpsilon()
{
	return epsilon_;
}

vector<bool> &NFAEdge::charSet()
{
	return charSet_;
}

NFANode *NFAEdge::desNode()
{
	return desNode_;
}

string NFAEdge::charSetStr()
{
	string str;
	for (unsigned int i = 0; i < charSet_.size(); i++) {
		if (charSet_[i]) {
			str.push_back((char)i);
		}
	}
	return str;
}
/* implementation of NFANode */
NFANode::NFANode()
{
}

void NFANode::addEpsilonEdge(NFANode *desNode)
{
	NFAEdge *edge = new NFAEdge(desNode);
	outEdges_.push_back(edge);
}

void NFANode::addEdge(vector<bool> charSet, NFANode *desNode)
{
	NFAEdge *edge = new NFAEdge(charSet, desNode);
	outEdges_.push_back(edge);
}

vector<NFAEdge *> &NFANode::outEdges()
{
	return outEdges_;
}

vector<NFANode *> NFANode::epsilonDesNodes()
{
	vector<NFANode *> nodes;
	for (unsigned int i = 0; i < outEdges_.size(); i++) {
		if (outEdges_[i]->isEpsilon()) {
			nodes.push_back(outEdges_[i]->desNode());
		}
	}

	return nodes;
}

vector<NFANode *> NFANode::desNodes(char c)
{
	vector<NFANode *> nodes;
	for (unsigned int i = 0; i < outEdges_.size(); i++) {
		vector<bool> charSet = outEdges_[i]->charSet();
		if (!outEdges_[i]->isEpsilon() && charSet[c]) {
			nodes.push_back(outEdges_[i]->desNode());
		}
	}
	return nodes;
}

/* implementation of NFAModel */
NFAModel::NFAModel(NFANode *head, NFANode *tail)
 :	head_(head),
	tail_(tail)
{
}

NFANode *NFAModel::head() const
{
	return head_;
}

NFANode *NFAModel::tail() const
{
	return tail_;
}

