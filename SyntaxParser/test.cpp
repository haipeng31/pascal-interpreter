#include <iostream>
#include <assert.h>
#include "ZhjSmartPointer.h"
using namespace std;

class Test {
public:
	Test() { cout << "create object" << endl; }
	~Test() { cout << "delete object" << endl; }

	int num() const { return num_; }
	void setNum(int num) { num_ = num; }

private:
	int num_;
};

int main()
{
	{

	ZhjSmartPointer<Test> a(new Test());
	a->setNum(2);
	cout << a->num() << endl;
	ZhjSmartPointer<Test> b(a);
	ZhjSmartPointer<Test> c(new Test());
	c = a;
	cout << (*c).num() << endl;
	(*c).setNum(3);
	cout << (*c).num() << endl;
	ZhjSmartPointer<Test> d = a = b = c;
	assert(d == a && a == b);	
	}

	ZhjSmartPointer<Test> e;
	if (e) {
		cout << "error in operator bool" << endl;
	}

	if (e == NULL) {
		
	}

	if (e != NULL) {
		cout << "error in !=" << endl;
	}

	cout << "a" << endl;
	return 0;
}
