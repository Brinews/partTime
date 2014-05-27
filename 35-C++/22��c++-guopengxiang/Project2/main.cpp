#include "Proj1_Options06.h"
#include "Barrier01.h"

#include <iostream>

using namespace std;

int main()
{
	int N = 4;
	cout << "Input N sequence by 100, 500, 1000, 5000:\n";
	while (N--)
	{
		CallOptionPrice("EurCallPrices.xls");
		PutOptionPrice("EurPutPrices.xls");
	}

	int arr[5] = {100, 500, 1000, 5000, 10000};
	for (int i = 0; i < 5; i ++)
	{
		BarrierOptionPrice(arr[i], "BarrierPrices.xls");
	}

	return 0;
}
