#include <iostream>

#include "queue.h"

using namespace std;

int main(int argc, char* argv[])
{
	int p = 0;
	queue q;
	
	while (p < 100)
	{
		q.enqueue(p);
		p++;
	}

	while (p > 0) {
		cout << q.dequeue() << endl;
		p--;
	}

	//cout << q.front() << endl;

	return 0;
}
