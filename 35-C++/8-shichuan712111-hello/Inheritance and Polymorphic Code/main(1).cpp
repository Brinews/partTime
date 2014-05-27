#include <iostream>
#include <string>
using namespace std;

#include "shape.h"
#include "circle.h"

void main() {

	Shape s1;
	Shape s2("Test");

	Circle c1(10.0);

	s1.draw();
	s2.draw();
	c1.draw();

	cout << "++++++++++++++++++++++++++++++++++\n";
	Shape *s;
	Circle *c;

	s = &s1;
	c = &c1;
	cout << s << endl;
	cout << c << endl;
	s->draw();
	c->draw();

	cout << "++++++++++++++++++++++++++++++++++\n";

	s = &c1;

	cout << s << endl;
	s->draw();
}


