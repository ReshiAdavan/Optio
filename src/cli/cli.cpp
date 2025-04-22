#include "cli/cli.hpp"
#include "options/europeanOption.hpp"
#include "options/americanOption.hpp"
#include "options/asianOption.hpp"
#include "options/lookbackOption.hpp"
#include "options/barrierOption.hpp"
#include "options/baseOption.hpp"
#include "factory/optionFactory.hpp"
#include "utils/utilFunctions.hpp"
#include <iostream>
#include <limits>

using namespace std;

// --- Input Validation Helpers ---

bool isValidNumber(const string& input) {
    char* end;
    strtod(input.c_str(), &end);
    return *end == '\0' && !input.empty();
}

bool isOneOrZero(const string& input) {
    return input == "1" || input == "0";
}

double getValidatedInput(const string& prompt, validationCondition validate) {
    string input;
    do {
        cout << prompt;
        cin >> input;
    } while (!validate(input));
    return stod(input);
}

// --- CommandLineInterface Core ---
std::unique_ptr<Option> CommandLineInterface::takeFixedOptionValues(int optionType) {
    double S = getValidatedInput("Enter underlying price: ", isValidNumber);
    double K = getValidatedInput("Enter strike price: ", isValidNumber);
    double vol = getValidatedInput("Enter volatility: ", isValidNumber);
    double div = getValidatedInput("Enter dividend rate: ", isValidNumber);
    double r = getValidatedInput("Enter risk-free rate: ", isValidNumber);
    double T = getValidatedInput("Enter time to expiry: ", isValidNumber);
    bool isCall = static_cast<bool>(getValidatedInput("Is it a call? (1 = Call, 0 = Put): ", isOneOrZero));

    double barrier = 0;
    if (optionType >= 5 && optionType <= 8) {
        barrier = getValidatedInput("Enter barrier price: ", isValidNumber);
    }

    std::unique_ptr<Option> option = OptionFactory::createFixedOption(optionType, S, K, vol, div, r, T, barrier, isCall);
    return option;
}

std::unique_ptr<FloatingOption> CommandLineInterface::takeFloatingOptionValues(int optionType) {
    double S = getValidatedInput("Enter underlying price: ", isValidNumber);
    double vol = getValidatedInput("Enter volatility: ", isValidNumber);
    double div = getValidatedInput("Enter dividend rate: ", isValidNumber);
    double r = getValidatedInput("Enter risk-free rate: ", isValidNumber);
    double T = getValidatedInput("Enter time to expiry: ", isValidNumber);
    bool isCall = static_cast<bool>(getValidatedInput("Is it a call? (1 = Call, 0 = Put): ", isOneOrZero));

    std::unique_ptr<FloatingOption> option = OptionFactory::createFloatingOption(optionType, S, vol, div, r, T, isCall);
    return option;
}

void CommandLineInterface::getPricingDetails(double& simulations, double& timeSteps) {
    simulations = getValidatedInput("Enter number of simulations (e.g., 10000): ", isValidNumber);
    timeSteps = getValidatedInput("Enter number of time steps: ", isValidNumber);
}

string CommandLineInterface::nameOption(const string& optionType) {
    string optionName;
    cout << "Enter a name for this " << optionType << " option: ";
    cin >> optionName;

    while (optionsMap.find(optionName) != optionsMap.end() || floatingOptionsMap.find(optionName) != floatingOptionsMap.end()) {
        cout << "Name already exists. Please enter a unique name: ";
        cin >> optionName;
    }
    return optionName;
}

void CommandLineInterface::displayOptionInfo(const string& optionName) {
    if (optionsMap.count(optionName)) {
        optionsMap[optionName]->displayInfo();
    } else if (floatingOptionsMap.count(optionName)) {
        floatingOptionsMap[optionName]->displayInfo();
    } else {
        cout << "Option not found!" << endl;
    }
}

void CommandLineInterface::displayMenu() {
    std::cout << "\nSelect an option type:" << std::endl;
    std::cout << "1. European" << std::endl;
    std::cout << "2. American" << std::endl;
    std::cout << "3. Asian" << std::endl;
    std::cout << "4. Lookback" << std::endl;
    std::cout << "5. Barrier" << std::endl;
    std::cout << "6. Exit to Main Menu" << std::endl;

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            createOption("European");
            break;

        case 2:
            createOption("American");
            break;

        case 3: {
            std::cout << "Asian option type:\n1. Fixed\n2. Floating" << std::endl;
            int sub;
            std::cin >> sub;
            if (sub == 1) createOption("Asian Fixed");
            else if (sub == 2) createOption("Asian Floating");
            else std::cout << "Invalid Asian option type!" << std::endl;
            break;
        }

        case 4: {
            std::cout << "Lookback option type:\n1. Fixed\n2. Floating" << std::endl;
            int sub;
            std::cin >> sub;
            if (sub == 1) createOption("Lookback Fixed");
            else if (sub == 2) createOption("Lookback Floating");
            else std::cout << "Invalid Lookback option type!" << std::endl;
            break;
        }

        case 5: {
            std::cout << "Barrier type:\n1. Down-In\n2. Up-In\n3. Down-Out\n4. Up-Out" << std::endl;
            int sub;
            std::cin >> sub;
            if (sub == 1) createOption("Barrier Down-In");
            else if (sub == 2) createOption("Barrier Up-In");
            else if (sub == 3) createOption("Barrier Down-Out");
            else if (sub == 4) createOption("Barrier Up-Out");
            else std::cout << "Invalid Barrier option type!" << std::endl;
            break;
        }

        case 6:
            return;

        default:
            std::cout << "Invalid choice!" << std::endl;
    }
}

