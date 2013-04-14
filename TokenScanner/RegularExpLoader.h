#ifndef REGULAREXPLOADER_H
#define REGULAREXPLOADER_H

#include "RegularExp.h"
#include <string>
using std::string;
namespace zhjcompiler {

class RegularExpLoader {
public:
	RegularExpLoader();
	~RegularExpLoader();

	RegularExp *load(string regExpStr);
private:	
	string posixExp(string regExpStr);
	RegularExp *genExpTree(string posixExp);
	int priority(char c);
	bool isOperation(char c);
	string addHelpSymbol(string regExpStr);
};
}
#endif
