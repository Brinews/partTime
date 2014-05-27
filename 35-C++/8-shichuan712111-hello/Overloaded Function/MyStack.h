#include <iostream>
#include <string>
using namespace std;

template<class T>
class MyStack {
private:
	struct node {
		T data;
		node *next;
	};

	node *top;

public:
	MyStack();
	~MyStack();

	void push(T);
	bool pop(T&);
};

template<class T>
MyStack<T>::MyStack() {
	top = NULL;
}

template<class T>
MyStack<T>::~MyStack() {
	node *n;
	while(top!=NULL) {
		n = top;
		top = top->next;
		n->next = NULL;
		delete n;
	}

}

template<class T>
void MyStack<T>::push(T d) {
	node *n = new node;
	n->data = d;
	n->next = top;
	top = n;
}

template<class T>
bool MyStack<T>::pop(T &d) {
	if (top==NULL) return false;
	node *n = top;
	top = top->next;
	n->next = NULL;
	d = n->data;
	delete n;
	return true;
}










