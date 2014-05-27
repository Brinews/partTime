#include "TriModel01.h"
#include "Proj1_Options06.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//the function of pricing the initial value of a European Option using CCR procidure
double EurOption::PriceByCRR(TriModel Model)
{
    double Pu=Model.RiskNeutProb_up();//risk-neutral probability of up-movement
    double Pd=Model.RiskNeutProb_down();//risk-neutral probability of down-movement
    double Pm=1-Pu-Pd;//risk-neutral probability of middle-movement
    vector<double> Price(2*N+1);//the array used to contain option prices at eath time step
    //the for-loop used to calculate the terminal time step option payoffs
    for(int i=0;i<=2*N;i++)
    {
        Price[i]=Payoff(Model.S(N,i));
    }
    //the nest for-loop using backward procedure to calculate the option fair price at time step n and node i
    for(int n=N-1;n>=0;n--)
    {
        for(int i=0;i<=2*n;i++)
        {
            Price[i]=exp(-1*Model.GetR()*Model.Getdt())*(Pu*Price[i+2]+Pm*Price[i+1]+Pd*Price[i]);
        }
    }
    return Price[0];//return the initial value of the option price
}

//input the call option data of expiry N and strike price K
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

//the payoff function of the call option
double Call::Payoff(double z)
{
   if (z>K) return z-K;
   return 0.0;
}

//input the put option data of expiry N and Strick price K
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

//the payoff function of the put option
double Put::Payoff(double z)
{
   if (z<K) return K-z;
   return 0.0;
}
