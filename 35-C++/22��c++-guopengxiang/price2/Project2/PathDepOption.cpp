#include "PathDepOption.h"
#include <cmath>

/********************UpAndOutCall*********************/

/* common function PriceByMc */
double PathDepOption::PriceByMC(BSModel Model, long N)
{
	double H=0.0;
	/* initialization for samplePath */
	SamplePath S(m);

	/* loop calculation the H */
	for(long i=0; i<N; i++)
	{
		/* get the sample Path */
		Model.GenerateSamplePath(T,m,S);

		/* calculate the payoff by samplepath */
		double pf = Payoff(S);

		H = (i * H + pf)/(i+1.0);
	}

	return exp(-Model.r * T) * H;
}

/* upandoutcall model's payoff */
double UpandOutCall::Payoff(SamplePath& S)
{
	double Ave=0.0;

	/* the formula realization */
	for (int k=0; k<m; k++) 
		Ave=(k * Ave+S[k])/(k+1.0);

	if (Ave<K) return 0.0;

	return Ave-K;
}

/*******************DownAndOutCall*********************/

/*
double DownandOutPut::PriceByMC(BSModel Model, long N)
{
	double H=0.0;
	SamplePath S(m);
	for(long i=0; i<N; i++)
	{
		Model.GenerateSamplePath(T,m,S);
		double pf = Payoff(S);
		if (pf <= barrier)
		{
			pf = 0;
		}

		H = (i * H + pf)/(i+1.0);
	}

	return exp(-Model.r * T) * H;
}
*/

/* down-and-output model's payoff */
double DownandOutPut::Payoff(SamplePath& S)
{
	double Ave=0.0;
	/* loop calculation */
	for (int k=0; k<m; k++) 
		Ave=(k * Ave+S[k])/(k+1.0);

	if (Ave>K) return 0.0;

	return K-Ave;
}

/*******************DoubleBarrierOutCall*********************/

/*
double DoubleBarrierOutCall::PriceByMC(BSModel Model, long N)
{
	double H=0.0;
	SamplePath S(m);
	for(long i=0; i<N; i++)
	{
		Model.GenerateSamplePath(T,m,S);

		double pf = Payoff(S);

		if (pf >= upper_barrier || pf <= lower_barrier)
			pf = 0;

		H = (i * H + pf)/(i+1.0);
	}

	return exp(-Model.r * T) * H;
}
*/

/* double-barrier model's payoff */
double DoubleBarrierOutCall::Payoff(SamplePath& S)
{
	double Ave=0.0;
	for (int k=0; k<m; k++) 
		Ave=(k * Ave+S[k])/(k+1.0);

	if (Ave<K) return 0.0;

	return Ave-K;
}

