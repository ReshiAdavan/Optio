#include "factory/optionFactory.hpp"
#include "options/europeanOption.hpp"
#include "options/americanOption.hpp"
#include "options/asianOption.hpp"
#include "options/lookbackOption.hpp"
#include "options/barrierOption.hpp"

std::unique_ptr<Option> OptionFactory::createFixedOption(int optionType, double S, double K, double vol, double div, double r, double T, double barrier, bool isCall) {
    switch (optionType) {
        case 1: return std::make_unique<EuropeanOption>(S, K, vol, div, r, T, isCall);
        case 2: return std::make_unique<AmericanOption>(S, K, vol, div, r, T, isCall);
        case 3: return std::make_unique<AsianOptionFixed>(S, K, vol, div, r, T, isCall);
        case 4: return std::make_unique<LookbackOptionFixed>(S, K, vol, div, r, T, isCall);
        case 5: return std::make_unique<BarrierDownIn>(S, K, vol, div, r, T, barrier, isCall);
        case 6: return std::make_unique<BarrierUpIn>(S, K, vol, div, r, T, barrier, isCall);
        case 7: return std::make_unique<BarrierDownOut>(S, K, vol, div, r, T, barrier, isCall);
        case 8: return std::make_unique<BarrierUpOut>(S, K, vol, div, r, T, barrier, isCall);
        default: return nullptr;
    }
}

std::unique_ptr<FloatingOption> OptionFactory::createFloatingOption(int optionType, double S, double vol, double div, double r, double T, bool isCall) {
    switch (optionType) {
        case 1: return std::make_unique<AsianOptionFloating>(S, vol, div, r, T, isCall);
        case 2: return std::make_unique<LookbackOptionFloating>(S, vol, div, r, T, isCall);
        default: return nullptr;
    }
}
