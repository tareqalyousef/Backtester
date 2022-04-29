#include <listCommand.h>
#include "fmt/core.h"
#include "fmt/color.h"

using fmt::print;
using fmt::color;

ListCommand::ListCommand(Driver& driver)
    : Command("list", "List all strategies for backtesting", {}), driver(driver) {}

void ListCommand::run(unordered_map<string, string> params)
{
    if (driver.getStrategies().size() == 0)
    {
        print(fg(color::slate_gray), "\nNo strategies found. Add a strategy to the driver.\n\n");
        return;
    }

    print(fg(color::slate_gray), "\nStrategies available to backtest\n");
    
    for (int i = 0; i < driver.getStrategies().size(); i++)
        print(fg(color::lime_green), "{}\n", driver.getStrategies()[i]->getName());

    print("\n");
}