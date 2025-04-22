#pragma once
#include "baseOption.hpp"

class EuropeanOption : public Option {
public:
    EuropeanOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall);

    double blackScholes_europeanOption();
    double monteCarlo_europeanOption(int timeSteps, int simulations);
    double monteCarlo_europeanOption_heston(int timeSteps, int simulations, double kappa, double theta, double sigma, double rho);
    void calculateGreeks();
    void displayInfo() override;
};
