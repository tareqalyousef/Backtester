#pragma once
#include <iostream>
#include <date.h>
#include <unordered_map>
#include <vector>
#include <bar.h>
#include <order.h>
#include <position.h>

using std::unordered_map;
using std::string;
using std::vector;
using std::pair;

/**
 * @brief Abstract class used for creating a trading strategy.
 * @details All logic for creating a trading strategy is implemented in classes
 *          that inherit this class.
 */
class Strategy {
    private:
        /**
         * @brief Nickname of the strategy.
         */
        const string name;

        /**
         * @brief Starting equity for backtesting.
         */
        int initialEquity;

        /**
         * @brief Start date for backtesting.
         */
        Date startDate;

        /**
         * @brief End date for backtesting.
         */
        Date endDate;

        /**
         * @brief All tickers loaded for backtesting.
         */
        vector<string> symbols = {};

        /**
         * @brief Has the strategy been initialized?
         */
        bool initialized = false;

        /**
         * @brief Has all data been populated?
         */
        bool populated = false;

        /**
         * @brief The current amount of money that can be spent.
         */
        double buyingPower;

        /**
         * @brief The current number of days in which the strategy has been executed.
         */
        int tradingDays;
        
        /**
         * @brief Current date in the backtest.
         */
        Date currentDate;

        /**
         * @brief The current, total value of the account.
         */
        double equity;

        /**
         * @brief The current number of profitable trades.
         */
        int wins = 0;

        /**
         * @brief The current number of nonprofitable trades.
         */
        int losses = 0;

        /**
         * @brief All loaded symbols and their corresponding daily bars for entire timeline.
         */
        unordered_map<string, unordered_map<Date, Bar, Date::DateHash>> data;

        /**
         * @brief All loaded symbols and their corresponding daily bars till current date.
         */
        unordered_map<string, vector<Bar>> market;

        /**
         * @brief All current orders.
         */
        vector<Order*> orders;

        /**
         * @brief All current positions.
         */
        vector<Position*> positions;

        /**
         * @brief Sum of all percent changes for wins.
         */
        double sumWinPercent = 0;

        /**
         * @brief Sum of all percent changes for losses.
         */
        double sumLossPercent = 0;

        /**
         * @brief List of all percent changes in equity till current date.
         */
        vector<double> returns;

        /**
         * @brief List of all previous equities till current date.
         */
        vector<double> equities;

        /**
         * @brief List of all date-log pairs added by the strategy.
         */
        vector<pair<Date, string>> logs;

        /**
         * @brief Ensure all parameters for backtest are valid.
         */
        void checkParams();

        /**
         * @brief Update all market data for today.
         */
        void updateMarketData();

        /**
         * @brief Make changes to equity based off aftermarket moves for all positions.
         */
        void processAfterHours();

        /**
         * @brief Analyze all orders and make changes accordingly.
         */
        void processOrders();

        /**
         * @brief Make changes to equity based off market moves for all positions.
         */
        void processTradingHours();

        /**
         * @brief Is the market open for trading today?
         */
        bool isMarketOpenToday();

        /**
         * @brief Simulate a "day" for backtesting.
         */
        void tick();

        /**
         * @brief Has the strategy beeen completed backtested?
         */
        bool hasCompleted();

        /**
         * @brief Load all data for all loaded symbols.
         */
        void populateData();

        /**
         * @brief Load all data for a give range in the symbols vector.
         */
        void populateData(string symbol);

    protected:
        /**
         * @brief Construct a new Strategy object.
         */
        Strategy(string name);

        /**
         * @brief Called every "day" in the backtest.
         */
        virtual void onDay() = 0;

        /**
         * @brief Called during initialization.
         */
        virtual void onInitialize() = 0;

        /**
         * @brief Called after an order is filled.
         * @param order The order that opened the position.
         * @param position The position that was added or removed.
         * @param price The price at which the position was opened or closed at.
         * @param shares The number of shares that were filled.
         */
        virtual void onOrderFilled(Order* order, Position* position, double price, int shares) = 0;

