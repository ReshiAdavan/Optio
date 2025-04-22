#ifndef CLI_HPP
#define CLI_HPP

#include "options/baseOption.hpp"
#include <string>
#include <map>
#include <functional>
#include <unordered_map>
#include <memory>

using validationCondition = std::function<bool(const std::string&)>;

class CommandLineInterface {
private:
    std::unordered_map<std::string, std::unique_ptr<Option>> optionsMap;
    std::unordered_map<std::string, std::unique_ptr<FloatingOption>> floatingOptionsMap;

public:
    // Parameter collection
    std::unique_ptr<Option> takeFixedOptionValues(int optionType);
    std::unique_ptr<FloatingOption> takeFloatingOptionValues(int optionType);

    // I/O utilities
    void getPricingDetails(double& simulations, double& timeSteps);
    std::string nameOption(const std::string& optionType);
    void displayOptionInfo(const std::string& optionName);
    void displayMenu();
    void createOption(const std::string& optionType);
};

#endif
