/*
 * This is a simple caculator, write to know how to 
 * generator posix expression
 */

#include <iostream>
#include <stack>
#include <string>
#include <sstream>

using namespace std;

int nice(char c) {
	if (c == '+' || c == '-') {
		return 1;
	}
	if (c == '*' || c == '/') {
		return 2;
	}
	if (c == '(') {
		return 3;
	}
	if (c == '#' || c == ')') {
		return 0;
	}
}

string posix(string exp)
{
	string posExp;
	stack<char> helpStack;
	
	helpStack.push('#');
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i] >= '0' && exp[i] <= '9') {
			posExp.push_back(exp[i]);
		} else {
			while (helpStack.top() != '(' && nice(helpStack.top()) >= nice(exp[i])) {
				char op = helpStack.top();
				helpStack.pop();
				posExp.push_back(op);
			}
			if (exp[i] != ')') {
				helpStack.push(exp[i]);
			} else {
				while (helpStack.top() != '(') {
					char op = helpStack.top();
					helpStack.pop();
					posExp.push_back(op);
				}
				helpStack.pop();
			}
		}
	}
	
	while (helpStack.size() != 1) {
		posExp.push_back(helpStack.top());
		helpStack.pop();
	}
	return posExp;
}

double caculate(string exp)
{
	string posExp = posix(exp);
	cout << posExp << endl;	
	double result = 0.0;
	stack<double> helpStack;
	for (int i = 0; i < posExp.size(); i++) {
		if (posExp[i] >= '0' && posExp[i] <= '9') {
			helpStack.push(posExp[i]-'0');
		} else {
			double l = helpStack.top();
			helpStack.pop();
			double r = helpStack.top();
			helpStack.pop();

			switch(posExp[i]) {
				case '+':
					helpStack.push(r + l);
					break;

				case '-':
					helpStack.push(r - l);
					break;

				case '*':
					helpStack.push(r * l);
					break;

				case '/':
					helpStack.push(r / l);
					break;

				default:
					break;
			}
					
		}

	}

	return helpStack.top();
}

int main() {
	string exp;

	while (1) {
		cin >> exp;
		cout << caculate(exp) << endl;
	}

	return 0;
}
