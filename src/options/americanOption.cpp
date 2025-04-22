#include "options/americanOption.hpp"
#include "utils/utilFunctions.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

//
// ---- AmericanOption Implementation ----
//

AmericanOption::AmericanOption(double currentVal, double strike, double vol, double divRate, double rfRate, double time, bool isCall)
    : Option(currentVal, strike, vol, divRate, rfRate, time, isCall), timeStepsNumber(100) {}

double AmericanOption::crr_americanOption(int timeSteps) {
    timeStepsNumber = timeSteps;
    double dt = timeToExpiry / timeSteps;

    double nu = (riskFreeRate - dividendRate - 0.5 * volatility * volatility);
    double lambda = sqrt(3.0 / 2.0);  // Kamrad and Ritchken convergence
    double pu = 1 / (2 * lambda * lambda) + 0.5 * (nu / (lambda * volatility)) * sqrt(dt);
    double pd = 1 / (2 * lambda * lambda) - 0.5 * (nu / (lambda * volatility)) * sqrt(dt);
    double pm = 1 - pu - pd;

    double u = exp(lambda * volatility * sqrt(dt));

    std::vector<std::vector<double>> S(timeSteps + 1, std::vector<double>(2 * timeSteps + 1, 0.0));
    std::vector<std::vector<double>> C(timeSteps + 1, std::vector<double>(2 * timeSteps + 1, 0.0));

    for (int i = timeSteps; i >= 0; i--) {
        for (int j = -i; j <= i; j++) {
            S[i][j + timeSteps] = underlyingPrice * pow(u, j);
        }
    }

    for (int j = -timeSteps; j <= timeSteps; j++) {
        C[timeSteps][j + timeSteps] = std::max(0.0, ((isCallOption ? 1 : -1) * (S[timeSteps][j + timeSteps] - strikePrice)));
    }

    for (int i = timeSteps - 1; i >= 0; i--) {
        for (int j = i; j >= -i; j--) {
            int idx = j + timeSteps;
            double expected = exp(-riskFreeRate * dt) * (
                pu * C[i + 1][idx + 1] +
                pm * C[i + 1][idx] +
                pd * C[i + 1][idx - 1]
            );
            double intrinsic = (isCallOption ? 1 : -1) * (S[i][idx] - strikePrice);
            C[i][idx] = std::max(expected, intrinsic);
        }
    }

    optionPrice = C[0][timeSteps];
    return optionPrice;
}


double AmericanOption::ls_americanOption(int timeSteps) {
    if (timeSteps % 2 == 0) timeSteps += 1;
    timeStepsNumber = timeSteps;
    double dt = timeToExpiry / timeSteps;

    double d1 = UtilFunctions::d1_(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility, dividendRate);
    double d2 = UtilFunctions::d2_(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility, dividendRate);
    double pu = UtilFunctions::peizer_pratt_cdf(d2, timeSteps);
    double pd = UtilFunctions::peizer_pratt_cdf(d1, timeSteps);

    double u = std::exp((riskFreeRate - dividendRate) * dt) * (pd / pu);
    double d = std::exp((riskFreeRate - dividendRate) * dt) * ((1 - pd) / (1 - pu));

    std::vector<std::vector<double>> S(timeSteps+1, std::vector<double>(timeSteps+1));
    std::vector<std::vector<double>> C(timeSteps+1, std::vector<double>(timeSteps+1));

    for (int i = 0; i <= timeSteps; i++) {
        for (int j = 0; j <= i; j++) {
            S[i][j] = underlyingPrice * std::pow(u, j) * std::pow(d, i - j);
        }
    }

    for (int j = 0; j <= timeSteps; j++) {
        C[timeSteps][j] = std::max(0.0, ((isCallOption ? 1 : -1) * (S[timeSteps][j] - strikePrice)));
    }

    for (int i = timeSteps-1; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            double hold = std::exp(-riskFreeRate * dt) * (pu * C[i+1][j+1] + (1 - pu) * C[i+1][j]);
            double exercise = ((isCallOption ? 1 : -1) * (S[i][j] - strikePrice));
            C[i][j] = std::max(hold, exercise);
        }
    }

    optionPrice = C[0][0];

    delta = (C[1][1] - C[1][0]) / (S[1][1] - S[1][0]);
    gamma = (((C[2][2] - C[2][1]) / (S[2][2] - S[2][1])) - ((C[2][1] - C[2][0]) / (S[2][1] - S[2][0]))) / (0.5 * (S[2][2] - S[2][0]));

    return optionPrice;
}

void AmericanOption::calculateGreeks() {
    double dV = 0.0001 * volatility;
    double dR = 0.0001 * riskFreeRate;
    double dT = 0.0001 * timeToExpiry;

    double vUp = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility + dV, dividendRate, timeStepsNumber, isCallOption);
    double vDn = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate, volatility - dV, dividendRate, timeStepsNumber, isCallOption);
    vega = (vUp - vDn) / (2 * dV);

    double rUp = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate + dR, volatility, dividendRate, timeStepsNumber, isCallOption);
    double rDn = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry, riskFreeRate - dR, volatility, dividendRate, timeStepsNumber, isCallOption);
    rho = (rUp - rDn) / (2 * dR);

    double tUp = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry + dT, riskFreeRate, volatility, dividendRate, timeStepsNumber, isCallOption);
    double tDn = UtilFunctions::lr_americanOption_forGreeks(underlyingPrice, strikePrice, timeToExpiry - dT, riskFreeRate, volatility, dividendRate, timeStepsNumber, isCallOption);
    theta = (tUp - tDn) / (2 * dT);
}

void AmericanOption::displayInfo() {
    std::cout << "Option Price is: " << getOptionPrice() << std::endl;
    std::cout << "Option Delta is: " << getDelta() << std::endl;
    std::cout << "Option Gamma is: " << getGamma() << std::endl;
    std::cout << "Option Vega is: "  << getVega()  << std::endl;
    std::cout << "Option Rho is: "   << getRho()   << std::endl;
    std::cout << "Option Theta is: " << getTheta() << std::endl;
}