void CommandLineInterface::createOption(const std::string& optionType) {
    if (optionType == "European") {
        cout << "European option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(1);
        EuropeanOption* europeanOption = static_cast<EuropeanOption*>(option.get());

        cout << "Choose the appropriate choices to price you European option: " << endl;
        cout << "1. Black Scholes pricer" << endl;
        cout << "2. Monte Carlo simulation" << endl;
        int pricer;
        cin >> pricer;

        double dummy;
        switch(pricer) {
            case 1:
                dummy = europeanOption->blackScholes_europeanOption();
                break;
            case 2:
                double timesteps;
                double simulations;
                getPricingDetails(simulations, timesteps);
                dummy = europeanOption->monteCarlo_europeanOption(timesteps, simulations);
                break;
            default:
                cout << "Invalid choice!" << endl;
                return;
        }
        cout << "Option priced successfully!" << endl;

        europeanOption->calculateGreeks();
        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "American") {
        cout << "American option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(2);
        AmericanOption* americanOption = static_cast<AmericanOption*>(option.get());

        double timesteps;
        cout << "As default, a Leisen-Reimer binary tree pricer will be used, please input the number of time-steps you would like to use:" << endl;
        cout << "time-steps: ";
        cin >> timesteps;
        double dummy = americanOption->ls_americanOption(timesteps);

        cout << "Option priced successfully!" << endl;

        americanOption->calculateGreeks();
        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Asian Fixed") {
        cout << "Asian Fixed Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(3);
        AsianOptionFixed* asianOptionFixed = static_cast<AsianOptionFixed*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = asianOptionFixed->monteCarlo_asianOption_geometricControlVariate(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Asian Floating") {
        cout << "Asian Floating Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<FloatingOption> option = takeFloatingOptionValues(1);
        AsianOptionFloating* asianOptionFloating = static_cast<AsianOptionFloating*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = asianOptionFloating->monteCarlo_asianOption_geometricControlVariate(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        floatingOptionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Lookback Fixed") {
        cout << "Lookback Fixed Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(4);
        LookbackOptionFixed* lookbackFixed = static_cast<LookbackOptionFixed*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = lookbackFixed->monteCarlo_lookbackOption_fixed(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Lookback Floating") {
        cout << "Lookback Floating Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<FloatingOption> option = takeFloatingOptionValues(2);
        LookbackOptionFloating* lookbackFloating = static_cast<LookbackOptionFloating*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = lookbackFloating->monteCarlo_lookbackOption_floating(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        floatingOptionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Barrier Down-In") {
        cout << "Barrier Down-In option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(5);
        BarrierDownIn* barrierDownIn = static_cast<BarrierDownIn*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = barrierDownIn->monteCarlo_barrierOption_downIn(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Barrier Up-In") {
        cout << "Barrier Up-In option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(6);
        BarrierUpIn* barrierUpIn = static_cast<BarrierUpIn*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = barrierUpIn->monteCarlo_barrierOption_upIn(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Barrier Down-Out") {
        cout << "Barrier Down-Out option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(7);
        BarrierDownOut* barrierDownOut = static_cast<BarrierDownOut*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = barrierDownOut->monteCarlo_barrierOption_downOut(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else if (optionType == "Barrier Up-Out") {
        cout << "Barrier Up-Out option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
        
        std::unique_ptr<Option> option = takeFixedOptionValues(8);
        BarrierUpOut* barrierUpOut = static_cast<BarrierUpOut*>(option.get());

        double timesteps;
        double simulations;
        cout << "As default, a Monte Carlo pricer will be used." << endl;
        getPricingDetails(simulations, timesteps);
        double dummy = barrierUpOut->monteCarlo_barrierOption_upOut(timesteps, simulations);

        cout << "Option priced successfully!" << endl;

        string optionName = nameOption(optionType);
        optionsMap[optionName] = std::move(option);
        cout << "Option priced successfully and logged under the name -> " << optionName << endl;
    }
    else {
        cout << "Invalid option type!" << endl;
    }
}
