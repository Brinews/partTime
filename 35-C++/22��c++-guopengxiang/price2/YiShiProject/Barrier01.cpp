#include "TriModel01.h"
#include "Barrier01.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//the payoff function of the Up-And-Out barrier call option
double UpAndOutCall::Payoff(double z)
{
    if(z>K) return z-K;
    return 0.0;
}

//the backward procedure used to price the barrier option prices at time step n and node i,
//finally return the initial value of the Up-And-Out barrier call option
double UpAndOutCall::PriceByTree(TriModel Model)
{
    double Pu=Model.RiskNeutProb_up();//the risk neutral probability of up-movement of stock price
    double Pd=Model.RiskNeutProb_down();//the risk neutral probability of down-movement of stock price
    double Pm=1-Pu-Pd;//the risk neutral probability of middle-movement of stock price
    int N=getN();//since expiry date N is the private variable of class Barrier Option.So need getN() function to get N
    vector<double> Price(2*N+1);//the array used to contain option prices at eath time step
    //double Price[2*N+1];//define the array to contain the price of barrier option at time step n and node i

    //the for-loop to calculate prices of the barrier option at the expiry date N and node i
    for(int i=0;i<=2*N;i++)
    {
        if(Model.S(N,i)<barrier)
        {
            Price[i]=Payoff(Model.S(N,i));
        }else{Price[i]=0;}
    }

    //the nest for-loop to calculate prices of the barrier option at the time step n and node i
    for(int n=N-1;n>=0;n--)
    {
        for(int i=0;i<=2*n;i++)
        {
            if(Model.S(n,i)<barrier)
            {
                Price[i]=exp(-Model.GetR()*Model.Getdt())*(Pu*Price[i+2]+Pm*Price[i+1]+Pd*Price[i]);
            }else{Price[i]=0;}
        }
    }
    return Price[0];//return the initial price of the barrier option
}

//the payoff function of the Down-And-Out barrier put option
double DownAndOutPut::Payoff(double z)
{
    if(z<K) return K-z;
    return 0.0;
}

//the backward procedure used to price the barrier option prices at time step n and node i,
//finally return the initial value of the Down-And-Out barrier put option
double DownAndOutPut::PriceByTree(TriModel Model)
{
    double Pu=Model.RiskNeutProb_up();//the risk neutral probability of up-movement of stock price
    double Pd=Model.RiskNeutProb_down();//the risk neutral probability of down-movement of stock price
    double Pm=1-Pu-Pd;//the risk neutral probability of middle-movement of stock price
    int N=getN();//since expiry date N is the private variable of class Barrier Option.So need getN() function to get N
    //double Price[2*N+1];//define the array to contain the price of barrier option at time step n and node i
    vector<double> Price(2*N+1);//the array used to contain option prices at eath time step

    //the for-loop to calculate prices of the barrier option at the expiry date N and node i
    for(int i=0;i<=2*N;i++)
    {
        if(Model.S(N,i)>barrier)
        {
            Price[i]=Payoff(Model.S(N,i));
        }else{Price[i]=0;}
    }

    //the nest for-loop to calculate prices of the barrier option at the time step n and node i
    for(int n=N-1;n>=0;n--)
    {
        for(int i=0;i<=2*n;i++)
        {
            if(Model.S(n,i)>barrier)
            {
                Price[i]=exp(-Model.GetR()*Model.Getdt())*(Pu*Price[i+2]+Pm*Price[i+1]+Pd*Price[i]);
            }else{Price[i]=0;}
        }
    }
    return Price[0];//return the initial price of the barrier option
}

//the payoff function of the Double-Barrier-Out barrier call option
double DoubleBarrierOutCall::Payoff(double z)
{
    if(z>K) return z-K;
    return 0.0;
}

//the backward procedure used to price the barrier option prices at time step n and node i,
//finally return the initial value of the Double-Barrier-Out barrier call option
double DoubleBarrierOutCall::PriceByTree(TriModel Model)
{
    double Pu=Model.RiskNeutProb_up();//the risk neutral probability of up-movement of stock price
    double Pd=Model.RiskNeutProb_down();//the risk neutral probability of down-movement of stock price
    double Pm=1-Pu-Pd;//the risk neutral probability of middle-movement of stock price
    int N=getN();//since expiry date N is the private variable of class Barrier Option.So need getN() function to get N
    //double Price[2*N+1];//define the array to contain the price of barrier option at time step n and node i
    vector<double> Price(2*N+1);//the array used to contain option prices at eath time step

    //the for-loop to calculate prices of the barrier option at the expiry date N and node i
    for(int i=0;i<=2*N;i++)
    {
        if(Model.S(N,i)<upper_barrier&&Model.S(N,i)>lower_barrier)
        {
            Price[i]=Payoff(Model.S(N,i));
        }else{Price[i]=0;}
    }

    //the nest for-loop to calculate prices of the barrier option at the time step n and node i
    for(int n=N-1;n>=0;n--)
    {
        for(int i=0;i<=2*n;i++)
        {
            if(Model.S(n,i)<upper_barrier&&Model.S(n,i)>lower_barrier)
            {
                Price[i]=exp(-Model.GetR()*Model.Getdt())*(Pu*Price[i+2]+Pm*Price[i+1]+Pd*Price[i]);
            }else{Price[i]=0;}
        }
    }
    return Price[0];//return the initial price of the barrier option
}

