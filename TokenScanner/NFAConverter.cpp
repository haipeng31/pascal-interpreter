#include "NFAConverter.h"
#include "NFAModel.h"
#include "RegularExp.h"
#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;
using namespace zhjcompiler;

AbstractNFAConverter::~AbstractNFAConverter()
{
}

NFAConverter::NFAConverter()
{
}

NFAConverter::~NFAConverter()
{
}

NFAModel *NFAConverter::convert(RegularExp *exp)
{
	assert(exp != NULL);
	return exp->accept(this);
}


NFAModel *NFAConverter::convertSymbolExp(SymbolRegularExp *exp)
{
	NFANode *head = new NFANode();
	NFANode *tail = new NFANode();
//	std::cout << "add normal edge" << std::endl;
	head->addEdge(exp->charSet(), tail);
	vector<NFAEdge *> outEdges = head->outEdges();
//	cout << "Node " << (int *)head <<  " has " << outEdges.size() << " edges" << endl;
	return new NFAModel(head, tail);
}

NFAModel *NFAConverter::convertConcateExp(ConcatenationExp *exp)
{
	NFANode *head = NULL, *tail = NULL;
	
	NFANode *preTail = NULL;
	vector<RegularExp *> childExps = exp->childExps();
//	std::cout << "childExps with " << childExps.size() << " children" << std::endl;
	for (unsigned int i = 0; i < childExps.size(); i++) {
		NFAModel *childModel = childExps[i]->accept(this);
		NFANode *childHead = childModel->head();
		NFANode *childTail = childModel->tail();
		
		if (preTail == NULL) {
			head = childHead;
		} else {
//			std::cout << "add edge" << std::endl;
			preTail->addEpsilonEdge(childHead);
		}
		preTail = childTail;

		if (i == childExps.size() - 1) {
			tail = childTail;
		}
	}

	return new NFAModel(head, tail);
}

NFAModel *NFAConverter::convertAlterExp(AlternationExp *exp)
{
	NFANode *head = new NFANode();
	NFANode *tail = new NFANode();

	vector<RegularExp *> childExps = exp->childExps();
	for (unsigned int i = 0; i < childExps.size(); i++) {
		NFAModel *childModel = childExps[i]->accept(this);
		NFANode *childHead = childModel->head();
		NFANode *childTail = childModel->tail();

		head->addEpsilonEdge(childHead);
		childTail->addEpsilonEdge(tail);
	}

	return new NFAModel(head, tail);
}

NFAModel *NFAConverter::convertRepeatExp(RepeatationExp *exp)
{
	NFANode *head = NULL, *tail = NULL;
	int min = exp->min();
	int max = exp->max();
	
	RegularExp *childExp = exp->exp();
	NFAModel *childModel =	childExp->accept(this);
	NFANode *childHead = childModel->head();
	NFANode *childTail = childModel->tail();
	if (min == 0 && max == RepeatationExp::MAX) {
		head = tail = new NFANode();
		head->addEpsilonEdge(childHead);
		childTail->addEpsilonEdge(head);
	} else if (min == 1 && max == RepeatationExp::MAX) {
		childTail->addEpsilonEdge(childHead);
		head = childHead;
		tail = childTail;
	} else if (min == 0 && max == 1) {
		childHead->addEpsilonEdge(childTail);
		head = childHead;
		tail = childTail;
	}
	
	return new NFAModel(head, tail);
}

