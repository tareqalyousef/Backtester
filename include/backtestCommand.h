#pragma once
#include <command.h>
#include <driver.h>


/**
 * @brief Command to perform a backtest.
 */
class BacktestCommand : public Command {
    private:
        /**
         * @brief Contains backtest functionality.
         */
        Driver& driver;
        
    public:
        /**
         * @brief Construct a new BacktestCommand object.
         */
        BacktestCommand(Driver& driver);

        /**
         * @brief Perform backtest.
         */
        void run(unordered_map<string, string> params);
};