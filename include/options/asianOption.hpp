#pragma once
#include "baseOption.hpp"

class AsianOptionFixed : public Option {
public:
    AsianOptionFixed(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall);

    double monteCarlo_asianOption_geometric(int timeSteps, int simulations);
    double monteCarlo_asianOption_arithmetic(int timeSteps, int simulations);
    double monteCarlo_asianOption_geometricControlVariate(int timeSteps, int simulations);
    void displayInfo() override;
};

class AsianOptionFloating : public FloatingOption {
public:
    AsianOptionFloating(double currentVal, double vol, double divRate, double rfRate, double time, bool isCall);

    double monteCarlo_asianOption_geometric(int timeSteps, int simulations);
    double monteCarlo_asianOption_arithmetic(int timeSteps, int simulations);
    double monteCarlo_asianOption_geometricControlVariate(int timeSteps, int simulations);
    void displayInfo() override;
};
