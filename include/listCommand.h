#pragma once
#include <command.h>
#include <driver.h>

/**
 * @brief Command to show all strategies available to backtest.
 */
class ListCommand : public Command {
    private:
        /**
         * @brief Contains the strategies.
         */
        Driver& driver;

    public:
        /**
         * @brief Construct a new ListCommand object.
         */
        ListCommand(Driver& driver);

        /**
         * @brief Show the names of all stratgies loaded into the driver
         */
        void run(unordered_map<string, string> params);
};