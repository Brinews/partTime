#pragma once


const double EPSINON = 1.0E-100;
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)


extern double  g_Boundary[][2];


double rnd_uni();
double Crand(double low, double high);
double PI();


double Shifted_Sphere(const double* X , int Dim);			// f1
double Shifted_Schwefel(const double* X , int Dim);		    // f2
double Shifted_Rosenbrock(const double* X , int Dim);		// f3
double Shifted_Rastrigin(const double* X , int Dim);		// f4
double Shifted_Griewank(const double* X , int Dim);			// f5
double Shifted_Ackley(const double* X , int Dim);			// f6
double f_Schwefel2_22(const double* X , int Dim);			// f7			
double f_Schwefel1_2(const double* X , int Dim);			// f8
double Extended_f_10(const double* X , int Dim);			// f9
double f_Bohachevsky(const double* X , int Dim);			// f10
