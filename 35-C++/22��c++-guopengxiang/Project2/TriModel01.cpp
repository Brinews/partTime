#include "TriModel01.h"

#include <iostream>
#include <cmath>

using namespace std;

// member function to work out time interval dt
// T maturity of the option, N number of time steps
void TriModel::Setdt(double T, int N)
{
	if (0 == N) {
		cerr << "N Should Not Be 0, Or Divide Zero Error" << endl;
		return;
	}

	dt = T / N;
}

// calculate the risk neutual probability of going up
double TriModel::RiskNeutProb_up()
{
	double dx = sigma * sqrt(3 * dt);
	double sigma_p2 = sigma * sigma;
	double v = R - q - 0.5 * sigma_p2;
	double v_p2 = v*v;
	double dx_p2 = dx * dx;
	double dt_p2 = dt * dt;

	double pu = 0.5 * ((sigma_p2*dt + v_p2*dt_p2)/dx_p2 + (v*dt)/dx );

	return pu;
}

// calculate the risk neutual probability of going down
double TriModel::RiskNeutProb_down()
{
	double dx = sigma * sqrt(3 * dt);
	double sigma_p2 = sigma * sigma;
	double v = R - q - 0.5 * sigma_p2;
	double v_p2 = v*v;
	double dx_p2 = dx *dx;
	double dt_p2 = dt * dt;

	double pd = 0.5 * ((sigma_p2*dt + v_p2*dt_p2)/dx_p2 - (v*dt)/dx );

	return pd;
}

// calculate the stock price at time step n and node i
double TriModel::S(int n, int i)
{
	double mdt = 1.0 / n;
	double dx = sigma * sqrt(3 * mdt);

	double S = S0 * exp(i * dx);

	return S;
}

// return the risk free interest rate R
double TriModel::GetR()
{
	return R;
}

// return time interval dt
double TriModel::Getdt()
{
	return dt;
}

