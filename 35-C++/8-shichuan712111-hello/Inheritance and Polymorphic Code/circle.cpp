#include "circle.h"

Circle::Circle() : Shape("Circle") {
	radius = 0.0;
}

Circle::Circle(double r) : Shape("Circle") {
	radius = r;
}

void Circle::draw() const {
	Shape::draw();
	cout << "Radius:" << radius << endl;
}
