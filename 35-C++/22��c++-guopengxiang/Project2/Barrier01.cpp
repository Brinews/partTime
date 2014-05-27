#include "Barrier01.h"

#include <cmath>
#include <iostream>
#include <fstream>

#include <vector>

using namespace std;

/*************************UpAndOutPut************************/
double UpAndOutCall::Payoff(double z)
{
	if (z < barrier) {
		if (z > K) return z - K;
	}

	return 0.0;
}

double UpAndOutCall::PriceByTree(TriModel Model)
{
	int N = getN();
	Model.Setdt(1.0, N);
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	double h = exp(-1*Model.GetR()*Model.Getdt());

	return h * (pu*Model.S(N+1,1) + pm*Model.S(N+1, 0) + pd*Model.S(N+1,-1));
}

/*********************DownAndOutPut************************/

double DownAndOutPut::Payoff(double z)
{
	if (z > barrier) {
		if (z < K) return K - z;
	}

	return 0.0;
}

double DownAndOutPut::PriceByTree(TriModel Model)
{
	int N = getN();
	Model.Setdt(1.0, N);
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	double h = exp(-1*Model.GetR()*Model.Getdt());

	return h * (pu*Model.S(N+1,1) + pm*Model.S(N+1, 0) + pd*Model.S(N+1,-1));
}

/*********************DownAndOutPut************************/

double DoubleBarrierOutCall::Payoff(double z)
{
	if (z < upper_barrier) {
		if (z > K) return z -K;
	}

	if (z > lower_barrier) {
		if (z < K) return K - z;
	}

	return 0.0;
}

double DoubleBarrierOutCall::PriceByTree(TriModel Model)
{
	int N = getN();
	Model.Setdt(1.0, N);
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	//double h = exp(-1*Model.GetR()*Model.Getdt());

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

	//return h * (pu*Model.S(N+1,1) + pm*Model.S(N+1, 0) + pd*Model.S(N+1,-1));
}

/*********************Test Case************************/
void BarrierOptionPrice(int N, char *fileName= "BarrierPrices.txt")
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

	//write to txt
	cout <<"Price = " << price << endl;
	out << price << "\t";
	out << endl;
	out.close();

	delete dboc;
}
