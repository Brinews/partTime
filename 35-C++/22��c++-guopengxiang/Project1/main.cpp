#include "Proj1_Options06.h"
#include "Barrier01.h"

#include <iostream>
#include <fstream>

using namespace std;

/**
 *Test Case: CallPrice, PutPrice
 */
void GetCallOptionPrice(char *fileName)
{
	ofstream out;
	out.open(fileName, ios::app);
	if (out.fail()) {
		cerr << "Open File " << fileName << " Error\n";
		return;
	}

	//set the K and N
	Call *c = new Call();
	c->SetK(100);
	c->GetInputData();

	//given some (K,N), for each S, get the Price
	for (int s = 80; s <= 120; s += 10) {
		//init trimodel
		TriModel m(1.0*s, 0.03, 0.2, 0.05);

		double price = c->PriceByCRR(m);

		//write to xls
		out << price << "\t";
	}
	out << endl;
	out.close();

	delete c;
}

void GetPutOptionPrice(char *fileName)
{
	ofstream out;
	out.open(fileName, ios::app);
	if (out.fail()) {
		cerr << "Open File " << fileName << " Error\n";
		return;
	}

	//calculate as GetCallOptionPrice
	Put *p = new Put();
	p->SetK(100);
	p->GetInputData();

	for (int s = 80; s <= 120; s += 10) {
		TriModel m(1.0*s, 0.03, 0.2, 0.05);

		double price = p->PriceByCRR(m);

		//write to xls
		out << price << "\t";
	}
	out << endl;
	out.close();

	delete p;
}

//consideration by barrier
void GetBarrierOptionPrice(int N, char *fileName)
{
	ofstream out;
	out.open(fileName, ios::app);
	if (out.fail()) {
		cerr << "Open File " << fileName << " Error\n";
		return;
	}

	DoubleBarrierOutCall *dboc 
		= new DoubleBarrierOutCall(120, 80, 100, N);

	TriModel m(100.0, 0.03, 0.2, 0.05);

	double price = dboc->PriceByTree(m);

	//write to xls
	cout <<"Price = " << price << endl;
	out << price << "\t";
	out << endl;
	out.close();

	delete dboc;
}

int main()
{
	int N = 4;
	cout << "Input N, Including 100, 500, 1000, 5000:\n";
	while (N--)
	{
		GetCallOptionPrice("EurCallPrices.xls");
		GetPutOptionPrice("EurPutPrices.xls");
	}

	//foreach N(100... 10000)
	int arr[5] = {100, 500, 1000, 5000, 10000};
	for (int i = 0; i < 5; i ++)
	{
		GetBarrierOptionPrice(arr[i], "BarrierPrices.xls");
	}

	return 0;
}
