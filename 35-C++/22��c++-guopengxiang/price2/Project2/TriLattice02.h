#ifndef TriLattice02_h
#define TriLattice02_h

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

/**
 * TriLattice fomular */
template<typename Type> class TriLattice
{
	private:
		int N;
		/* two dimension array to save the lattice value result */
		vector< vector<Type> > Lattice;
	public:
		/* set N */
		void SetN(int N_)
		{
			N=N_;
			/* dymanic change the size */
			Lattice.resize(N+1);

			for(int n=0; n<=N; n++) Lattice[n].resize(2*N+1);
		}

		/* set node value by index */
		void SetNode(int n, int i, Type x)
		{
			Lattice[n][i]=x;
		}

		/* get node value by index */
		Type GetNode(int n, int i)
		{
			return Lattice[n][i];
		}

		void Display()
		{
			/* output by given format */
			cout << setiosflags(ios::fixed)
				<< setprecision(3);

			for(int n=0; n<=N; n++)
			{
				for(int i=0; i<=N; i++)
					cout << setw(7) << GetNode(n,i);
				cout << endl;
			}
			cout << endl;
		}
};

#endif

