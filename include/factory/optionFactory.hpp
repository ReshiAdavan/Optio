#pragma once
#include <memory>
#include "options/baseOption.hpp"

class OptionFactory {
public:
    static std::unique_ptr<Option> createFixedOption(
        int optionType,
        double S, double K, double vol, double div, double r, double T,
        double barrier, bool isCall
    );

    static std::unique_ptr<FloatingOption> createFloatingOption(
        int optionType,
        double S, double vol, double div, double r, double T,
        bool isCall
    );
};
