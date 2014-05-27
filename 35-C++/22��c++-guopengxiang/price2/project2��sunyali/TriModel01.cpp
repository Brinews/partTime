#include "TriModel01.h"
#include <iostream>
#include <cmath>
using namespace std;

void TriModel::Setdt(double T, int N)
{
    dt=T/N;//caculate time interval delta(t),do not return anything
}

double TriModel::RiskNeutProb_up()//calculate the probability of going up
{
    double dx,v,a,b;//calculate everthing according to the formula given
    dx=sigma*sqrt(3*dt);
    v=R-q-sigma*sigma/2;
    a=(sigma*sigma*dt+v*v*dt*dt)/(dx*dx);
    b=(v*dt)/dx;
    return (a+b)/2;
}

double TriModel::RiskNeutProb_down()//calculate the probability of going down
{
    double dx,v,a,b;
    dx=sigma*sqrt(3*dt);
    v=R-q-sigma*sigma/2;
    a=(sigma*sigma*dt+v*v*dt*dt)/(dx*dx);
    b=(v*dt)/dx;
    return (a-b)/2;
}

double TriModel::S(int n, int i)//calculate the European option price at time step n and node i
{
    double dx;
    dx=sigma*sqrt(3*dt);
    return S0*exp(i*dx);
}

double TriModel::GetR()//return the value of R
{
    return R;
}

double TriModel::Getdt()//return the value of dt
{
    return dt;
}
