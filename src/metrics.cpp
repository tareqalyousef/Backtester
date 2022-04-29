#include "metrics.h"
#include <fmt/core.h>
#include <fmt/color.h>
#include "util.h"
#include <cmath>
#include "technical.h"

#define TRADING_DAYS_PER_YEAR 252

using std::min;
using fmt::print;
using fmt::color;
using fmt::format;

Metrics::Metrics(Strategy* strategy)
    : strategy(strategy) {}

double Metrics::computeSharpeRatio()
{
    if (Technical::standardDeviation(strategy->getReturnHistory()) == 0)
        return 0;

    return (computeCAGR() - 1) / (Technical::standardDeviation(strategy->getReturnHistory()) * sqrt(TRADING_DAYS_PER_YEAR));
}

double Metrics::computeSortinoRatio()
{
    // we are only interested in negative returns
    vector<double> negative;
    for (double change : strategy->getReturnHistory())
    {
        if (change < 0)
            negative.push_back(change);
    }

    if (Technical::standardDeviation(negative) == 0)
        return 0;

    return (computeCAGR() - 1) / (Technical::standardDeviation(negative) * sqrt(TRADING_DAYS_PER_YEAR));
}

double Metrics::computeCAGR()
{
    if (strategy->getTradingDuration() == 0)
        return 0;

    // equity_initial * cagr^duration = equity_final
    double daily =  pow(strategy->getEquity() / strategy->getInitialEquity(), 1.0 / strategy->getTradingDuration());
    double cagr = Util::percentDifference(1, pow(daily, TRADING_DAYS_PER_YEAR));

    return cagr;
}

double Metrics::computeCalmarRatio()
{
    return computeCAGR() / computeMaxDrawdown();
}

double Metrics::computeMaxDrawdown()
{
    vector<double> equities = strategy->getEquityHistory();

    double maxDrawdown = 1;
    int peak = equities[0];

    for (int i = 1; i < equities.size(); i++)
    {
        if (equities[i] >= peak)
            peak = equities[i];

        maxDrawdown = min(maxDrawdown, equities[i] / peak);
    }

    if (maxDrawdown == 1)
        return 0;
    
    return maxDrawdown * 100;
}


void Metrics::displayResults()
{
    print(fg(color::slate_gray), "\nBacktest results for ");
    print(fg(color::lime_green), "{}\n", strategy->getName());

    print("CAGR: {}", format(fg(color::dark_golden_rod), format("{}%\n", Util::roundToIncrement(computeCAGR(), 100))));
    print("Sharpe Ratio: {}", format(fg(color::dark_golden_rod), format("{}\n", Util::roundToIncrement(computeSharpeRatio(), 100))));
    print("Sortino Ratio: {}", format(fg(color::dark_golden_rod), format("{}\n", Util::roundToIncrement(computeSortinoRatio(), 100))));
    print("Calmar Ratio: {}", format(fg(color::dark_golden_rod), format("{}\n", Util::roundToIncrement(computeCalmarRatio(), 100))));
    print("Max drawdown: {}", format(fg(color::dark_golden_rod), format("{}%\n", Util::roundToIncrement(computeMaxDrawdown(), 100))));
    print("Initial Equity: {}", format(fg(color::dark_golden_rod), format("${}\n", Util::roundToIncrement(strategy->getInitialEquity(), 100))));
    print("Final Equity: {}", format(fg(color::dark_golden_rod), format("${}\n", Util::roundToIncrement(strategy->getEquity(), 100))));
    print("Wins: {}", format(fg(color::dark_golden_rod), format("{}\n", strategy->getWins())));
    print("Losses: {}", format(fg(color::dark_golden_rod), format("{}\n", strategy->getLosses())));
    print("Number of trades: {}", format(fg(color::dark_golden_rod), format("{}\n", strategy->getTransactionCount())));
    print("Winrate: {}", format(fg(color::dark_golden_rod), format("{}%\n", Util::roundToIncrement(strategy->getWinRate(), 100))));
    print("Average win: {}", format(fg(color::dark_golden_rod), format("{}%\n", Util::roundToIncrement(strategy->getAverageWin(), 100))));
    print("Average loss: {}", format(fg(color::dark_golden_rod), format("{}%\n", Util::roundToIncrement(strategy->getAverageLoss(), 100))));
    print("Percent daily return st. deviation: {}", format(fg(color::dark_golden_rod), format("{}\n", Util::roundToIncrement(Technical::standardDeviation(strategy->getReturnHistory()), 100))));
    print("Start date: {}", format(fg(color::dark_golden_rod), format("{}\n", strategy->getStartDate())));
    print("End date: {}", format(fg(color::dark_golden_rod), format("{}\n", strategy->getEndDate())));
    print("Backtest duration: {}", format(fg(color::dark_golden_rod), format("{} days\n", strategy->getTradingDuration())));
    print("Backtest duration (w/ weekends): {}", format(fg(color::dark_golden_rod), format("{} days\n", strategy->getDuration())));
}