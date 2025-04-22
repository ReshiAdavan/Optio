#pragma once

#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace UtilFunctions {

    // ==== Standard Normal Distribution ====

    inline double pdf(double x) {
        return (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * x * x);
    }

    inline double cdf(double x) {
        return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
    }

    // ==== Black-Scholes Helpers ====

    inline double d1_(double S, double K, double T, double r, double sig, double q) {
        return (std::log(S / K) + T * (r - q + 0.5 * sig * sig)) / (sig * std::sqrt(T));
    }

    inline double d2_(double S, double K, double T, double r, double sig, double q) {
        return d1_(S, K, T, r, sig, q) - sig * std::sqrt(T);
    }

    // ==== Greeks ====

    inline double delta(double S, double K, double r, double q, double sig, double T, bool callOption) {
        double d1 = d1_(S, K, T, r, sig, q);
        return callOption ? std::exp(-q * T) * cdf(d1) : std::exp(-q * T) * (cdf(d1) - 1);
    }

    inline double gamma(double S, double K, double r, double q, double sig, double T, bool /* callOption */) {
        double d1 = d1_(S, K, T, r, sig, q);
        return std::exp(-q * T) * pdf(d1) / (sig * S * std::sqrt(T));
    }

    inline double vega(double S, double K, double r, double q, double sig, double T, bool /* callOption */) {
        double d1 = d1_(S, K, T, r, sig, q);
        return std::exp(-q * T) * S * std::sqrt(T) * pdf(d1);
    }

    inline double theta(double S, double K, double r, double q, double sig, double T, bool callOption) {
        double d1 = d1_(S, K, T, r, sig, q);
        double d2 = d2_(S, K, T, r, sig, q);
        if (callOption) {
            return (-S * pdf(d1) * sig / (2 * std::sqrt(T))) - r * K * std::exp(-r * T) * cdf(d2) + q * S * std::exp(-q * T) * cdf(d1);
        }
        return (-S * pdf(d1) * sig / (2 * std::sqrt(T))) + r * K * std::exp(-r * T) * cdf(-d2) - q * S * std::exp(-q * T) * cdf(-d1);
    }

    inline double rho(double S, double K, double r, double q, double sig, double T, bool callOption) {
        double d2 = d2_(S, K, T, r, sig, q);
        return callOption ? K * T * std::exp(-r * T) * cdf(d2) : -K * T * std::exp(-r * T) * cdf(-d2);
    }

    // ==== Utility ====

    inline double sign(double x) {
        return (x > 0) - (x < 0);
    }

    inline double peizer_pratt_cdf(double z, double N) {
        double term = std::pow(z / (N + 1.0 / 3.0 + 0.1 / (N + 1)), 2) * (N + 1.0 / 6.0);
        return 0.5 + 0.5 * sign(z) * std::sqrt(1 - std::exp(-term));
    }

    // ==== Heston Dynamics ====

    inline void hestonDynamics(double& S, double& v, double r, double q,
                                double kappa, double theta, double sigma, double rho,
                                double dt, double z1, double z2) {
        double dWt = std::sqrt(dt) * z1;
        double dZt = std::sqrt(dt) * (rho * z1 + std::sqrt(1 - rho * rho) * z2);

        v += kappa * (theta - v) * dt + sigma * std::sqrt(v) * dZt;
        S *= std::exp((r - q - 0.5 * v) * dt + std::sqrt(v) * dZt);
    }

    // ==== Used in LR Greeks (American Option Approximation) ====

    inline double lr_americanOption_forGreeks(double S, double K, double T, double r, double sig, double q, int N, bool callOption) {
        if (N % 2 == 0) N++;
        double dt = T / N;

        double d1 = d1_(S, K, T, r, sig, q);
        double d2 = d2_(S, K, T, r, sig, q);
        double pu = peizer_pratt_cdf(d2, N);
        double pd = peizer_pratt_cdf(d1, N);

        double u = std::exp((r - q) * dt) * (pd / pu);
        double d = std::exp((r - q) * dt) * ((1 - pd) / (1 - pu));

        std::vector<std::vector<double>> stock(N+1, std::vector<double>(N+1));
        std::vector<std::vector<double>> opt(N+1, std::vector<double>(N+1));

        for (int i = 0; i <= N; ++i)
            for (int j = 0; j <= i; ++j)
                stock[i][j] = S * std::pow(u, j) * std::pow(d, i - j);

        for (int j = 0; j <= N; ++j)
            opt[N][j] = std::max(0.0, (callOption ? 1 : -1) * (stock[N][j] - K));

        for (int i = N - 1; i >= 0; --i)
            for (int j = 0; j <= i; ++j)
                opt[i][j] = std::max(
                    std::exp(-r * dt) * (pu * opt[i+1][j+1] + (1 - pu) * opt[i+1][j]),
                    (callOption ? 1 : -1) * (stock[i][j] - K)
                );

        return opt[0][0];
    }
}
