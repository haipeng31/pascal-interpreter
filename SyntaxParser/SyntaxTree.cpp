#include "SyntaxTree.h"

using namespace zhjcompiler;

SyntaxNode::~SyntaxNode()
{
}

NonTerminalNode::NonTerminalNode(SyntaxNodeType type)
 :	type_(type),
	childs_()
{
}

SyntaxNode::SyntaxNodeType NonTerminalNode::type() const 
{
	return type_;
}

void NonTerminalNode::addChild(ZhjSmartPointer<SyntaxNode> child)
{
	childs_.push_back(child);
}

vector<ZhjSmartPointer<SyntaxNode> > NonTerminalNode::childs()
{
	return childs_;
}

int NonTerminalNode::childCnt()
{
	return childs_.size();
}

TerminalNode::TerminalNode(TokenInfo tokenInfo)
 :	tokenInfo_(tokenInfo)
{
}

SyntaxNode::SyntaxNodeType TerminalNode::type() const
{
	return TERMINAL;
}

TerminalNode::TokenInfo TerminalNode::tokenInfo()
{
	return tokenInfo_;
}

string TerminalNode::tokenStr()
{
	return tokenInfo_.first.second;
}

string TerminalNode::tokenType()
{
	return tokenInfo_.first.first;
}

int TerminalNode::line()
{
	return tokenInfo_.second;
}
