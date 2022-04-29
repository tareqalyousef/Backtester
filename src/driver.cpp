#include "driver.h"
#include "strategy.h"
#include <fmt/core.h>
#include <fmt/color.h>
#include "metrics.h"

using fmt::print;
using fmt::color;
using std::invalid_argument;

Driver::Driver(vector<Strategy*> strategies) : strategies(strategies) {}

void Driver::run(string strategyName)
{
    for (int i = 0; i < strategies.size(); i++)
    {
        // wrong strategy, ignore
        if (strategies[i]->getName() != strategyName)
            continue;

        print("\e[?25l");

        // prepare strategy for backtest
        strategies[i]->initialize();

        // stimulate!
        strategies[i]->simulate();
        
        // show logs
        strategies[i]->displayLogs();
        
        // build metrics and display results
        Metrics results(strategies[i]);
        results.displayResults();

        print("\e[?25h\n");
        return;
    }

    throw invalid_argument("Given strategy not in list");
}

vector<Strategy*> Driver::getStrategies() const
{
    return strategies;
}