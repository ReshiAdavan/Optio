#pragma once
#include "baseOption.hpp"

class AmericanOption : public Option {
protected:
    double timeStepsNumber;

public:
    AmericanOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall);

    double crr_americanOption(int timeSteps);
    double ls_americanOption(int timeSteps);
    void calculateGreeks();
    void displayInfo() override;
};
