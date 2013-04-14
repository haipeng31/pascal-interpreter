#include "RunStackItem.h"
#include <cassert>

using namespace zhjcompiler;

RunStackItem::RunStackItem(RunStackItem::Type type, int value)
{
	assert(type == INT);
	type_ = type;
	value_.intValue_ = value;
}

RunStackItem::RunStackItem(RunStackItem::Type type, float value)
{
	assert(type == FLOAT);
	type_ = type;
	value_.floatValue_ = value;
}

RunStackItem::RunStackItem(RunStackItem::Type type, bool value)
{
	assert(type == BOOL);
	type_ = type;
	value_.boolValue_ = value;
}

int RunStackItem::intValue() const
{
	assert(type_ == INT);
	return value_.intValue_;
}

float RunStackItem::floatValue() const
{
	assert(type_ == FLOAT);
	return value_.floatValue_;
}

bool RunStackItem::boolValue() const
{
	assert(type_ == BOOL);
	return value_.boolValue_;
}

RunStackItem::Type RunStackItem::type() const
{
	return type_;
}
