#include "TriLattice02.h"
#include "TriModel01.h"
#include "Proj2_Options.h"
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    double S,q,sigma,r,T,N,K;
    cout <<"Please input S0: "; cin>>S;
    cout <<"Please input q: "; cin>>q;
    cout <<"Please input sigma: "; cin>>sigma;
    cout <<"Please input r: "; cin>>r;
    cout <<"please input expiry date T: "; cin>>T;
    cout <<"please input time steps N: ";cin>>N;
    cout <<"Please input strike price K: ";cin>>K;
    TriModel Model(S,q,sigma,r);
    Model.Setdt(1.0,N);
    Call Option1;
    vector<double> Greeks(6);
    Option1.SetN(N);
    Option1.SetK(K);
    Option1.PriceByCRR(Model,Greeks);
    cout <<Option1.PriceByCRR(Model,Greeks)<<endl;
    cout <<"Greeks for European call option:"<< endl;
    cout <<"detla: "<< Greeks[0]<< endl;
    cout <<"gamma: "<< Greeks[1]<< endl;
    cout <<"theta: "<< Greeks[2]<< endl;
    TriLattice<double> PriceTree;
    vector<double> EarlyBoundary;
    Option1.PriceBySnell(Model,PriceTree,Greeks,EarlyBoundary);
    cout <<Option1.PriceBySnell(Model,PriceTree,Greeks,EarlyBoundary)<<endl;
    cout <<"Greeks for Americon call option:"<< endl;
    cout <<"detla: "<< Greeks[0]<< endl;
    cout <<"gamma: "<< Greeks[1]<< endl;
    cout <<"theta: "<< Greeks[2]<< endl;

	system("pause");

return 0;

}
