#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include <iostream>
#include <string>
using namespace std;

#include "shape.h"

class Circle : public Shape {
private:
	double radius;

public:
	Circle();
	Circle(double);
	~Circle() { }

	virtual void draw() const;
};

#endif
