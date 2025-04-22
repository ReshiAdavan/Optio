#include "options/barrierOption.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

//
// ---- BarrierOption Implementation ----
//

BarrierOption::BarrierOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall)
    : Option(currentVal, strike, vol, divRate, rfRate, time, isCall), barrierPrice(barrier) {}

double BarrierOption::getBarrierPrice() const {
    return barrierPrice;
}

void BarrierOption::setBarrierPrice(double barrier) {
    barrierPrice = barrier;
}

//
// ---- BarrierDownIn Implementation ----
//

BarrierDownIn::BarrierDownIn(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall)
    : BarrierOption(currentVal, strike, vol, divRate, rfRate, time, barrier, isCall) {}

double BarrierDownIn::monteCarlo_barrierOption_downIn(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        bool crossed = false;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            if (St <= barrierPrice) crossed = true;
        }

        if (crossed) {
            option_sum += std::max(0.0, (isCallOption ? 1 : -1) * (St - strikePrice));
        }
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

//
// ---- BarrierUpIn Implementation ----
//

BarrierUpIn::BarrierUpIn(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall)
    : BarrierOption(currentVal, strike, vol, divRate, rfRate, time, barrier, isCall) {}

double BarrierUpIn::monteCarlo_barrierOption_upIn(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        bool crossed = false;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            if (St >= barrierPrice) crossed = true;
        }

        if (crossed) {
            option_sum += std::max(0.0, (isCallOption ? 1 : -1) * (St - strikePrice));
        }
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

//
// ---- BarrierDownOut Implementation ----
//

BarrierDownOut::BarrierDownOut(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall)
    : BarrierOption(currentVal, strike, vol, divRate, rfRate, time, barrier, isCall) {}

double BarrierDownOut::monteCarlo_barrierOption_downOut(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        bool crossed = false;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            if (St <= barrierPrice) crossed = true;
        }

        if (!crossed) {
            option_sum += std::max(0.0, (isCallOption ? 1 : -1) * (St - strikePrice));
        }
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

//
// ---- BarrierUpOut Implementation ----
//

BarrierUpOut::BarrierUpOut(double currentVal, double strike, double vol, double divRate, double rfRate, double time, double barrier, bool isCall)
    : BarrierOption(currentVal, strike, vol, divRate, rfRate, time, barrier, isCall) {}

double BarrierUpOut::monteCarlo_barrierOption_upOut(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        bool crossed = false;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            if (St >= barrierPrice) crossed = true;
        }

        if (!crossed) {
            option_sum += std::max(0.0, (isCallOption ? 1 : -1) * (St - strikePrice));
        }
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

//
// ---- Display Info for All ----
//

void BarrierDownIn::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}

void BarrierUpIn::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}

void BarrierDownOut::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}

void BarrierUpOut::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}
