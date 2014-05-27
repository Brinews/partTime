#include "TriModel01.h"
#include <iostream>
#include <cmath>
using namespace std;

// the function of setting the length of the time step
void TriModel::Setdt(double T,int N)
{
    dt=T/N;
}

//the function of the risk neutral probability of up-movement
double TriModel::RiskNeutProb_up()
{
    double dx=sigma*pow(3*dt,0.5);
    double v=R-q-0.5*sigma*sigma;
    double Pu=0.5*((sigma*sigma*dt+v*v*dt*dt)/(dx*dx)+v*dt/dx);
    return Pu;
}

//the function of the risk neutral probability of down-movement
double TriModel::RiskNeutProb_down()
{
    double dx=sigma*pow(3*dt,0.5);
    double v=R-q-0.5*sigma*sigma;
    double Pd=0.5*((sigma*sigma*dt+v*v*dt*dt)/(dx*dx)-v*dt/dx);
    return Pd;
}

//the function of stock price at the time step n and node i
double TriModel::S(int n,int i)
{
    double dx=sigma*pow(3*dt,0.5);
    return S0*exp((i-n)*dx);
}

//the function of getting risk-free interest rate
double TriModel::GetR()
{
    return R;
}

//the function of getting the length of the time step
double TriModel::Getdt()
{
    return dt;
}

