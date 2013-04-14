#ifndef STACK_H
#define STACK_H

#include <vector>
#include <cassert>
using std::vector;

namespace zhjcompiler {

template <typename T>
class Stack {
public:
	Stack();
	void push(const T &value);
	void pop();
	const T &top() const;
	T &operator[](size_t index);
	size_t size() const;

private:
	vector<T> vec_;
};	// end of class Stack

template <typename T>
Stack<T>::Stack()
 :	vec_()
{
}

template <typename T>
void Stack<T>::push(const T &value)
{
	vec_.push_back(value);
}

template <typename T>
void Stack<T>::pop()
{
	vec_.pop_back();
}

template <typename T>
const T &Stack<T>::top() const
{
	return vec_[vec_.size()-1];
}

template <typename T>
T &Stack<T>::operator [](size_t index)
{
	assert(index < vec_.size());
	return vec_[index];
}

template <typename T>
size_t Stack<T>::size() const
{
	return vec_.size();
}

}   // end of namespace 
#endif
