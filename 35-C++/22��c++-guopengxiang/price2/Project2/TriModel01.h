#ifndef TriModel01_h
#define TriModel01_h

class TriModel
{
    private:
        double S0; // inital stock price
        double q;  // continuous dividend yield
        double sigma;  // volatility
        double R;    // continuous compounding interest rate

        double dt;   // time interval

    public:
        // contructor to initialise the data member by list
        TriModel(double S0_, double q_, double sigma_,double R_):S0(S0_),q(q_),sigma(sigma_),R(R_)
        {

        }

        // member function to work out time interval dt
        // T maturity of the option, N number of time steps
        void Setdt(double T, int N);

        // calculate the risk neutual probability of going up
        double RiskNeutProb_up();

        // calculate the risk neutual probability of going down
        double RiskNeutProb_down();

        // calculate the stock price at time step n and node i
        double S(int n, int i);

        // return the risk free interest rate R
        double GetR();

        // return time interval dt
        double Getdt();

};


#endif