        /**
         * @brief Get all symbols available to load.
         */
        vector<string> getAllSymbols();

        /**
         * @brief Load a stock ticker for backtesting.
         */
        void addSymbol(string symbol);

        /**
         * @brief Set first day for backtesting.
         */
        void setStartDate(Date date);

        /**
         * @brief Set last date for backtesting.
         */
        void setEndDate(Date date);

        /**
         * @brief Set initial capital.
         */
        void setInitialEquity(int equity);

        /**
         * @brief Get all symbols with corresponding daily bars till current date.
         */
        unordered_map<string, vector<Bar>> getMarket();

        /**
         * @brief Create an order.
         */
        Order* createOrder(string symbol, int shares, Order::OrderType orderType, Position::PositionType positionType);

        /**
         * @brief Create an order.
         */
        Order* createOrder(string symbol, int shares, double limit, Order::OrderType orderType, Position::PositionType positionType);

        /**
         * @brief Removes all orders that have not been filled.
         */
        void cancelOrders();

        /**
         * @brief Adds an entry to the logs that will be printed after strategy execution.
         */
        void log(string content);

    public:
        /**
         * @brief Get nickname of the strategy.
         */
        string getName() const;

        /** 
         * @brief This method continuously calls the tick() function till the backtester is complete.
         */
        void simulate();

        /**
         * @brief Allows to set custom parameters for backtester. 
         */
        void initialize();

        /**
         * @brief Get current number of profitable trades.
         */
        int getWins();

        /**
         * @brief Get current number of nonprofitable trades.
         */
        int getLosses();

        /**
         * @brief Get the number of transactions created to current date.
         */
        int getTransactionCount();

        /**
         * @brief Get sum of all percent changes for wins.
         */
        double getSumWinPercent();

        /**
         * @brief Get sum of all percent changes for losses.
         */
        double getSumLossPercent();

        /**
         * @brief Gets all current, unfilled orders.
         */
        vector<Order*> getOrders();

        /**
         * @brief Gets all current positions.
         */
        vector<Position*> getPositions();
        
        /**
         * @brief Get all tickers loaded into the strategy.
         */
        vector<string> getSymbols();

        /**
         * @brief Get start date.
         */
        Date getStartDate();

        /**
         * @brief Get end date.
         */
        Date getEndDate();

        /**
         * @brief Get buying power.
         */
        double getBuyingPower();

        /**
         * @brief Get current date.
         */
        Date getCurrentDate();

        /**
         * @brief Get current, total value of account.
         */
        double getEquity();

        /**
         * @brief Get number of shares for specified position.
         */
        int getPositionCount(Position::PositionType positionType, string symbol);

        /**
         * @brief Get number of shares for specified position.
         */
        int getPositionCount(string symbol);

        /**
         * @brief Get number of shares for specified order.
         */
        int getOrderCount(Order::OrderType orderType, Position::PositionType positionType, string symbol);

        /**
         * @brief Get number of shares for specified order.
         */
        int getOrderCount(Position::PositionType positionType, string symbol);

        /**
         * @brief Get number of shares for specified order.
         */
        int getOrderCount(string symbol);

        /**
         * @brief Get the average win percent for profitable transactions.
         */
        double getAverageWin();

        /**
         * @brief Get the average loss percent for nonprofitable transactions.
         */
        double getAverageLoss();

        /**
         * @brief Get initial equity.
         */
        double getInitialEquity();

        /**
         * @brief Get the current winrate.
         */
        double getWinRate();

        /**
         * @brief Get the number of days between start date and end date.
         */
        int getDuration();

        /**
         * @brief Get the number of days between start date and end date in which the market was opened.
         */
        int getTradingDuration();

        /**
         * @brief Get list of all percent changes in equity till current date.
         */
        vector<double> getReturnHistory();

        /**
         * @brief Get list of all previous equities till current date.
         */
        vector<double> getEquityHistory();

        /**
         * @brief Prints all date-log pairs to console.
         */
        void displayLogs();
};