#ifndef Barrier01_h
#define Barrier01_h

#include "TriModel01.h"

extern void BarrierOptionPrice(int , char *);

class BarrierOption
{
    private:
        int N; // number of time steps to maturity
    public:
        // set time steps
        void setN(int N_){N = N_;}

        // return time steps
        int getN(){return N;}

        // function to calculate the payoff of the option
        virtual double Payoff(double z) = 0;

        // function to calculate the barrier option price by
        // a Trinomial Tree Model
        virtual double PriceByTree(TriModel Model) = 0;
};

class UpAndOutCall: public BarrierOption
{
    private:
        double barrier; // upper barrier
        double K;      // strike of the option
    public:
        // contructor
        UpAndOutCall(double barrier_,double K_,int N_){barrier = barrier_; K = K_; setN(N_);}

        // please implement the payoff function for the up-and-out call
        double Payoff(double z);

        // please implement pricing function for the up-and-out call
        double PriceByTree(TriModel Model);
};

class DownAndOutPut: public BarrierOption
{
    private:
        double barrier; // lower barrier
        double K;       // strike of the option
    public:
        // constructor
        DownAndOutPut(double barrier_,double K_,int N_){barrier = barrier_; K = K_; setN(N_);}

        // please implement the payoff function for the down-and-out put
        double Payoff(double z);

        // please implement the pricing function for the down-and-out put
        double PriceByTree(TriModel Model);
};


class DoubleBarrierOutCall : public BarrierOption
{
    private:
        double upper_barrier;  // upper barrier
        double lower_barrier;  // lower barrier
        double K;              // strike of the option
    public:
        // Constructor
        DoubleBarrierOutCall(double ub,double lb,double K_,int N_)
        {
            upper_barrier = ub;
            lower_barrier = lb;
            K = K_;
            setN(N_);
        }

        // please implement the payoff function for the double barrier call
        double Payoff(double z);

        // please implement the pricing function for the double barrier call
        double PriceByTree(TriModel model);
};

#endif

