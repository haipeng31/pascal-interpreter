#ifndef NFACONVERTER_H
#define NFACONVERTER_H

//#include "NFAModel.h"
//#include "RegularExp.h"

namespace zhjcompiler {

class RegularExp;
class SymbolRegularExp;
class ConcatenationExp;
class AlternationExp;
class RepeatationExp;
class NFAModel;

class AbstractNFAConverter {
public:
	virtual NFAModel *convertSymbolExp(SymbolRegularExp *exp) = 0;
	virtual NFAModel *convertConcateExp(ConcatenationExp *exp) = 0;
	virtual NFAModel *convertAlterExp(AlternationExp *exp) = 0;
	virtual NFAModel *convertRepeatExp(RepeatationExp *exp) = 0;
	virtual ~AbstractNFAConverter();
};

class NFAConverter : public AbstractNFAConverter {
public:
	NFAConverter();
	~NFAConverter();

	NFAModel *convert(RegularExp *exp);
	NFAModel *convertSymbolExp(SymbolRegularExp *exp);
	NFAModel *convertConcateExp(ConcatenationExp *exp);
	NFAModel *convertAlterExp(AlternationExp *exp);
	NFAModel *convertRepeatExp(RepeatationExp *exp);
};

}
#endif
