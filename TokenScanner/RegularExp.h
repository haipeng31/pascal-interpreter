#ifndef REGULAREXP_H
#define REGULAREXP_H

//#include "NFAModel.h"
//#include "NFAConverter.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
namespace zhjcompiler {
class AbstractNFAConverter;
class NFAModel;
class RegularExp {
public:
	enum RegularExpType {
		SYMBOL,
		EPSILON,
		CONCATENATION,
		ALTERNATION,
		REPEATATION
	};
		
	virtual RegularExpType type() = 0;
	virtual NFAModel *accept(AbstractNFAConverter *converter) = 0;
};

class SymbolRegularExp : public RegularExp {
public:
	static const unsigned int SETSIZE;
	SymbolRegularExp(char c);
	SymbolRegularExp(RegularExp *start, RegularExp *end);
	
	vector<bool> &charSet();
	void reverse();
	void merge(RegularExp *exp);
	RegularExpType type();
	NFAModel *accept(AbstractNFAConverter *converter);
private:
	vector<bool> charSet_;
};

class EpsilonRegularExp : public RegularExp {
public:
	EpsilonRegularExp();
	
	RegularExpType type();
	NFAModel *accept(AbstractNFAConverter *converter);
private:
	static const string EmptyStr;
};

class ConcatenationExp : public RegularExp {
public:
	ConcatenationExp();

	void addChildExp(RegularExp *child);
	vector<RegularExp *> &childExps();
	void merge(RegularExp *exp);
	RegularExpType type();
	NFAModel *accept(AbstractNFAConverter *converter);
private:
	vector<RegularExp *> childExps_;
};

class AlternationExp : public RegularExp {
public:
	AlternationExp();

	void addChildExp(RegularExp *child);
	vector<RegularExp *> &childExps();
	void merge(RegularExp *exp);
	RegularExpType type();
    NFAModel *accept(AbstractNFAConverter *converter);
private:
	vector<RegularExp *> childExps_;
};

class RepeatationExp : public RegularExp {
public:
	static const int MAX;
	RepeatationExp(RegularExp *exp, int min, int max);
	
	RegularExpType type();
	unsigned int min();
	unsigned int max();
	RegularExp *exp();
	NFAModel *accept(AbstractNFAConverter *converter);
private:
	RegularExp *exp_;
	unsigned int min_;
	unsigned int max_;
};

}
#endif
