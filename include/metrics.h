#pragma once
#include <vector>
#include "strategy.h"

using std::vector;

/**
 * @brief Metrics related to a strategies performance.
 */
class Metrics {
    private:
        /**
         * @brief Strategy we are analyzing.
         */
        Strategy* strategy;

        /**
         * @brief Compute the sharpe ratio.
         */
        double computeSharpeRatio();

        /**
         * @brief Compute the sortino ratio.
         */
        double computeSortinoRatio();

        /**
         * @brief Compute the compounded, annual growth return (CAGR).
         */
        double computeCAGR();

        /**
         * @brief Compute the calmar ratio.
         */
        double computeCalmarRatio();

        /**
         * @brief Compute the maximum drawdown.
         */
        double computeMaxDrawdown();

    public:
        /**
         * @brief Construct a new Metrics object.
         */
        Metrics(Strategy* strategy);

        /**
         * @brief Prints metrics to console.
         */
        void displayResults();
};