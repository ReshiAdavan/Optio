#include "options/europeanOption.hpp"
#include "utils/utilFunctions.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

//
// ---- EuropeanOption Implementation ----
//

EuropeanOption::EuropeanOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall)
    : Option(currentVal, strike, vol, divRate, rfRate, time, isCall) {}

double EuropeanOption::blackScholes_europeanOption() {
    double d1 = UtilFunctions::d1_(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility, dividendRate);
    double d2 = UtilFunctions::d2_(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility, dividendRate);

    if (isCallOption) {
        optionPrice = std::exp(-riskFreeRate * timeToExpiry) *
            (underlyingPrice * std::exp((riskFreeRate - dividendRate) * timeToExpiry) * UtilFunctions::cdf(d1) - 
             strikePrice * UtilFunctions::cdf(d2));
    } else {
        optionPrice = std::exp(-riskFreeRate * timeToExpiry) *
            (strikePrice * UtilFunctions::cdf(-d2) -
             underlyingPrice * std::exp((riskFreeRate - dividendRate) * timeToExpiry) * UtilFunctions::cdf(-d1));
    }
    return optionPrice;
}

double EuropeanOption::monteCarlo_europeanOption(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = (riskFreeRate - dividendRate - 0.5 * volatility * volatility);
    double erddt = std::exp((riskFreeRate - dividendRate) * dt);
    double egamma = std::exp((2 * (riskFreeRate - dividendRate) + volatility * volatility) * dt) - 2 * erddt + 1;
    double beta1 = -1;
    double beta2 = -0.5;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St1 = underlyingPrice;
        double St2 = underlyingPrice;
        double cv1 = 0.0;
        double cv2 = 0.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = distribution(gen);
            double delta1 = UtilFunctions::delta(St1, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double delta2 = UtilFunctions::delta(St2, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double gamma1 = UtilFunctions::gamma(St1, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double gamma2 = UtilFunctions::gamma(St2, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);

            double Stn1 = St1 * std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            double Stn2 = St2 * std::exp(nu * dt + volatility * std::sqrt(dt) * (-z));

            cv1 += delta1 * (Stn1 - St1 * erddt) + delta2 * (Stn2 - St2 * erddt);
            cv2 += gamma1 * ((Stn1 - St1)*(Stn1 - St1) - St1*St1*egamma) +
                   gamma2 * ((Stn2 - St2)*(Stn2 - St2) - St2*St2*egamma);

            St1 = Stn1;
            St2 = Stn2;
        }

        double payoff1 = std::max(0.0, (isCallOption ? 1 : -1) * (St1 - strikePrice));
        double payoff2 = std::max(0.0, (isCallOption ? 1 : -1) * (St2 - strikePrice));
        option_sum += 0.5 * (payoff1 + beta1 * cv1 + payoff2 + beta2 * cv2);
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

double EuropeanOption::monteCarlo_europeanOption_heston(int timeSteps, int simulations, double kappa, double theta, double sigma, double rho) {
    double dt = timeToExpiry / timeSteps;
    double erddt = std::exp((riskFreeRate - dividendRate) * dt);
    double beta1 = -1;
    double beta2 = -0.5;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St1 = underlyingPrice;
        double St2 = underlyingPrice;
        double vt1 = volatility;
        double vt2 = volatility;
        double cv1 = 0.0;
        double cv2 = 0.0;

        for (int i = 0; i < timeSteps; ++i) {
            double zS = distribution(gen);
            double zV = distribution(gen);

            double delta1 = UtilFunctions::delta(St1, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double delta2 = UtilFunctions::delta(St2, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double gamma1 = UtilFunctions::gamma(St1, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);
            double gamma2 = UtilFunctions::gamma(St2, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry - i * dt, isCallOption);

            double tempS1 = St1;
            double tempS2 = St2;

            UtilFunctions::hestonDynamics(St1, vt1, riskFreeRate, dividendRate, kappa, theta, sigma, rho, dt, zS, zV);
            UtilFunctions::hestonDynamics(St2, vt2, riskFreeRate, dividendRate, kappa, theta, sigma, rho, dt, -zS, -zV);

            double egamma1 = std::exp((2 * (riskFreeRate - dividendRate) + vt1 * vt1) * dt) - 2 * erddt + 1;
            double egamma2 = std::exp((2 * (riskFreeRate - dividendRate) + vt2 * vt2) * dt) - 2 * erddt + 1;

            cv1 += delta1 * (St1 - tempS1 * erddt) + delta2 * (St2 - tempS2 * erddt);
            cv2 += gamma1 * ((St1 - tempS1)*(St1 - tempS1) - St1*St1*egamma1) +
                   gamma2 * ((St2 - tempS2)*(St2 - tempS2) - St2*St2*egamma2);
        }

        double payoff1 = std::max(0.0, (isCallOption ? 1 : -1) * (St1 - strikePrice));
        double payoff2 = std::max(0.0, (isCallOption ? 1 : -1) * (St2 - strikePrice));
        option_sum += 0.5 * (payoff1 + beta1 * cv1 + payoff2 + beta2 * cv2);
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

void EuropeanOption::calculateGreeks() {
    delta = UtilFunctions::delta(underlyingPrice, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry, isCallOption);
    gamma = UtilFunctions::gamma(underlyingPrice, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry, isCallOption);
    vega  = UtilFunctions::vega(underlyingPrice, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry, isCallOption);
    rho   = UtilFunctions::rho(underlyingPrice, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry, isCallOption);
    theta = UtilFunctions::theta(underlyingPrice, strikePrice, riskFreeRate, dividendRate, volatility, timeToExpiry, isCallOption);
}

void EuropeanOption::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Option Delta is: " << getDelta() << std::endl;
    std::cout << "Option Gamma is: " << getGamma() << std::endl;
    std::cout << "Option Vega is: "  << getVega()  << std::endl;
    std::cout << "Option Rho is: "   << getRho()   << std::endl;
    std::cout << "Option Theta is: " << getTheta() << std::endl;
}
