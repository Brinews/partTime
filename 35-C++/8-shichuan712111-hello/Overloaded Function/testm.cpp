#include <iostream>
#include <string>
using namespace std;

template<class T>
T findMax(T a, T b) {
	if (a>b) return a;
	else return b;
}

void main() {

	cout << findMax(10,12) << endl;
	cout << findMax('a','b') << endl;
	cout << findMax<double>(12.3, 45.10) << endl;


}


