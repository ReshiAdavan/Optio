#include "options/asianOption.hpp"
#include "utils/utilFunctions.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

//
// ---- AsianOptionFixed Implementation ----
//

AsianOptionFixed::AsianOptionFixed(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall)
    : Option(currentVal, strike, vol, divRate, rfRate, time, isCall) {}

double AsianOptionFixed::monteCarlo_asianOption_geometric(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double product = 1.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            product *= St;
        }

        double geometric_avg = std::pow(product, 1.0 / timeSteps);
        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (geometric_avg - strikePrice)));
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

double AsianOptionFixed::monteCarlo_asianOption_arithmetic(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double sum = 0.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            sum += St;
        }

        double arithmetic_avg = sum / timeSteps;
        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (arithmetic_avg - strikePrice)));
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

double AsianOptionFixed::monteCarlo_asianOption_geometricControlVariate(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double sum = 0.0;
        double product = 1.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            sum += St;
            product *= St;
        }

        double arithmetic_avg = sum / timeSteps;
        double geometric_avg = std::pow(product, 1.0 / timeSteps);

        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (arithmetic_avg - strikePrice))) -
                      std::max(0.0, ((isCallOption ? 1 : -1) * (geometric_avg - strikePrice)));
    }

    double control = this->monteCarlo_asianOption_geometric(timeSteps, simulations);
    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations) + control;
    return optionPrice;
}

void AsianOptionFixed::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}

//
// ---- AsianOptionFloating Implementation ----
//

AsianOptionFloating::AsianOptionFloating(double currentVal, double vol, double divRate, double rfRate, double time, bool isCall)
    : FloatingOption(currentVal, vol, divRate, rfRate, time, isCall) {}

double AsianOptionFloating::monteCarlo_asianOption_geometric(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double product = 1.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            product *= St;
        }

        double geometric_avg = std::pow(product, 1.0 / timeSteps);
        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (St - geometric_avg)));
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

double AsianOptionFloating::monteCarlo_asianOption_arithmetic(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double sum = 0.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            sum += St;
        }

        double arithmetic_avg = sum / timeSteps;
        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (St - arithmetic_avg)));
    }

    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations);
    return optionPrice;
}

double AsianOptionFloating::monteCarlo_asianOption_geometricControlVariate(int timeSteps, int simulations) {
    double dt = timeToExpiry / timeSteps;
    double nu = riskFreeRate - dividendRate - 0.5 * volatility * volatility;
    double option_sum = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(0.0, 1.0);

    for (int j = 0; j < simulations; ++j) {
        double St = underlyingPrice;
        double sum = 0.0;
        double product = 1.0;

        for (int i = 0; i < timeSteps; ++i) {
            double z = norm(gen);
            St *= std::exp(nu * dt + volatility * std::sqrt(dt) * z);
            sum += St;
            product *= St;
        }

        double arithmetic_avg = sum / timeSteps;
        double geometric_avg = std::pow(product, 1.0 / timeSteps);

        option_sum += std::max(0.0, ((isCallOption ? 1 : -1) * (St - arithmetic_avg))) -
                      std::max(0.0, ((isCallOption ? 1 : -1) * (St - geometric_avg)));
    }

    double control = this->monteCarlo_asianOption_geometric(timeSteps, simulations);
    optionPrice = std::exp(-riskFreeRate * timeToExpiry) * (option_sum / simulations) + control;
    return optionPrice;
}

void AsianOptionFloating::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Greeks information currently not available for exotic options, sorry!" << std::endl;
}
