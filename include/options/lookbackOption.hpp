#pragma once
#include "baseOption.hpp"

class LookbackOptionFixed : public Option {
public:
    LookbackOptionFixed(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall);

    double monteCarlo_lookbackOption_fixed(int timeSteps, int simulations);
    void displayInfo() override;
};

class LookbackOptionFloating : public FloatingOption {
public:
    LookbackOptionFloating(double currentVal, double vol, double divRate, double rfRate, double time, bool isCall);

    double monteCarlo_lookbackOption_floating(int timeSteps, int simulations);
    void displayInfo() override;
};
