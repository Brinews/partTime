#ifndef Proj2_Options_h
#define Proj2_Options_h

#include<vector>
#include "TriLattice02.h"
#include "TriModel01.h"

class Option
{
   private:
      int N; //steps to expiry
   public:
   // Please implement a constructor for an option:
      //double detla,gamma,theta;
      void SetN(int N_){N=N_;}
      int GetN(){return N;}
      virtual double Payoff(double z)=0;
};

class EurOption: public virtual Option
{
   public:
      //pricing European option and calculate Greeks
      // including delta, gamma and theta
      double PriceByCRR(TriModel Model,
                        vector<double>& Greeks);
};

class AmOption: public virtual Option
{
   public:
      //pricing American option, calculate Greeks
      // including delta, gamma and theta
      // and calculate early exercise boundary
      double PriceBySnell(TriModel Model,
         TriLattice<double>& PriceTree,
         vector<double>& Greeks,
         vector<double>& EarlyBoundary);
};

class Call: public EurOption, public AmOption
{
   private:
      double K; //strike price
   public:
      //Please implement a contructor for a Call:
     // Call(double S0_, double q_, double sigma_,double R_)
     // {
      //   S0=S0_;q=q_;sigma=sigma_;R=R_;
      //}
      void SetK(double K_){K=K_;}
      double Payoff(double z);
};

class Put: public EurOption, public AmOption
{
   private:
      double K; //strike price
   public:
      //Please implement a contructor for Put:
      //Put(double S0_, double q_, double sigma_,double R_)
      //{
       //  S0=S0_;q=q_;sigma=sigma_;R=R_;
     // }
      void SetK(double K_){K=K_;}
      double Payoff(double z);
};

#endif
