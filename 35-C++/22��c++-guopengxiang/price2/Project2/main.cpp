#include "TriLattice02.h"
#include "TriModel01.h"
#include "Proj2_Options.h"
#include "PathDepOption.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	int N = 500;
	double K = 100.0;

	double S0, r, q, sigma, T;

	/* input the initial parameters */
	cout << "Enter TriModel parameters. S0(100):";
	cin >> S0;
	cout << "r(0.05):";
	cin >> r;
	cout << "q(0.03):";
	cin >> q;
	cout << "sigma(0.2):";
	cin >> sigma;
	cout << "T(1):";
	cin >> T;

	//TriModel Model(100.0, 0.03, 0.2, 0.05);
	/* 
	   construct trimode
	 */
	TriModel Model(S0, q, sigma, r);

	/* 
	   construct put and call class
	 */
	Put myPut;
	Call myCall;

	/* input the steps and price K */
	cout << "Enter Put Option Data.\nEnter steps to expiry N(500): ";
	cin >> N;
	cout << "Enter strike price K(100.0): "; 
	cin >> K;

	/* put model setting T and N, K */
	Model.Setdt(T, N);

	myPut.SetN(N);
	myPut.SetK(K);

	cout << "Enter Call Option Data.\nEnter steps to expiry N(500): ";
	cin >> N;
	cout << "Enter strike price K(100.0): "; 
	cin >> K;

	/* call model setting T and N, K */
	myCall.SetN(N);
	myCall.SetK(K);

	/* the output file stream */
	ofstream out;
	string filename = "EurGreeks.xls";

	out.open(filename.c_str(), ios::app);
	if (out.fail()) {
		cerr << "Open File " << filename << " Error\n";
		return -1;
	}

	//ans for (2)
	vector<double> Greeks;
	out << "Option\tDelta\tGamma\tTheta\n";

	/* call model's price */
	double t = myCall.PriceByCRR(Model, Greeks);
	out << "Call\t" << Greeks[0] << "\t" << Greeks[1] << "\t" << Greeks[2]<<endl;

	/* put model's price */
	myPut.PriceByCRR(Model, Greeks);
	out << "Put\t" << Greeks[0] << "\t" << Greeks[1] << "\t" << Greeks[2]<<endl;

	out.close();

	//ans for (3)
	filename = "AmePrices.xls";
	out.open(filename.c_str(), ios::app);
	if (out.fail()) {
		cerr << "Open File " << filename << " Error\n";
		return -1;
	}

	vector<double> EarlyBoundary;
	vector<double> EarlyBoundary2;
	TriLattice<double> PriceTree;
	vector<double> Greeks2;

	/* put option's price */
	double putOptions = 
		myPut.PriceBySnell(Model, PriceTree, Greeks, EarlyBoundary);

	/* call option's price */
	double callOptions = 
		myCall.PriceBySnell(Model, PriceTree, Greeks2, EarlyBoundary2);

	out << "Option\tPrice\n";
	out << "Call\t"<< callOptions << endl;
	out << "Put\t" << putOptions << endl;

	out.close();

	//ans for (4)

	/* get the ame model's greeks by call and put */
	filename = "AmeGreeks.xls";
	out.open(filename.c_str(), ios::app);
	if (out.fail()) {
		cerr << "Open File " << filename << " Error\n";
		return -1;
	}

	out << "Option\tDelta\tGamma\tTheta\n";
	out << "Call\t" << Greeks2[0] << "\t" << Greeks2[1] << "\t" << Greeks2[2]<<endl;
	out << "Put\t" << Greeks[0] << "\t" << Greeks[1] << "\t" << Greeks[2]<<endl;

	out.close();

	//ans for (5)

	/* get the boundary by call and put */
	filename = "EarlyBoundary.xls";
	out.open(filename.c_str(), ios::app);
	if (out.fail()) {
		cerr << "Open File " << filename << " Error\n";
		return -1;
	}

	out << "Option\t";
	for (int i = N; i <= 2*N; i++)
	{
		out << i-N+1 << "\t";
	}
	out << endl;

	out << "Call\t";
	for (int i = N; i <=2*N; i++)
	{
		out << EarlyBoundary2[i] << "\t";
	}
	out << endl;

	out << "Put\t";
	for (int i = N; i <= 2*N; i++)
	{
		out << EarlyBoundary[i] << "\t";
	}
	out << endl;

	out.close();

	//ans for (7)
	filename = "BarrierPriceMC.xls";
	out.open(filename.c_str(), ios::app);
	if (out.fail()) {
		cerr << "Open File " << filename << " Error\n";
		return -1;
	}

	/* input the paramters: S0, r, sigma, T */
	cout << "Enter bsModel parameters. S0(100):";
	cin >> S0;
	cout << "r(0.05):";
	cin >> r;
	cout << "Sigma(0.2):";
	cin >> sigma;
	cout << "T(1):";
	cin >> T;

	//BSModel bsModel(100, 0.05, 0.2);
	/* initialization of bsmodel */
	BSModel bsModel(S0, r, sigma);

	out << "N-Options\tUpandOutCall\tDownAndOutPut\tDoubleBarrierOutCall\n";

	int NEpts[5] = {100, 500, 1000, 5000, 10000};

	int u=120, d=80, du=120, dd=80;

	/* input parameter: K, up-level, down-level */
	cout << "Enter barrier option parameters.\nEnter strike price K(100.0): "; 
	cin >> K;

	cout << "Enter up-barrier for UpandOutCall(120):"; 
	cin >> u;

	cout << "Enter down-barrier for DownandOutPut(80):";
	cin >> d;

	cout << "Enter up and down barrier form DoubleBarrierOutCall(120 80):"; 
	cin >> du >> dd;

	/* constuct up-call, down-put, double-barrier */
	UpandOutCall uoc(u, 100);
	DownandOutPut dop(d, 100);
	DoubleBarrierOutCall dboc(du, dd, 100);

	uoc.SetMT(T/12.0, 30);
	dop.SetMT(T/12.0, 30);
	dboc.SetMT(T/12.0, 30);

	/* loop calculation for diff steps given by file */
	for (int i = 0; i < 5; i++)
	{
		cout << "Enter steps to expiry N:";
		cin >> NEpts[i];

		out << NEpts[i] << "\t" << uoc.PriceByMC(bsModel, NEpts[i]) << "\t"
			<< dop.PriceByMC(bsModel, NEpts[i]) << "\t"
			<< dboc.PriceByMC(bsModel, NEpts[i]) << endl;
	}

	out.close();

	return 0;
}

