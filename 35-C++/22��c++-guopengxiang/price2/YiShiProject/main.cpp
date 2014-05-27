#include "TriModel01.h"
#include "Proj1_Options06.h"
#include "Barrier01.h"
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

int main()
{

    //used to output the prices of the european call option of different initial prices and length of time steps to an Exel File
    //called EurCallPrices.xls
    ofstream out1;
    out1.open("EurCallPrices.xls");
    Call Option1;
    out1<<"N\\S"<<'\t';
    for(double S0=80;S0<=120;S0=S0+10)
    {
        out1<<S0<<'\t';
    }
    out1<<endl;
    for(int i=0;i<=3;i++)
    {
        Option1.GetInputData();
        out1<<Option1.getN()<<'\t';
        for(double S0=80;S0<=120;S0=S0+10)
        {
            TriModel Model1(S0,0.03,0.2,0.05);
            Model1.Setdt(1.0,Option1.getN());
            cout<<"when N = "<<Option1.getN()<<" and initial stock price = "<<S0<<endl<<"European Call Option1 Price = "<<Option1.PriceByCRR(Model1)
            <<endl<<endl;
            out1<<Option1.PriceByCRR(Model1)<<'\t';
        }
        out1<<endl;
    }

    //used to output the prices of the european put option of different initial prices and length of time steps to an Exel File
    //called EurPutPrices.xls
    ofstream out2;
    out2.open("EurPutPrices.xls");
    Put Option2;
    out2<<"N\\S"<<'\t';
    for(double S0=80;S0<=120;S0=S0+10)
    {
        out2<<S0<<'\t';
    }
    out2<<endl;
    for(int i=0;i<=3;i++)
    {
        Option2.GetInputData();
        out2<<Option2.getN()<<'\t';
        for(double S0=80;S0<=120;S0=S0+10)
        {
            TriModel Model2(S0,0.03,0.2,0.05);
            Model2.Setdt(1.0,Option2.getN());
            cout<<"when N = "<<Option2.getN()<<" and initial stock price = "<<S0<<endl<<"European Put Option2 Price = "<<Option2.PriceByCRR(Model2)
            <<endl<<endl;
            out2<<Option2.PriceByCRR(Model2)<<'\t';
        }
        out2<<endl;
    }

    //used to output the prices of different kind of barrier options of different length of time steps to an Exel File
    //called BarrierPrices.xls
    ofstream out3;
    out3.open("BarrierPrice.xls");
    out3<<"N\\Options"<<'\t'<<"UpAndOutCall"<<'\t'<<"DownAndOutPut"<<'\t'<<"DoubleBarrierOutCall"<<endl;
    for(int i=0;i<=4;i++)
    {
        int N;
        cout<<"Enter steps to expiry N: ";cin>>N;
        out3<<N<<'\t';
        TriModel Model3(100,0.03,0.2,0.05);
        Model3.Setdt(1,N);
        UpAndOutCall Option3(120,100,N);
        cout<<"When N = "<<N<<" And The Initial Stock Price = 100"<<endl<<"UpAndOutCall Option3 Price = "<<Option3.PriceByTree(Model3)
        <<endl<<endl;
        out3<<Option3.PriceByTree(Model3)<<'\t';
        DownAndOutPut Option4(80,100,N);
        cout<<"When N = "<<N<<" And The Initial Stock Price = 100"<<endl<<"DownAndOutPut Option4 Price = "<<Option4.PriceByTree(Model3)
        <<endl<<endl;
        out3<<Option4.PriceByTree(Model3)<<'\t';
        DoubleBarrierOutCall Option5(120,80,100,100);
        cout<<"When N = "<<N<<" And The Initial Stock Price = 100"<<endl<<"DoubleBarrierOutCall Option5 Price = "<<Option5.PriceByTree(Model3)
        <<endl<<endl;
        out3<<Option5.PriceByTree(Model3)<<endl;
    }
    return 0;
}
