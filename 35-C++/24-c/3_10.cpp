#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int main()
{
	int x, y;
	string message;
	double z;
	x = 4;
	y = 3;
	z = 2.5;
	cout <<static_cast<int>(pow(x, 2.0)) << endl;
	cout << static_cast<int>(pow(z, y)) << endl;
	cout << pow(x, z) << endl;
	cout << sqrt(36.0) << endl;
	z = pow(9.0, 2.5);
	cout << z << endl;
	message = "Using C++ predefined function";
	cout << "Length of message = "
		<< message.length() << endl;
	return 0;
}

