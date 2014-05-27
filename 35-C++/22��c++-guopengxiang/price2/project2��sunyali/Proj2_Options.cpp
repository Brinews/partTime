#include "Proj2_Options.h"
#include "TriModel01.h"
#include "TriLattice02.h"

#include <iostream>
#include <cmath>
using namespace std;

double EurOption::PriceByCRR(TriModel Model, vector<double>& Greeks)
{
   double pu=Model.RiskNeutProb_up();//get the probability of going up
   double pd=Model.RiskNeutProb_down();//get the probability of going down
   double pm=1-pu-pd;//get the probability of unchanged
   int N=GetN();
   vector<double> Price(2*N+1);
   double h11,h10,h1_1;
   for (int i=0;i<=2*N;i++)
   {
       Price[i]=Payoff(Model.S(N,i-N));
   }
   for (int n=N-1;n>=0;n--)//calculate price at step N-1,N-2,N-3.....0
  {
      for (int i=0;i<=2*n;i++)
      {
          Price[i]=(pu*Price[i+2]+pm*Price[i+1]+pd*Price[i])*exp(-Model.GetR()*Model.Getdt());//using risk neurtal probability to calculate Price
      }
      if (n==1)
      {
          h1_1=Price[0];h10=Price[1];h11=Price[2];
      }
  }
  //vector<double> Greeks(6);
 // cout<<"h11:"<<h11<<"h10:"<<h10<<"h1_1:"<<h1_1<<endl;
  Greeks[0]=(h11-h1_1)/(Model.S(1,1)-Model.S(1,-1));//delta
  Greeks[1]=2*((h11-h10)/(Model.S(1,1)-Model.S(1,0))-(h10-h1_1)/(Model.S(1,0)-Model.S(1,-1)))/(Model.S(1,1)-Model.S(1,-1));//gamma
  double t = Model.S(1, 1);
  Greeks[2]=(h10-Price[0])/(Model.Getdt());//theta
  double t = Model.S(1, 1);
  //Greeks under GBM:

  double t = Model.S(1, 1);
  return  Price[0];//get fair price
}


double AmOption::PriceBySnell(TriModel Model, TriLattice<double>& PriceTree, vector<double>& Greeks, vector<double>& EarlyBoundary)
{
    double pu=Model.RiskNeutProb_up();//get the probability of going up
    double pd=Model.RiskNeutProb_down();//get the probability of going down
    double pm=1-pu-pd;//get the probability of unchanged
    int N=GetN();
    PriceTree.SetN(N);
    double ContVal;
    double h11,h10,h1_1;
    for (int i=0; i<=2*N; i++)
    {
        PriceTree.SetNode(N,i,Payoff(Model.S(N,i-N)));
    }
    for (int n=N-1; n>=0; n--)
    {
        for (int i=0; i<=2*n; i++)
        {
            ContVal=(pu*PriceTree.GetNode(n+1,i+2)+pm*PriceTree.GetNode(n+1,i+1)+pd*PriceTree.GetNode(n+1,i))*exp(-Model.GetR()*Model.Getdt());
            PriceTree.SetNode(n,i,Payoff(Model.S(n,i-n)));
            if (ContVal>PriceTree.GetNode(n,i))
            {
                PriceTree.SetNode(n,i,ContVal);
            }
        }
        if (n==1)
        {
             h1_1=PriceTree.GetNode(1,0);h10=PriceTree.GetNode(1,1);h11=PriceTree.GetNode(1,2);
        }
    }
    //cout<<"h11:"<<h11<<"h10:"<<h10<<"h1_1:"<<h1_1<<endl;
    Greeks[0]=(h11-h1_1)/(Model.S(1,1)-Model.S(1,-1));//delta
    Greeks[1]=2*((h11-h10)/(Model.S(1,1)-Model.S(1,0))-(h10-h1_1)/(Model.S(1,0)-Model.S(1,-1)))/(Model.S(1,1)-Model.S(1,-1));//gamma
    Greeks[2]=(h10-PriceTree.GetNode(0,0))/(Model.Getdt());//theta
    return PriceTree.GetNode(0,0);

}

double Call::Payoff(double z)
{
    if (z>K) return z-K;
    return 0.0;
}

double Put::Payoff(double z)
{
    if (z<K) return K-z;
    return 0.0;
}
