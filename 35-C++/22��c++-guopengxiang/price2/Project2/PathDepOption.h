#ifndef PathDepOption_h
#define PathDepOption_h

#include "BSModel01.h"

class PathDepOption
{
   public:
      double T, Price, PricingError;
      int m;

	  /* the common function :
		 PriceByMC, through Model to get price */
      double PriceByMC(BSModel Model, long N);

	  /* initialize T and m */
	  void SetMT(double T_, int m_)
	  {
		  T = T_;
		  m = m_;
	  }

	  /* interface : payoff caculation */
      virtual double Payoff(SamplePath& S)=0;
};

class UpandOutCall: public PathDepOption
{
    public:
        double K;
        double barrier;
        // Please implement a constructor for UpandOutcall:
		UpandOutCall(double b_, double k_):barrier(b_), K(k_)
		{
		}

        double Payoff(SamplePath& S);
};

class DownandOutPut: public PathDepOption
{
    public:
        double K;
        double barrier;
        // Please implement a constructor for a DownandOutPut:

		DownandOutPut(double b_, double k_):barrier(b_),K(k_)
		{
		}

        double Payoff(SamplePath& S);
};

class DoubleBarrierOutCall: public PathDepOption
{
    public:
        double K;
        double upper_barrier;  // upper barrier
        double lower_barrier;
        // Please implement a constructor for a DoubleBarrierOutCall:
		DoubleBarrierOutCall(double u_b, double l_b, double k)
			:upper_barrier(u_b), lower_barrier(l_b), K(k)
		{
		}

        double Payoff(SamplePath& S);
};

#endif
