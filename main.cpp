#include <iostream>
#include <map>
#include <string>
#include <functional>

#include "options/option_class.hpp"
#include "options/vanilla_options.cpp"
#include "options/exotic_options.cpp"
#include "utils/utility_functions.hpp"

using namespace std;
using validationCondition = std::function<bool(const std::string&)>;

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

class CommandLineInterface {
private:
    map<string, Option*> optionsMap;
    map<string, FloatingOption*> floatingOptionsMap;

public:
    Option* createOptionFromType(int optionType, double underlyingPrice, double strikePrice, double volatility, 
                             double dividendRate, double interestRate, double timeToExpiration, 
                             double barrier, bool isCallOption) {
    switch(optionType) {
        case 1: return new EuropeanOption(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
        case 2: return new AmericanOption(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
        case 3: return new AsianOptionFixed(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
        case 4: return new LookbackOptionFixed(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
        case 5: return new BarrierDownIn(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, barrier, isCallOption);
        case 6: return new BarrierUpIn(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, barrier, isCallOption);
        case 7: return new BarrierDownOut(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, barrier, isCallOption);
        case 8: return new BarrierUpOut(underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, barrier, isCallOption);
        default: return new Option(0,0,0,0,0,0,0);
    }
}

    Option* takeFixedOptionValues(int optionType) {
        double underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration;
        bool isCallOption;
        string input;

        underlyingPrice = getValidatedInput("Enter underlying price: ", isValidNumber);
        strikePrice = getValidatedInput("Enter strike price: ", isValidNumber);
        volatility = getValidatedInput("Enter volatility (as a decimal fraction representing percentage): ", isValidNumber);
        dividendRate = getValidatedInput("Enter dividend rate (as a decimal fraction representing percentage): ", isValidNumber);
        interestRate = getValidatedInput("Enter interest rate (as a decimal fraction representing percentage): ", isValidNumber);
        timeToExpiration = getValidatedInput("Enter time to expiration (as a decimal fraction/or multiple of a year): ", isValidNumber);
        isCallOption = getValidatedInput("Is it a call or put option? (1 for Call, 0 for Put): ", isOneOrZero);

        double barrier = 0;
        if (optionType >= 5 && optionType <= 8) { 
            barrier = getValidatedInput("Enter barrier price: ", isValidNumber);
        }

        return createOptionFromType(optionType, underlyingPrice, strikePrice, volatility, dividendRate, interestRate, timeToExpiration, barrier, isCallOption);
    }

    FloatingOption* takeFloatingOptionValues(int optionType) {
        double underlyingPrice, volatility, dividendRate, interestRate, timeToExpiration;
        bool isCallOption;

        underlyingPrice = getValidatedInput("Enter underlying price: ", isValidNumber);
        volatility = getValidatedInput("Enter volatility (as a decimal fraction representing percentage): ", isValidNumber);
        dividendRate = getValidatedInput("Enter dividend rate (as a decimal fraction representing percentage): ", isValidNumber);
        interestRate = getValidatedInput("Enter interest rate (as a decimal fraction representing percentage): ", isValidNumber);
        timeToExpiration = getValidatedInput("Enter time to expiration (as a decimal fraction/or multiple of a year): ", isValidNumber);
        isCallOption = getValidatedInput("Is it a call or put option? (1 for Call, 0 for Put): ", isOneOrZero);

        switch(optionType) {
            case 1: return new AsianOptionFloating(underlyingPrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
            case 2: return new LookbackOptionFloating(underlyingPrice, volatility, dividendRate, interestRate, timeToExpiration, isCallOption);
            default: return new FloatingOption(0,0,0,0,0,0);
        }
    }

    void displayMenu() {
        cout << "Select an option type:" << endl;
        cout << "1. European" << endl;
        cout << "2. American" << endl;
        cout << "3. Asian" << endl;
        cout << "4. Lookback" << endl;
        cout << "5. Barrier" << endl;
        cout << "6. Exit to Main Menu" << endl;

        int choice;
        cin >> choice;

        switch(choice) {
            case 1:
                createOption("European");
                break;
            case 2:
                createOption("American");
                break;
            case 3:
                cout << "Does your Asian option have fixed or floating strike?" << endl;
                cout << "1. Fixed" << endl;
                cout << "2. Floating" << endl;

                int choiceAsian;
                cin >> choiceAsian;

                switch(choiceAsian) {
                    case 1:
                        createOption("Asian Fixed");
                        break;
                    case 2:
                        createOption("Asian Floating");
                        break;
                    default:
                        cout << "Invalid Asian option type!" << endl;
                }
                break;
            case 4:
                cout << "Does your Lookback option have fixed or floating strike?" << endl;
                cout << "1. Fixed" << endl;
                cout << "2. Floating" << endl;

                int choiceFloating;
                cin >> choiceFloating;

                switch(choiceFloating) {
                    case 1:
                        createOption("Lookback Fixed");
                        break;
                    case 2:
                        createOption("Lookback Floating");
                        break;
                    default:
                        cout << "Invalid Lookback option type!" << endl;
                }
                break;
            case 5:
                cout << "Is your Barrier option down-in, up-in, down-out, up-out?" << endl;
                cout << "1. Down & In" << endl;
                cout << "2. Up & In" << endl;
                cout << "3. Down & Out" << endl;
                cout << "4. Up & Out" << endl;

                int choiceBarrier;
                cin >> choiceBarrier;

                switch(choiceBarrier) {
                    case 1:
                        createOption("Barrier Down-In");
                        break;
                    case 2:
                        createOption("Barrier Up-In");
                        break;
                    case 3:
                        createOption("Barrier Down-Out");
                        break;
                    case 4:
                        createOption("Barrier Up-Out");
                        break;
                    default:
                        cout << "Invalid Barrier option type!" << endl;
                }
                break;
            case 6:
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    void getPricingDetails(double& simulations, double& timesteps) {
        cout << "Please input the number of simulations and time-steps you would like to use:" << endl;
        cout << "simulations (10,000 is recommended): ";
        cin >> simulations;
        cout << "time-steps: ";
        cin >> timesteps;
    }

    string nameOption(const string& optionType) {
        string optionName;
        cout << "Enter a name for this " << optionType << " option: ";
        cin >> optionName;

        while (optionsMap.find(optionName) != optionsMap.end()) {
            cout << "This name is already used. Please enter a unique name: ";
            cin >> optionName;
        }

        return optionName;
    }

    void createOption(string optionType) {
        if (optionType == "European") {
            cout << "European option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(1);
            EuropeanOption* europeanOption = static_cast<EuropeanOption*>(option);

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
            optionsMap[optionName] = europeanOption;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "American") {
            cout << "American option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(2);
            AmericanOption* americanOption = static_cast<AmericanOption*>(option);

            double timesteps;
            cout << "As default, a Leisen-Reimer binary tree pricer will be used, please input the number of time-steps you would like to use:" << endl;
            cout << "time-steps: ";
            cin >> timesteps;
            double dummy = americanOption->ls_americanOption(timesteps);

            cout << "Option priced successfully!" << endl;

            americanOption->calculateGreeks();
            string optionName = nameOption(optionType);
            optionsMap[optionName] = americanOption;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Asian Fixed") {
            cout << "Asian Fixed Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(3);
            AsianOptionFixed* asianOptionFixed = static_cast<AsianOptionFixed*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = asianOptionFixed->monteCarlo_asianOption_geometricControlVariate(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = asianOptionFixed;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Asian Floating") {
            cout << "Asian Floating Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            FloatingOption* option = takeFloatingOptionValues(1);
            AsianOptionFloating* asianOptionFloating = static_cast<AsianOptionFloating*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = asianOptionFloating->monteCarlo_asianOption_geometricControlVariate(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            floatingOptionsMap[optionName] = asianOptionFloating;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Lookback Fixed") {
            cout << "Lookback Fixed Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(4);
            LookbackOptionFixed* lookbackFixed = static_cast<LookbackOptionFixed*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = lookbackFixed->monteCarlo_lookbackOption_fixed(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = lookbackFixed;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Lookback Floating") {
            cout << "Lookback Floating Strike option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            FloatingOption* option = takeFloatingOptionValues(2);
            LookbackOptionFloating* lookbackFloating = static_cast<LookbackOptionFloating*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = lookbackFloating->monteCarlo_lookbackOption_floating(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            floatingOptionsMap[optionName] = lookbackFloating;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Barrier Down-In") {
            cout << "Barrier Down-In option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(5);
            BarrierDownIn* barrierDownIn = static_cast<BarrierDownIn*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = barrierDownIn->monteCarlo_barrierOption_downIn(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = barrierDownIn;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Barrier Up-In") {
            cout << "Barrier Up-In option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(6);
            BarrierUpIn* barrierUpIn = static_cast<BarrierUpIn*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = barrierUpIn->monteCarlo_barrierOption_upIn(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = barrierUpIn;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Barrier Down-Out") {
            cout << "Barrier Down-Out option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(7);
            BarrierDownOut* barrierDownOut = static_cast<BarrierDownOut*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = barrierDownOut->monteCarlo_barrierOption_downOut(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = barrierDownOut;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else if (optionType == "Barrier Up-Out") {
            cout << "Barrier Up-Out option selected, fill in the appropriate parameters for the option one-by-one:" << endl;
            
            Option* option = takeFixedOptionValues(8);
            BarrierUpOut* barrierUpOut = static_cast<BarrierUpOut*>(option);

            double timesteps;
            double simulations;
            cout << "As default, a Monte Carlo pricer will be used." << endl;
            getPricingDetails(simulations, timesteps);
            double dummy = barrierUpOut->monteCarlo_barrierOption_upOut(timesteps, simulations);

            cout << "Option priced successfully!" << endl;

            string optionName = nameOption(optionType);
            optionsMap[optionName] = barrierUpOut;
            cout << "Option priced successfully and logged under the name -> " << optionName << endl;
        }
        else {
            cout << "Invalid option type!" << endl;
        }
    }

    void displayOptionInfo(string optionName) {
        if (optionsMap.find(optionName) != optionsMap.end()) {
            optionsMap[optionName]->displayInfo();
        }
        else if (floatingOptionsMap.find(optionName) != floatingOptionsMap.end()) {
            floatingOptionsMap[optionName]->displayInfo();
        }
        else {
            cout << "Option not found!" << endl;
        }
    }
};

int main() {
    CommandLineInterface cli;
    int choice;
    do {
        cout << "Welcome to Optio!" << endl;
        cout << "1. Create Option" << endl;
        cout << "2. Generate & Display Option Information" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                cli.displayMenu();
                break;
            case 2: {
                string optionName;
                cout << "Enter the name of the option: ";
                cin >> optionName;
                cli.displayOptionInfo(optionName);
                break;
            }
            case 3:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while(choice != 3);

    return 0;
}
