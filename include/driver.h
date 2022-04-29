#pragma once
#include <strategy.h>

/**
 * @brief Class used to perform backtests.
 */
class Driver {
    private:
        /**
         * @brief List of all strategies available to backtest.
         */
        const vector<Strategy*> strategies;

    public:
        /**
         * @brief Construct a new Driver object.
         */
        Driver(vector<Strategy*> strategies);

        /**
         * @brief Perform a backtest for a given strategy.
         */
        void run(string strategyName);

        /**
         * @brief Get list of strategies ready for backtest.
         */
        vector<Strategy*> getStrategies() const;
};