#ifndef RUNSTACKITEM_H
#define RUNSTACKITEM_H

namespace zhjcompiler {

class RunStackItem {
public:
	enum Type {
		INT,
		FLOAT,
		BOOL
	};

	RunStackItem(Type type, int value);
	RunStackItem(Type type, float value);
	RunStackItem(Type type, bool value);
	
	Type type() const;
	int intValue() const;
	float floatValue() const;
	bool boolValue() const;

private:
	Type type_;
	union Value {
		int intValue_;
		float floatValue_;
		bool boolValue_;
	} value_;

};	// end of class
}	// end of namespace 
#endif
