#include "MyStack.h"


void main() {

	MyStack<int> iStack;
	MyStack<string> sStack;

	iStack.push(10);
	iStack.push(4);
	iStack.push(3);
	iStack.push(9);

	sStack.push("world");
	sStack.push("hello");


	int d;

	iStack.pop(d);
	cout << d << endl;
	iStack.pop(d);
	cout << d << endl;
	iStack.pop(d);
	cout << d << endl;
	iStack.pop(d);
	cout << d << endl;

	string s;
	sStack.pop(s);
	cout << s << endl;
	sStack.pop(s);
	cout << s << endl;


}

