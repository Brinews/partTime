#include <iostream>
#include <string>
using namespace std;

#include "mtime.h"

void main() {

	MTime t1(230);
	MTime t2(530);

	cout << t1 << endl;
	cout << t2 << endl;

	MTime t3 = t1 + t2;
	MTime t4 = t2 - t1;

	cout << t1 << endl;
	cout << t2 << endl;
	cout << t3 << endl;
	cout << t4 << endl;

	if (t1>t2) cout << "t1 larger";
	else cout << "t1 smaller or equal";
	cout << endl;

	MTime t5 = ++t1;
	MTime t6 = t1++;
	cout << t1 << endl;
	cout << t5 << endl;
	cout << t6 << endl;
	cout << t1 << endl;


}

