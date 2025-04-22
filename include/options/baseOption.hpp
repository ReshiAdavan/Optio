#pragma once

class Option {
protected:
    double underlyingPrice;
    double strikePrice;
    double volatility;
    double dividendRate;
    double riskFreeRate;
    double timeToExpiry;
    bool isCallOption;

    double delta = 0.0;
    double gamma = 0.0;
    double vega = 0.0;
    double rho = 0.0;
    double theta = 0.0;

    double optionPrice = 0.0;

public:
    Option(double currentVal, double strike, double vol, double div, double r, double time, bool isCall);
    
    virtual ~Option() {}
    virtual void displayInfo() = 0;

    double getUnderlyingPrice() const;
    double getStrikePrice() const;
    double getVolatility() const;
    double getDividendRate() const;
    double getRiskFreeRate() const;
    double getTimeToExpiry() const;
    bool isCall() const;
    double getDelta() const;
    double getGamma() const;
    double getVega() const;
    double getRho() const;
    double getTheta() const;
    double getOptionPrice() const;

    void setUnderlyingPrice(double val);
    void setStrikePrice(double strike);
    void setVolatility(double vol);
    void setDividendRate(double divRate);
    void setRiskFreeRate(double rfRate);
    void setTimeToExpiry(double time);
    void setOptionType(bool isCall);
};

class FloatingOption {
protected:
    double underlyingPrice;
    double volatility;
    double dividendRate;
    double riskFreeRate;
    double timeToExpiry;
    bool isCallOption;

    double delta = 0.0;
    double gamma = 0.0;
    double vega = 0.0;
    double rho = 0.0;
    double theta = 0.0;

    double optionPrice = 0.0;

public:
    FloatingOption(double currentVal, double vol, double div, double r, double time, bool isCall);
    
    virtual ~FloatingOption() {}
    virtual void displayInfo() = 0;

    double getUnderlyingPrice() const;
    double getVolatility() const;
    double getDividendRate() const;
    double getRiskFreeRate() const;
    double getTimeToExpiry() const;
    bool isCall() const;
    double getDelta() const;
    double getGamma() const;
    double getVega() const;
    double getRho() const;
    double getTheta() const;
    double getOptionPrice() const;

    void setUnderlyingPrice(double val);
    void setVolatility(double vol);
    void setDividendRate(double divRate);
    void setRiskFreeRate(double rfRate);
    void setTimeToExpiry(double time);
    void setOptionType(bool isCall);
};
