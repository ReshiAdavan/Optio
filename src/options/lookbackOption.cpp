#include "options/lookbackOption.hpp"
#include "utils/utilFunctions.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

//
// ---- LookbackOptionFixed Implementation ----
//

LookbackOptionFixed::LookbackOptionFixed(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall)
    : Option(currentVal, strike, vol, divRate, rfRate, time, isCall) {}

double LookbackOptionFixed::monteCarlo_lookbackOption_fixed(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double erddt = std::exp((riskFreeRate - dividendRate) * dt);
    double egamma = std::exp((2 * (riskFreeRate - dividendRate) + volatility * volatility) * dt) - 2 * erddt + 1;
    double beta1 = -1;
    double beta2 = -0.5;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St1 = underlyingPrice, St2 = underlyingPrice;
        double cv1 = 0.0, cv2 = 0.0;
        double St1_min = St1, St1_max = St1;
        double St2_min = St2, St2_max = St2;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            double t = timeToExpiry - i * dt;

            double delta1 = UtilFunctions::delta(St1, strikePrice, riskFreeRate, dividendRate, volatility, t, isCallOption);
            double delta2 = UtilFunctions::delta(St2, strikePrice, riskFreeRate, dividendRate, volatility, t, isCallOption);
            double gamma1 = UtilFunctions::gamma(St1, strikePrice, riskFreeRate, dividendRate, volatility, t, isCallOption);
            double gamma2 = UtilFunctions::gamma(St2, strikePrice, riskFreeRate, dividendRate, volatility, t, isCallOption);

            double Stn1 = St1 * std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            double Stn2 = St2 * std::exp(nu * dt + volatility * std::sqrt(dt) * -z);

            cv1 += delta1 * (Stn1 - St1 * erddt) + delta2 * (Stn2 - St2 * erddt);
            cv2 += gamma1 * ((Stn1 - St1) * (Stn1 - St1) - St1 * St1 * egamma) +
                   gamma2 * ((Stn2 - St2) * (Stn2 - St2) - St2 * St2 * egamma);

            St1 = Stn1;
            St2 = Stn2;
            St1_min = std::min(St1, St1_min);
            St1_max = std::max(St1, St1_max);
            St2_min = std::min(St2, St2_min);
            St2_max = std::max(St2, St2_max);
        }

        double payoff1 = isCallOption ? std::max(0.0, St1_max - strikePrice) : std::max(0.0, strikePrice - St1_min);
        double payoff2 = isCallOption ? std::max(0.0, St2_max - strikePrice) : std::max(0.0, strikePrice - St2_min);

        double option_T = 0.5 * (payoff1 + beta1 * cv1 + payoff2 + beta2 * cv2);
        option_sum += option_T;
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

void LookbackOptionFixed::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}

//
// ---- LookbackOptionFloating Implementation ----
//

LookbackOptionFloating::LookbackOptionFloating(double currentVal, double vol, double divRate, double rfRate, double time, bool isCall)
    : FloatingOption(currentVal, vol, divRate, rfRate, time, isCall) {}

double LookbackOptionFloating::monteCarlo_lookbackOption_floating(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double St_min = St, St_max = St;

        for (int i = 0; i < timeSteps; ++i) {
            double z = dist(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            St_min = std::min(St, St_min);
            St_max = std::max(St, St_max);
        }

        double payoff = isCallOption ? St - St_min : St_max - St;
        option_sum += payoff;
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

void LookbackOptionFloating::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}
