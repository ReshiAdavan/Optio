#ifndef CLI_HPP
#define CLI_HPP

#include "options/baseOption.hpp"
#include <string>
#include <map>
#include <functional>

using validationCondition = std::function<bool(const std::string&)>;

class CommandLineInterface {
private:
    std::map<std::string, Option*> optionsMap;
    std::map<std::string, FloatingOption*> floatingOptionsMap;

public:
    // Factory method
    Option* createOptionFromType(int optionType,
                                 double underlyingPrice,
                                 double strikePrice,
                                 double volatility,
                                 double dividendRate,
                                 double interestRate,
                                 double timeToExpiration,
                                 double barrier,
                                 bool isCallOption);

    // Parameter collection
    Option* takeFixedOptionValues(int optionType);
    FloatingOption* takeFloatingOptionValues(int optionType);

    // I/O utilities
    void getPricingDetails(double& simulations, double& timeSteps);
    std::string nameOption(const std::string& optionType);
    void displayOptionInfo(const std::string& optionName);
    void displayMenu();
    void createOption(const std::string& optionType);
};

#endif
