#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <iostream>
#include <string>
using namespace std;

class Shape {
private:
	string name;

public:
	Shape() {name="Shape";}
	Shape(string s) {name=s;}
	virtual ~Shape() { }

	virtual void draw() const;
	//virtual double area()=0;
};

#endif
