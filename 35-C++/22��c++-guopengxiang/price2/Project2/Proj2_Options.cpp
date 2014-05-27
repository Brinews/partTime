#include "Proj2_Options.h"


#include "TriModel01.h"
#include "TriLattice02.h"

#include <iostream>

#include <vector>
#include <cmath>

using namespace std;

/* EurOption Model */
double EurOption::PriceByCRR(TriModel Model,
							vector<double>&Greeks)
{
	int N=GetN();
	//Model.Setdt(1.0, N);

	/**
	  risk-neutral probability of up-movement 
	  , down-movement
	  and middle-movement
	  */
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	//double q=Model.RiskNeutProb();
	/**
	  array to contain option prices
	  */
	vector<double> Price(2*N+1);

	/**
	  payoffs calculation 
	  */
	for (int i=0; i<=2*N; i++)
	{
		Price[i]=Payoff(Model.S(N,i-N));
	}

	//cal the e^(-1*r*dt)
	double h = exp(-1*Model.GetR()*Model.Getdt());

	double H11, H1N1, H10, H00;
	double S11, S1N1, S10;

	

	/**
	  nest-loop: using backward procedure to calculate
	  the option fair price at time step n to node i
	  */
	for (int n=N-1; n>=0; n--)
	{
		for (int i=0; i<=2*n; i++)
		{
			//Price[i] = h*(pu*Price[i+1]+pm*Price[i]+pd*Price[i-1]);
			Price[i] = h*(pu*Price[i+2]+pm*Price[i+1]+pd*Price[i]);
		}

		if (1 == n)
		{
			H11 = Price[2];
			H1N1 = Price[0];
			H10 = Price[1];
		}

		if (0 == n)
		{
			H00 = Price[0];
		}
	}

	double delta, gamma, theta;

	S11 = Model.S(1, 1);
	S1N1 = Model.S(1, -1);
	S10 = Model.S(1, 0);

	/**
	  calculate the delta, gamma, theta 
	  by formula (3), (4), (5)
	  */
	delta = (H11 - H1N1) / (S11 - S1N1);
	gamma = ( ((H11-H10)/(S11-S10))-((H10-H1N1)/(S10-S1N1)) )
			/ (0.5*(S11-S1N1));
	theta = (H10-H00)/Model.Getdt();

	Greeks.clear();

	/* save them to vector */
	Greeks.push_back(delta);
	Greeks.push_back(gamma);
	Greeks.push_back(theta);

	cout << "PriceByMC: \n";
	cout << delta << " " << gamma << " " << theta << endl;
	cout << Price[0] << endl;

	return Price[0];
}

/* amoption model */
double AmOption::PriceBySnell(TriModel Model,
		TriLattice<double>& PriceTree,
		vector<double>& Greeks,
		vector<double>& EarlyBoundary)
{
	int N=GetN();
	//Model.Setdt(1.0, N);

	/* up, down, middle movement calculation */
	double pu = Model.RiskNeutProb_up();
	double pd = Model.RiskNeutProb_down();
	double pm = 1 - pu - pd;

	/* set N */
	PriceTree.SetN(N);

	/* clear the early boundary */
	EarlyBoundary.clear();

	double ContVal;
	/* loop for early boundary initialization */
	for (int i=0; i<=2*N; i++)
	{
		double pf = Payoff(Model.S(N, i-N));
		PriceTree.SetNode(N,i,pf);
		EarlyBoundary.push_back(1);
	}

	//cal the basic parameter: e^(-1*r*dt)
	double h = exp(-1*Model.GetR()*Model.Getdt());

	double H11, H1N1, H10, H00;
	double S11, S1N1, S10;

	/* calculate the S(1, 1), S(1, -1), S(1, 0) */
	S11 = Model.S(1, 1);
	S1N1 = Model.S(1, -1);
	S10 = Model.S(1, 0);

	/* nest loop for snell model */
	for (int n=N-1; n>=0; n--)
	{
		for (int i=0; i<=2*n; i++)
		{
			/* by the formula */
			ContVal = (pu*PriceTree.GetNode(n+1, i+2)
					  +pm*PriceTree.GetNode(n+1, i+1)
					  +pd*PriceTree.GetNode(n+1, i));

			ContVal = h*ContVal;

			/* calculate the payoff */
			double Sni = Payoff(Model.S(n,i-n));

			/* set pricetree */
			PriceTree.SetNode(n, i, Sni);

			//StoppingTree.SetNode(n,i,1);
			if (ContVal>Sni)
			{
				/* if the calculate result big than the thresh,
				  setting the early boundary 
				 */
				EarlyBoundary[i] = ContVal;

				PriceTree.SetNode(n,i,ContVal);
				//StoppingTree.SetNode(n,i,0);
			}
			else if (PriceTree.GetNode(n,i)==0.0)
			{
				//StoppingTree.SetNode(n,i,0);
			}

			/* calculate the H(1, 1), H(1, -1), H(1, 0)
			   */
			if (1 == n)
			{
				H11 = PriceTree.GetNode(1, 2);
				H1N1 = PriceTree.GetNode(1, 0);
				H10 = PriceTree.GetNode(1, 1);
			}

			/* calculate the H(0, 0)
			   */
			if (0 == n)
			{
				H00 = PriceTree.GetNode(0, 0);
			}
		}
	}

	double delta, gamma, theta;

	/* calculate the delta, gamma, theta
	   by formula(3), (4), (5)
	   */
	delta = (H11 - H1N1) / (S11 - S1N1);
	gamma = ( ((H11-H10)/(S11-S10))-((H10-H1N1)/(S10-S1N1)) )
			/ (0.5*(S11-S1N1));
	theta = (H10-H00)/Model.Getdt();

	Greeks.clear();
	/* save the delta, gamma, theta */
	Greeks.push_back(delta);
	Greeks.push_back(gamma);
	Greeks.push_back(theta);

	cout << "PriceBySnell: \n";
	cout << delta << "=" << gamma << "=" << theta << endl;
	cout << PriceTree.GetNode(0,0) << endl;

	/* return the inital price */
	return PriceTree.GetNode(0,0);
}

/* the payoff by Call model */
double Call::Payoff(double z)
{
	if (z>K) return z-K;
	return 0.0;
}

/* the payoff by Put model */
double Put::Payoff(double z)
{
	if (z<K) return K-z;
	return 0.0;
}
