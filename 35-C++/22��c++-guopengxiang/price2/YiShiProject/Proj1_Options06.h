#ifndef Proj1_Options06_h
#define Proj1_Options06_h
#include "TriModel01.h"

class EurOption
{
   private:
      int N; //steps to expiry
   public:
      void SetN(int N_){N=N_;}
      int getN(){return N;}
      //Payoff defined to return 0.0
      //for pedagogical purposes.
      //To use a pure virtual function replace by
      //virtual double Payoff(double z)=0;
      virtual double Payoff(double z){return 0.0;}

      //pricing European option by a Trinomial Tree model
      // please implement this function in the accompany Options06.cpp file
      double PriceByCRR(TriModel Model);
};

class Call: public EurOption
{
   private:
      double K; //strike price
   public:
      void SetK(double K_){K=K_;}
      int GetInputData();
      double Payoff(double z);
};

class Put: public EurOption
{
   private:
      double K; //strike price
   public:
      void SetK(double K_){K=K_;}
      int GetInputData();
      double Payoff(double z);
};

#endif
