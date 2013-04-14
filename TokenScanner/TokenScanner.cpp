#include "TokenScanner.h"
#include "RegularExp.h"
#include "RegularExpLoader.h"
#include "NFAModel.h"
#include "NFAConverter.h"
#include "DFAModel.h"
#include "DFAConverter.h"

#include <fstream>
#include <iostream>
#include <assert.h>

using std::ifstream;
using std::cout;
using std::endl;
using namespace zhjcompiler;

TokenScanner::TokenScanner(const string &lexDefFile)
{
	RegularExpLoader loader;
	NFAConverter nfaConverter;
	DFAConverter dfaConverter;

	vector<NFAModel *> models;
	vector<string> types;
	string regExpStr, type;
	
	ifstream lexFileStream(lexDefFile.c_str());
	while (lexFileStream >> regExpStr >> type) {
		RegularExp *exp = loader.load(regExpStr);
		NFAModel *model = nfaConverter.convert(exp);
		models.push_back(model);
		types.push_back(type);
	}
	
	// regularExp of \t\n\32
	regExpStr = "";
	regExpStr.push_back(' ');
	regExpStr.push_back('|');
	regExpStr.push_back('\t');
	regExpStr.push_back('|');
	regExpStr.push_back('\n');
	type = "SPACE";

	RegularExp *exp = loader.load(regExpStr);
	NFAModel *model = nfaConverter.convert(exp);
	models.push_back(model);
	types.push_back(type);

	dfaModel_ = dfaConverter.convertToDFA(models, types);
}

vector<TokenScanner::TokenInfo> TokenScanner::scanTokens(const string &codeFile)
{
	vector<TokenInfo> tokenInfos;
	ifstream codeFileStream;
	codeFileStream.open(codeFile.c_str(), std::ios::in);
	string line;
	int lineno = 1;
	while (!codeFileStream.eof()) {
		std::getline(codeFileStream, line);
		vector<Token> subTokens = dfaModel_->match(line);
		for (unsigned int i = 0; i < subTokens.size(); i++) {
			if (subTokens[i].first != "SPACE") {
				tokenInfos.push_back(pair<Token, int>(subTokens[i], lineno));			
			}
		}
		lineno++;
	}
	
	for (unsigned int i = 0; i < tokenInfos.size(); i++) {
		cout << tokenInfos[i].first.first << " " << tokenInfos[i].first.second << " " << tokenInfos[i].second << endl;
	}
	return tokenInfos;
}
