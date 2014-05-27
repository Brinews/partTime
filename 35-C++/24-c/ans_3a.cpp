#include <iostream>

using namespace std;

int nestloop_print(int up)
{
	int total = up*2 - 1;
	for (int i = 1; i <= total; i++)
	{
		int j = 1;

		int max = i;
		if (max > up) max = up - (max - up);

		for (; j <= max; j++)
		{
			cout << j << " ";
		}
		cout << endl;
	}
}

int main()
{
	nestloop_print(9);
	return 0;
}
