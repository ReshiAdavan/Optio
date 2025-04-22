#pragma once
#include "baseOption.hpp"

class BarrierOption : public Option {
protected:
    double barrierPrice;

public:
    BarrierOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall);
    double getBarrierPrice() const;
    void setBarrierPrice(double barrier);
};

class BarrierDownIn : public BarrierOption {
public:
    BarrierDownIn(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall);
    double monteCarlo_barrierOption_downIn(int timeSteps, int simulations);
    void displayInfo() override;
};

class BarrierUpIn : public BarrierOption {
public:
    BarrierUpIn(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall);
    double monteCarlo_barrierOption_upIn(int timeSteps, int simulations);
    void displayInfo() override;
};

class BarrierDownOut : public BarrierOption {
public:
    BarrierDownOut(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall);
    double monteCarlo_barrierOption_downOut(int timeSteps, int simulations);
    void displayInfo() override;
};

class BarrierUpOut : public BarrierOption {
public:
    BarrierUpOut(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall);
    double monteCarlo_barrierOption_upOut(int timeSteps, int simulations);
    void displayInfo() override;
};
