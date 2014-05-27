#include <iostream>

#include "queue.h"

using namespace std;

int main(int argc, char* argv[])
{
	int p = 0;
	queue q;
	
	while (p < 100)
	{
		if (p%2)
			q.enqueue(p);
		else
			q.enqueue(50);
		p++;
	}

	cout << "remove 50 count = " << q.remove(50) << endl;

	p = 0;
	while (p < 100)
	{
		if (p%2)
			cout << "remove " << p << " = " <<  q.remove(p) << endl;
		else
			cout << "front now = " << q.front() << endl;

		p++;
	}

	cout << "queue size = " << q.size() << endl;

	return 0;
}
