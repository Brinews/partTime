//2_8.cpp 输入一个数，求它的所有因子
#include <iostream>
using namespace std;
int main()
{ 
	int n, k;
	
	cout << "Enter a positive integer: ";
	cin >> n;
	cout << "Number  " << n << "   Factors  ";
	
	for (k=1; k <= n; k++)
		if (n % k == 0)
			cout << k << "  ";
		cout << endl;
}
