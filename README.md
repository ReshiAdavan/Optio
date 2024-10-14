# Optio

As of September 2024, I have slowly become more and more interested in quant, and decided it is time to make a few projects given the experience I have.

I decided to create a simple options-pricing engine in C++ and named it "Optio", which is the latin origin of the word "option".

## Features

Optio is a tool for pricing vanilla (European, American) and exotic (Asian, Barrier, Lookback) options, along with their Greeks. It utilizes a variety of pricing models, including Black-Scholes, binomial/trinomial trees, and Monte Carlo simulations.

### Run Optio

1. Clone the repository.
2. In project directory, run `make` to build the Makefile.
3. Run `./main` in `build/` to start the command-line interface.
4. Follow the CLI prompts to:
   - Create an option by specifying its type and parameters.
   - Name the option to store it in memory.
   - Retrieve the option's price and Greeks.

### Architecture

#### Structure

1. Option Classes

   - Defines the Option and FloatingOption parent classes, providing functionality to create, modify, and display various vanilla and exotic options.
   - Includes methods to retrieve input parameters, Greeks, and the option price.

2. Utility Functions

   - Contains key helper functions for probability distributions, forward prices, Greeks calculations (delta, gamma, vega, etc.), and Heston stochastic volatility models.

3. Vanilla Options Pricing

   - Contains classes for European and American options.
   - Implements Black-Scholes pricing, Monte Carlo simulations, and binomial/trinomial tree methods for pricing and Greeks.

4. Exotic Options Pricing

   - Contains classes for Asian, Lookback, and Barrier options.
   - Uses Monte Carlo simulations for pricing exotic options due to their path-dependent nature.

#### Models

1. _Black-Scholes Model_ -- Used to price European options, incorporating dividends.

2. _Monte Carlo Simulations_ -- Used for exotic and European options, leveraging variance reduction techniques such as antithetic variates and delta/gamma control variates for faster and more accurate pricing.

3. _Leisen-Reimer Binomial Tree_ -- Primarily used for American options, offering efficient and accurate pricing by centering the tree around the option’s strike price.
