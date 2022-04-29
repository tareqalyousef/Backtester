#include <backtestCommand.h>
#include "fmt/core.h"
#include "fmt/color.h"

using fmt::print;
using fmt::color;
using fmt::format;

BacktestCommand::BacktestCommand(Driver& driver)
    : Command("backtest", "Backtest a strategy", {"strategy"}), driver(driver) {}

void BacktestCommand::run(unordered_map<string, string> params)
{
    for (int i = 0; i < driver.getStrategies().size(); i++)
    {
        if (driver.getStrategies()[i]->getName() == params["strategy"])
        {
            print("{}{}", format(fg(color::slate_gray), "\nStarting backtest for "), format(fg(color::lime_green), "{}\n", params["strategy"]));
            driver.run(params["strategy"]);
            return;
        }
    }

    print("{}{}", format(fg(fmt::color::red), "\nError: "), format("Could not find strategy \"{}\"\n\n", params["strategy"]));
}