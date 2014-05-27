#include <iostream>

using namespace std;

int number_comb(int);

int COUNT;
int total;
int c1, c2, c3;

int number_comb(int t)
{
	COUNT = 0;

	for (c1 = 0; c1 <= t; c1++) {
		for (c2 = c1; c2 <= t; c2++) {
			for (c3 = c2; c3 <= t; c3++) {
				if (c1+c2+c3 == t) {
					cout << c1 << " " << " "<< c2 << " "<<c3 << endl;
					COUNT++;
				}
			}
		}
	}

	return COUNT;
}

int main()
{
	while (cin >> total)
	{
		number_comb(total);

		cout << "Combination COUNT = " << COUNT << endl;
	}

	return 0;
}
