#include "RegularExp.h"
#include "NFAConverter.h"
#include "NFAModel.h"
#include <assert.h>

using namespace zhjcompiler;

const unsigned int SymbolRegularExp::SETSIZE = 256;

SymbolRegularExp::SymbolRegularExp(char c)
{
	charSet_.resize(SETSIZE);
	for (unsigned int i = 0; i < charSet_.size(); i++) {
		charSet_[i] = false;
	}
	charSet_[c] = true;
}

SymbolRegularExp::SymbolRegularExp(RegularExp *start, RegularExp *end)
{
	charSet_.resize(SETSIZE);
	for (unsigned int i = 0; i < charSet_.size(); i++) {
		charSet_[i] = false;
	}
	
	SymbolRegularExp *startExp = (SymbolRegularExp *)start;
	SymbolRegularExp *endExp = (SymbolRegularExp *)end;
	vector<bool> startSet = startExp->charSet();
	vector<bool> endSet = endExp->charSet();
	int startIndex = -1;
	for (unsigned int i = 0; i < SETSIZE; i++) {
		if (startSet[i] == true) {
			startIndex = i;
			break;
		}
	}
	for (unsigned int i = startIndex; i < SETSIZE; i++) {
		if (endSet[i] == false) {
			charSet_[i] = true;
		} else {
			charSet_[i] = true;
			break;
		}
	}
	
	delete start;
	delete end;
}

vector<bool> &SymbolRegularExp::charSet()
{
	return charSet_;
}

void SymbolRegularExp::reverse()
{
	for (unsigned int i = 0; i < charSet_.size(); i++) {
		if (charSet_[i]) {
			charSet_[i] = false;
		} else {
			charSet_[i] = true;
		}
	}
}

void SymbolRegularExp::merge(RegularExp *exp)
{
	assert(exp->type() == SYMBOL);

	/* check C++'s type transformation operation, fix this */
	SymbolRegularExp *symbolExp = (SymbolRegularExp *)exp;
	vector<bool> charSet = symbolExp->charSet();
	for (unsigned int i = 0; i < charSet.size(); i++) {
		if (charSet[i]) {
			charSet_[i] = true;
		}
	}

	delete exp;
}

RegularExp::RegularExpType SymbolRegularExp::type()
{
	return SYMBOL;
}

NFAModel *SymbolRegularExp::accept(AbstractNFAConverter *converter)
{
	return converter->convertSymbolExp(this);
}

const string EpsilonRegularExp::EmptyStr = "";

EpsilonRegularExp::EpsilonRegularExp()
{
}

RegularExp::RegularExpType EpsilonRegularExp::type()
{
	return EPSILON;
}

/* maybe we should fix this */
NFAModel *EpsilonRegularExp::accept(AbstractNFAConverter *converter)
{
	return NULL;
}


ConcatenationExp::ConcatenationExp()
{
}

void ConcatenationExp::addChildExp(RegularExp *child)
{
	childExps_.push_back(child);
}

vector<RegularExp *> &ConcatenationExp::childExps()
{
	return childExps_;
}

void ConcatenationExp::merge(RegularExp *exp)
{
	assert(exp->type() == CONCATENATION);
	
	ConcatenationExp *concatenationExp = (ConcatenationExp *)exp;
	vector<RegularExp *> childExps = concatenationExp->childExps();
	for (unsigned int i = 0; i < childExps.size(); i++) {
		childExps_.push_back(childExps[i]);
	}

	delete concatenationExp;
}

RegularExp::RegularExpType ConcatenationExp::type()
{
	return CONCATENATION;
}

NFAModel *ConcatenationExp::accept(AbstractNFAConverter *converter)
{
	return converter->convertConcateExp(this);
}


AlternationExp::AlternationExp()
{
}

void AlternationExp::addChildExp(RegularExp *child)
{
	childExps_.push_back(child);
}

vector<RegularExp *> &AlternationExp::childExps()
{
	return childExps_;
}

void AlternationExp::merge(RegularExp *exp)
{
	assert(exp->type() == ALTERNATION);
	
	AlternationExp *alternationExp = (AlternationExp *)exp;
	vector<RegularExp *> childExps = alternationExp->childExps();
	for (unsigned int i = 0; i < childExps.size(); i++) {
		childExps_.push_back(childExps[i]);
	}
}

RegularExp::RegularExpType AlternationExp::type()
{
	return ALTERNATION;
}

NFAModel *AlternationExp::accept(AbstractNFAConverter *converter)
{
	return converter->convertAlterExp(this);
}

const int RepeatationExp::MAX = 1000;
RepeatationExp::RepeatationExp(RegularExp *exp, int min, int max)
 :  exp_(exp),
	min_(min),
	max_(max)
{
}

RegularExp::RegularExpType RepeatationExp::type()
{
	return REPEATATION;
}

unsigned int RepeatationExp::min()
{
	return min_;
}

unsigned int RepeatationExp::max()
{
	return max_;
}

RegularExp *RepeatationExp::exp()
{
	return exp_;
}

NFAModel *RepeatationExp::accept(AbstractNFAConverter *converter)
{
	return converter->convertRepeatExp(this);
}

