Backtester
===

Backtest trading strategies in a multithreaded CLI application. This backtester was written entirely in C++ and uses data from [Alpha Vantage](https://www.alphavantage.co/). Docs created using [Doxygen](https://github.com/doxygen/doxygen).

Getting started
---

First, clone the repository:

`git clone https://github.com/tareqalyousef/Backtester.git`

Next, navigate to the `/build` directory and build the project:

`cmake ../ && make`

Documentation
---
For a detailed list of the source files and descriptions, open `index.html` in the `/docs` folder. 

Commands
---

For a list of commands, use `./backtester help`.

Creating a trading strategy
---

All strategies inherit from `Strategy`, which contains boilerplate for creating a strategy. It contains a list of all orders, positions, market data, and metrics.

Create a class that inherits from `Strategy`. You are given a few virtual functions to implement:

```c++
class ExampleStrategy : public Strategy {
    public:
        void onInitialize();
        void onDay();
        void onOrderFilled(Order* order, Position* position, double price, int shares);
};
```

Inside the `onInitialize()` method, you can adjust parameters for your algorithm. For example, you can modify the start date, end date, initial equity. You will need to load symbols using `addSymbol()`.

The bulk of your algorithm will exist in the `onDay()` method. Here, you can analyze all new market data for the date and act accordingly. For example, you can analyze how a provided symbol has performed today by pulling its tradebars using `getMarket()`. With this information, you might want to create an order with `createOrder()`.

Once you have created your class, add it to the driver in `main.cpp`:

```c++
Driver driver({ new ExampleStrategy() });
```

When a backtest starts, a `Driver` object will continuously call the `onDay()` member function of your strategy until the end date has been reached. Market data is updated "daily" before the function is called.

To be more precise, this is the order of execution every "day":

1) Market data is updated to include tradebars of all included symbols for the date
2) Equity is adjusted to account for aftermarket moves from previous date to today
3) All pending orders are processed
4) Equity is adjusted to account for moves in the market today
5) The strategy member method `onDay()` is called.


Backtesting the strategy
---

Call the build command. An example output:

```
$ ./backtester backtest "example strategy"

Starting backtest for example strategy
[1/3] Initializing...
[2/3] Collecting data...
[3/3] Performing backtest...

Logs:
No logs recorded

Backtest results for example strategy
CAGR: 25.28%
Sharpe Ratio: 0.77
Sortino Ratio: 0.93
Calmar Ratio: 0.49
Max drawdown: 51.72%
Initial Equity: $25000
Final Equity: $201484.06
Wins: 30
Losses: 42
Number of trades: 72
Winrate: 41.67%
Average win: 16.78%
Average loss: -5.32%
Percent daily return st. deviation: 1.97
Start date: 29/12/2000
End date: 13/4/2010
Backtest duration: 2333 days
Backtest duration (w/ weekends): 3392 days
```

