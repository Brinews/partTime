#include "StdAfx.h"
#include "TestProble.h"

#include <windows.h>
#include <math.h>
#include <assert.h>




double g_Boundary[][2] = 
{
	//----------------------------f1-f6
	{-100,		100},
	{-100,		100},
	{-100,		100},
	{-5,		5},
	{-600,		600},
	{-32,		32},
	//-----------------------------f7-f11
	{-10,		10},
	{-65.536,	65.536},
	{-100,		100},
	{-15,		15},
	{-100,		100},
	//--------------------------- f12--f21
	{-100,		100},             //f12
	{-100,		100},			  //f13
	{-5,		5},				  //f14
	{-10,		10},			  //f15
	{-100,		100},			  //f16---------*
	{-10,		10},			  //f17---------*
	{-100,		100},			  //f18
	{-100,		100},			  //f19
	{-5,		5},				  //f20				
	{-10,		10}				  //f21
};



/**C*F****************************************************************
**                                                                  **
** SRC-FUNCTION   :rnd_uni()                                        **
** LONG_NAME      :random_uniform                                   **
** AUTHOR         :(see below)                                      **
**                                                                  **
** DESCRIPTION    :rnd_uni() generates an equally distributed ran-  **
**                 dom number in the interval [0,1]. For further    **
**                 reference see Press, W.H. et alii, Numerical     **
**                 Recipes in C, Cambridge University Press, 1992.  **
**                                                                  **
** FUNCTIONS      :none                                             **
**                                                                  **
** GLOBALS        :none                                             **
**                                                                  **
** PARAMETERS     :*idum    serves as a seed value                  **
**                                                                  **
** PRECONDITIONS  :*idum must be negative on the first call.        **
**                                                                  **
** POSTCONDITIONS :*idum will be changed                            **
**                                                                  **
***C*F*E*************************************************************/

//全局函数产生[0，1]之间的随机数
/*long g_rnd_Seed = timeGetTime();
double rnd_uni()
{
	long j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double temp;

	if (g_rnd_Seed <= 0)
	{
		if (-(g_rnd_Seed) < 1) g_rnd_Seed=1;
		else g_rnd_Seed = -(g_rnd_Seed);
		idum2=(g_rnd_Seed);
		for (j=NTAB+7;j>=0;j--)
		{
			k=(g_rnd_Seed)/IQ1;
			g_rnd_Seed=IA1*(g_rnd_Seed-k*IQ1)-k*IR1;
			if (g_rnd_Seed < 0) g_rnd_Seed += IM1;
			if (j < NTAB) iv[j] = g_rnd_Seed;
		}
		iy=iv[0];
	}
	k=(g_rnd_Seed)/IQ1;
	g_rnd_Seed=IA1*(g_rnd_Seed-k*IQ1)-k*IR1;
	if (g_rnd_Seed < 0) g_rnd_Seed += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = g_rnd_Seed;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;

}*/
/*------End of rnd_uni()--------------------------*/
/*

double PI()
{
	return atan(1.0)*4.0;
}

double Crand(double low, double high)
{
	double K = rnd_uni();
	return low + K * (high - low);
}
*/


// f1
double Shifted_Sphere(const double* X , int Dim)
{
	double FitNess = 0;
	for(int i=0; i<Dim; i++)
	{
		FitNess += X[i]*X[i];
	}
	return FitNess;
}


// f2
double Shifted_Schwefel(const double* X , int Dim)
{
	double FitNess = abs(X[0]);
	for(int i=1; i<Dim; i++)
	{
		FitNess = ( FitNess > abs(X[i]) ) ? FitNess :  abs(X[i]);
	}
	return FitNess; 
}


// f3
double Shifted_Rosenbrock(const double* X , int Dim)
{
	double FitNess = 0;

	for(int i=0; i<Dim-1; i++)
	{
		FitNess = FitNess + 100*( pow((pow(X[i], 2) - X[i+1]), 2) ) + pow(X[i] - 1, 2);
	}

	return FitNess; 
}

// f4
double Shifted_Rastrigin(const double* X , int Dim)
{

	double FitNess = 0;
	for(int i=0; i<Dim; i++)
	{  
		FitNess = FitNess + ( pow(X[i], 2) - 10*cos(2*PI()*X[i]) + 10 );
	}
	return FitNess; 
}


// f5
double Shifted_Griewank(const double* X , int Dim)
{
	double ff1 = 0;
	double ff2 = 1;
	for(int i=0; i<Dim; i++)
	{       
		ff1 = ff1 + ( pow(X[i], 2) / 4000 );
		ff2 = ff2 * ( cos(X[i]/sqrt(i+1.0)) );

	}
	return (ff1 - ff2 + 1); 
}

// f6
double Shifted_Ackley(const double* X , int Dim)
{
	double Sum1 = 0;
	double Sum2 = 0;
	double FitNess = 0;
	for(int i=0; i<Dim; i++)
	{   
		Sum1 = Sum1 + pow(X[i], 2 );
		Sum2 = Sum2 + cos(2*PI()*X[i]);
	}
	FitNess = -20*exp(-0.2*sqrt(Sum1/Dim)) - exp(Sum2/Dim) + 20 + exp(1.0);

	return FitNess; 
}


// f7
double f_Schwefel2_22(const double* X , int Dim) 
{
	double sum, Value, prod;
	double FitNess = 0;

	sum = 0.0;
	prod = 1.0;

	for (int i=0; i<Dim; i++) 
	{
		Value = abs(X[i]);
		sum  += Value;
		prod *= Value;
	}

	FitNess = sum + prod;
	return FitNess;
}


// f8
double f_Schwefel1_2(const double* X , int Dim)
{
	double	FitNess = 0.0, Value = 0.0;
	for (int i=0; i<Dim; i++)
	{  
		Value   += X[i];
		FitNess += Value * Value;
	}

	return FitNess;
}

double f_10(double x, double y){
	double p, z, t;
	p = (x*x + y*y); 
	z = pow(p, 0.25);
	t = sin(50.0*pow(p, 0.1));
	t = t*t + 1.0;    
	return z*t;
}

// f9
double Extended_f_10(const double* X , int Dim)
{
	double	FitNess = 0.0;
	for(int i=0; i<Dim-1; i++)
	{
		FitNess += f_10(X[i], X[i+1]);
	}

	FitNess += f_10(X[Dim-1], X[0]);
	return FitNess;
}

// f10
double f_Bohachevsky(const double* X , int Dim) 
{   
	double	FitNess = 0.0;
	double  Value1 = 0.0;
	double  Value2 = 0.0;

	Value1 = X[0];
	for (int i=1; i<Dim; i++) 
	{
		Value2 = X[i];

		FitNess += Value1 * Value1 + 2.0 * Value2 * Value2;
		FitNess += -0.3 * cos(3.0 * PI() * Value1) -0.4 * cos(4.0 * PI() * Value2) + 0.7;
		
		Value1 = Value2;
	}

	return FitNess;
}


