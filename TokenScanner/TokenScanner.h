#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H

#include <utility>
#include <string>
#include <vector>
using std::pair;
using std::string;
using std::vector;

namespace zhjcompiler {

class DFAModel;
class TokenScanner {
public:
	typedef pair<string, string> Token;
	typedef pair<Token, int> TokenInfo;
	TokenScanner(const string  &lexDefFile);
	vector<TokenInfo> scanTokens(const string &codeFile);
private:
	DFAModel *dfaModel_;
};
}
#endif
