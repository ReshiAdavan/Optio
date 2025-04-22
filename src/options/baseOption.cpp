#include "options/baseOption.hpp"

// --- Option class ---

Option::Option(double currentVal, double strike, double vol, double div, double r, double time, bool isCall)
    : underlyingPrice(currentVal), strikePrice(strike), volatility(vol), dividendRate(div),
      riskFreeRate(r), timeToExpiry(time), isCallOption(isCall) {}

double Option::getUnderlyingPrice() const { return underlyingPrice; }
double Option::getStrikePrice() const { return strikePrice; }
double Option::getVolatility() const { return volatility; }
double Option::getDividendRate() const { return dividendRate; }
double Option::getRiskFreeRate() const { return riskFreeRate; }
double Option::getTimeToExpiry() const { return timeToExpiry; }
bool Option::isCall() const { return isCallOption; }
double Option::getDelta() const { return delta; }
double Option::getGamma() const { return gamma; }
double Option::getVega() const { return vega; }
double Option::getRho() const { return rho; }
double Option::getTheta() const { return theta; }
double Option::getOptionPrice() const { return optionPrice; }

void Option::setUnderlyingPrice(double val) { underlyingPrice = val; }
void Option::setStrikePrice(double strike) { strikePrice = strike; }
void Option::setVolatility(double vol) { volatility = vol; }
void Option::setDividendRate(double divRate) { dividendRate = divRate; }
void Option::setRiskFreeRate(double rfRate) { riskFreeRate = rfRate; }
void Option::setTimeToExpiry(double time) { timeToExpiry = time; }
void Option::setOptionType(bool isCall) { isCallOption = isCall; }

// --- FloatingOption class ---

FloatingOption::FloatingOption(double currentVal, double vol, double div, double r, double time, bool isCall)
    : underlyingPrice(currentVal), volatility(vol), dividendRate(div),
      riskFreeRate(r), timeToExpiry(time), isCallOption(isCall) {}

double FloatingOption::getUnderlyingPrice() const { return underlyingPrice; }
double FloatingOption::getVolatility() const { return volatility; }
double FloatingOption::getDividendRate() const { return dividendRate; }
double FloatingOption::getRiskFreeRate() const { return riskFreeRate; }
double FloatingOption::getTimeToExpiry() const { return timeToExpiry; }
bool FloatingOption::isCall() const { return isCallOption; }
double FloatingOption::getDelta() const { return delta; }
double FloatingOption::getGamma() const { return gamma; }
double FloatingOption::getVega() const { return vega; }
double FloatingOption::getRho() const { return rho; }
double FloatingOption::getTheta() const { return theta; }
double FloatingOption::getOptionPrice() const { return optionPrice; }

void FloatingOption::setUnderlyingPrice(double val) { underlyingPrice = val; }
void FloatingOption::setVolatility(double vol) { volatility = vol; }
void FloatingOption::setDividendRate(double divRate) { dividendRate = divRate; }
void FloatingOption::setRiskFreeRate(double rfRate) { riskFreeRate = rfRate; }
void FloatingOption::setTimeToExpiry(double time) { timeToExpiry = time; }
void FloatingOption::setOptionType(bool isCall) { isCallOption = isCall; }
