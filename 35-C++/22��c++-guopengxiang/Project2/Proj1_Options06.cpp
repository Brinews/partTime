#include "TriModel01.h"
#include "Proj1_Options06.h"

#include <iostream>
#include <fstream>

#include <cmath>

#include <vector>

using namespace std;

double EurOption::PriceByCRR(TriModel Model)
{
    // please implement European opton pricing by a Trinomial Tree here
	
	Model.Setdt(1.0, N);
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	vector<double> prices(N + 1);
	for (int i = 0; i <= N; i++)
		prices[i] = Model.S(N, i);
	
	vector<double> call_values(N + 1);
	for (int j = 0; j<= N; j++)
		call_values[j] = Payoff(prices[j]);

	double h = exp(-1*Model.GetR()*Model.Getdt());

	for (int step = N - 1; step >= 0; --step)
	{
		for (int i = 0; i <= step; ++i)
		{
			call_values[i] = pu*Model.S(N, i+1) + pm*Model.S(N, i) + pd*Model.S(N, i-1);
			//call_values[i] = pu*call_values[i+1] + pm*call_values[i] + pd*call_values[i-1];
		}
	}

	return h * call_values[0];
}

int Call::GetInputData()
{
   cout << "Enter call option data:" << endl;
   int N;
   cout << "Enter steps to expiry N: "; cin >> N;
   SetN(N);
   cout << "Enter strike price K:    "; cin >> K;
   cout << endl;
   return 0;
}

double Call::Payoff(double z)
{
   if (z>K) return z-K;
   return 0.0;
}

int Put::GetInputData()
{
   cout << "Enter put option data:" << endl;
   int N;
   cout << "Enter steps to expiry N: "; cin >> N;
   SetN(N);
   cout << "Enter strike price K:    "; cin >> K;
   cout << endl;
   return 0;
}

double Put::Payoff(double z)
{
   if (z<K) return K-z;
   return 0.0;
}

/**
 *Test Case
 */
void CallOptionPrice(char *fileName= "EurCallPrices.txt")
{
	ofstream out;
	out.open(fileName, ios::app);
	if (out.fail()) {
		cerr << "Open File " << fileName << " Error\n";
		return;
	}

	Call *c = new Call();
	c->SetK(100);
	c->GetInputData();

	for (int s = 80; s <= 120; s += 10) {
		TriModel m(1.0*s, 0.03, 0.2, 0.05);

		double price = c->PriceByCRR(m);

		//write to txt
		out << price << "\t";
	}
	out << endl;
	out.close();

	delete c;
}

void PutOptionPrice(char *fileName= "EurPutPrices.txt")
{
	ofstream out;
	out.open(fileName, ios::app);
	if (out.fail()) {
		cerr << "Open File " << fileName << " Error\n";
		return;
	}

	Put *p = new Put();
	p->SetK(100);
	p->GetInputData();

	for (int s = 80; s <= 120; s += 10) {
		TriModel m(1.0*s, 0.03, 0.2, 0.05);

		double price = p->PriceByCRR(m);

		//write to txt
		out << price << "\t";
	}
	out << endl;
	out.close();

	delete p;
}


