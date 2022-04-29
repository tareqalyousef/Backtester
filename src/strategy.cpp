#include <stdexcept>
#include <fmt/core.h>
#include <fmt/color.h>
#include <csvReader.h>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <cmath>
#include <thread>
#include <set>
#include "strategy.h"
#include "util.h"
#include "defaults.h"

using std::logic_error;
using fmt::print;
using fmt::color;
using std::filesystem::directory_iterator;
using std::make_pair;
using std::reverse;
using std::unordered_set;
using std::cout;
using fmt::format;
using std::thread;
using std::set;

Strategy::Strategy(std::string name)
    : name(name) {}

void Strategy::addSymbol(string symbol)
{
    if (initialized)
        throw logic_error("Cannot add a symbol after initialization");

    symbols.push_back(symbol);
}

vector<string> Strategy::getAllSymbols()
{
    vector<string> res;

    // assuming the files are all .csv
    for (const auto & entry : directory_iterator("../data/")) {
        string full = entry.path().filename();
        string file = "";

        // scan from end of filename from character before period till first slash
        bool foundPeriod = false;
        for (int i = full.size() - 1; i >= 0 && full[i] != '/'; i--) 
        {
            if (foundPeriod)
                file += full[i];
            else
                foundPeriod = full[i] == '.';
        }

        // reverse to get filename without type
        reverse(file.begin(), file.end());

        // add to list
        res.push_back(file);
    }

    return res;
}

void Strategy::setStartDate(Date date)
{
    if (initialized)
        throw logic_error("Cannot set start date after initialization");

    startDate = date;
}

void Strategy::setEndDate(Date date)
{
    if (initialized)
        throw logic_error("Cannot set end date after initialization");

    endDate = date;
}

void Strategy::setInitialEquity(int capital)
{
    if (initialized)
        throw logic_error("Cannot set initial equity after initialization");

    initialEquity = capital;
}

string Strategy::getName() const
{
    return name;
}

void Strategy::populateData(string symbol)
{
    CSVReader reader("../data/" + symbol + ".csv");
    unordered_map<Date, Bar, Date::DateHash> bars;

    while (reader.hasNext())
    {
        unordered_map<string, CSVField> map = reader.getRow();

        // read from start date
        if (map["timestamp"].getDate() < startDate)
            continue;

        double ratio = map["adjusted_close"].getDouble() / map["close"].getDouble();

        // create tradebar from csv data
        Bar corrected(
            map["open"].getDouble() * ratio,
            map["high"].getDouble() * ratio,
            map["low"].getDouble() * ratio,
            map["close"].getDouble() * ratio,
            map["volume"].getLong(),
            Date::fromString(map["timestamp"].get())
        );
        
        bars.insert(make_pair(map["timestamp"].getDate(), corrected));
    }

    data.insert(make_pair(symbol, bars));
}

void Strategy::populateData()
{
    print("[{}] {}\n", format(fg(color::lime_green), "2/3"), "Collecting data...");

    // how many threads does our cpu have?
    int threadCount = thread::hardware_concurrency();

    // no need for additional threads
    if (threadCount >= symbols.size())
    {
        for (int i = 0; i < symbols.size(); i++)
            populateData(symbols[i]);
    }
    else
    {
        // all threads
        vector<thread*> threads;

        // how many symbols per thread?
        int perThread = symbols.size() / threadCount;

        for (int i = 0; i < threadCount; i++)
        {
            threads.push_back(new thread([this, i, perThread, threadCount]
            {
                int start = i * perThread;
                int end = i + 1 == threadCount ? symbols.size() - 1 : start + perThread;

                for (int j = start; j <= end; j++)
                    populateData(symbols[j]);
            }));
        }

        for (int i = 0; i < threads.size(); i++)
            threads[i]->join();

        print("\n");
    }

    // prevent repeat populating
    populated = true;
}

void Strategy::simulate()
{
    print("[{}] {}\n", format(fg(color::lime_green), "3/3"), "Performing backtest...");

    // number of days between start and end
    int length = endDate - startDate;

    // number of days simulated from start date
    int elapsed = 0;

    while (!hasCompleted())
    {
        int percentCompletion = (int)((double)elapsed / length * 100);
        cout.flush();

        tick();
        elapsed++;
    }

    print("\n");
}

void Strategy::tick()
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");
    
    // if market is not open today, done
    if (isMarketOpenToday())
    {
        // equity before processing orders and positions
        double beforeEquity = equity;

        // update all market data
        updateMarketData();

        // how has our equity changed in afterhours?
        processAfterHours();

        // how have our orders been affected by the new data?
        processOrders();

        // how has our equity changed during trading hours?
        processTradingHours();

        // now that the day has "finished," allow user to make decisions with new changes
        this->onDay();

        // since trading was done today, increment
        tradingDays++;

        // add stats to history
        returns.push_back(Util::percentDifference(equity, beforeEquity));
        equities.push_back(equity);
    }

    // increment, repeat
    currentDate++;
}

void Strategy::updateMarketData()
{
    // loop through each symbol and expose data from current date to strategy
    for (string symbol : symbols)
    {
        // in this case, this asset does not have data for this day. this means
        // that the underlying company has either went bankrupt or has not been
        // created yet! this won't affect the backtest negatively.
        if (data[symbol].find(currentDate) == data[symbol].end())
            continue;

        market[symbol].push_back(data[symbol][currentDate]);
    }
}

void Strategy::processAfterHours()
{
    for (int i = 0; i < positions.size(); i++)
    {
        vector<Bar> bars = market[positions[i]->getSymbol()];
        
        // no data for this asset on this day, done
        if (bars[bars.size() - 1].getTimestamp() != currentDate)
            continue;
        
        // how much has the stock changed in value overnight?
        double change = bars[bars.size() - 1].getOpen() - bars[bars.size() - 2].getClose();

        // change our equity!
        if (positions[i]->getPositionType() == Position::PositionType::LONG)
            equity += change * positions[i]->getShares();
        else
            equity -= change * positions[i]->getShares();
    }
}

void Strategy::processOrders()
{
    // we sort our orders such that we ALWAYS compute orders to close positions before orders to open positions.
    //
    // imagine a case where an asset has a high price of $2.00 and a low of $1.00 in a single trading day.
    // let's say we have two orders, one to open a position at $2.00, and an order to close a position $1.00.
    // since we do not know if the high came before or after the low, we cannot know the order of execution.
    // we only know that the price has reached both $1.00 and $2.00-- but we don't know the order!
    sort(orders.begin(), orders.end(), [](Order* a, Order* b) -> bool
    {
        return a->getOrderType() == Order::OrderType::CLOSE;
    });

    // we reverse this since we traverse the list backwards.
    // we traverse the list backwards since we remove elements as we go.
    reverse(orders.begin(), orders.end());

    // traverse all orders and decide to ignore them, fill them partially, or fill them completely
    for (int i = orders.size() - 1; i >= 0; i--)
    {
        Order* order = orders[i];
        vector<Bar> bars = market[orders[i]->getSymbol()];

        // no data for this asset on this day, done
        if (bars[bars.size() - 1].getTimestamp() != currentDate)
            continue;
        
        // the price per share that we will open or close our position(s) at
        double perShare = -1;

        // depending on action type, determine price per share to open or close shares
        if (order->getActionType() == Order::ActionType::MARKET)
        {
            perShare = bars[bars.size() - 1].getOpen();
        }
        else if (order->getActionType() == Order::ActionType::LIMIT)
        {
            // has the price crossed the limit today?
            //
            // NOTE: we are backtesting exclusively on trading hours and not afterhours.
            //       it is possible for the limit to be crossed in aftermarket hours,
            //       in which case nothing will happen.
            if (bars[bars.size() - 1].getHigh() >= order->getLimit() && bars[bars.size() - 1].getLow() <= order->getLimit())
                perShare = order->getLimit();
            else
                continue;
        }

        // open a new position
        if (order->getOrderType() == Order::OrderType::OPEN)
        {
            int trueShares = order->getShares();             // the number of shares we can open or close
            double cost = perShare * order->getShares();    // the total cost of opening this position
            
            // if we can't afford this order, then adjust number of shares
            if (cost > buyingPower)
            {
                double factor = (cost - buyingPower) / perShare;
                int drop = factor;

                if (factor - drop != 0) 
                    drop++;

                trueShares -= drop;
            }

            // can't afford anything, done
            if (trueShares == 0)
                continue;

            // adjust buying power
            buyingPower -= trueShares * perShare;

            // create new position with shares we can afford
            Position* position = new Position(currentDate, order->getSymbol(), trueShares, order->getPositionType(), perShare);
            positions.push_back(position);

            // adjust equity for and only for today
            // we do this here, and processTradingHours() takes care of positions for the full day
            double change = trueShares * (bars[bars.size() - 1].getClose() - perShare);

            if (position->getPositionType() == Position::PositionType::LONG)
                equity += change;
            else
                equity -= change;

            // either adjust or remove order (depending on our ability to fill entire order)
            if (trueShares != order->getShares())
                orders[i] = new Order(order, order->getShares() - trueShares);
            else
                orders.erase(orders.begin() + i);

            // allow strategy to play with this information!
            this->onOrderFilled(order, position, perShare, trueShares);
        }
        // close a preexisting position
        else if (order->getOrderType() == Order::OrderType::CLOSE)
        {
            // how many shares have we sold of this asset?
            int sold = 0;

            for (int j = positions.size() - 1; j >= 0; j--)
            {
                Position* pos = positions[j];

                // we have closed the requested amount, done
                if (sold == order->getShares())
                    return;

                // must be same symbol
                if (pos->getSymbol() != order->getSymbol())
                    continue;

                // must be same position type
                if (pos->getPositionType() != order->getPositionType())
                    continue;
                
                // the number of shares we will close from this position
                int trueShares = pos->getShares() > order->getShares() ? order->getShares() : pos->getShares();
                sold += trueShares;

                // if we can't completetely close position, modify number of shares
                if (trueShares == pos->getShares())
                    positions.erase(positions.begin() + j);
                else
                    positions[j] = new Position(pos->getTimestamp(), pos->getSymbol(), pos->getShares() - trueShares, pos->getPositionType(), pos->getPrice());
                
                // adjust buying power
                buyingPower += trueShares * perShare;

                // adjust equity for and only for today
                // we do this here since the position will be removed and not add to equity in processTradingHours()
                double change = trueShares * (perShare - bars[bars.size() - 1].getOpen());

                if (pos->getPositionType() == Position::PositionType::LONG)
                    equity += change;
                else
                    equity -= change;
                
                // update win and loss record
                double percent = Util::percentDifference(pos->getPrice(), perShare);
                bool win = perShare - pos->getPrice() >= 0 && pos->getPositionType() == Position::PositionType::LONG ||
                           perShare - pos->getPrice() < 0 && pos->getPositionType() == Position::PositionType::SHORT;
                        
                if (win)
                {
                    sumWinPercent += percent;
                    wins++;
                }
                else
                {
                    sumLossPercent += percent;
                    losses++;
                }

                // allow strategy to play with this information!
                this->onOrderFilled(order, pos, perShare, trueShares);
            }

            // either adjust or remove order (depending on our ability to fill entire order)
            if (sold != order->getShares())
                orders[i] = new Order(order, order->getShares() - sold);
            else
                orders.erase(orders.begin() + i);
        }
    }
}

void Strategy::processTradingHours()
{
    for (int i = 0; i < positions.size(); i++)
    {
        // if this position was added today, then the equity was taken care of already (in processOrders())
        if (positions[i]->getTimestamp() == currentDate)
            continue;

        vector<Bar> bars = market[positions[i]->getSymbol()];
        
        // no data for this asset on this day, done
        if (bars[bars.size() - 1].getTimestamp() != currentDate)
            continue;
        
        // how much has the stock changed in value today?
        double change = bars[bars.size() - 1].getClose() - bars[bars.size() - 1].getOpen();

        // change our equity!
        if (positions[i]->getPositionType() == Position::PositionType::LONG)
            equity += change * positions[i]->getShares();
        else
            equity -= change * positions[i]->getShares();
    }
}

bool Strategy::isMarketOpenToday()
{
    // if there is a single asset that has data for today,
    // then we are confident the market is open today.
    //
    // it is possible, however, for some assets to be open
    // while others are closed as all exchanges do not follow
    // the same holidays.
    for (string symbol : symbols)
    {
        if (data[symbol].find(currentDate) != data[symbol].end())
            return true;
    }

    return false;
}

void Strategy::initialize()
{
    if (initialized)
        throw logic_error("Strategy has already been initialized");

    print("[{}] {}\n", format(fg(color::lime_green), "1/3"), "Initializing...");
    
    // populate with default values
    setInitialEquity(DEFAULT_INTIIAL_EQUITY);
    setStartDate(DEFAULT_START_DATE);
    setEndDate(DEFAULT_END_DATE);

    // allow user to change settings
    this->onInitialize();

    // check for any errors in their settings
    checkParams();

    // set initial values for backtest
    currentDate = startDate;
    buyingPower = initialEquity;
    equity = initialEquity;
    equities.push_back(equity);

    // prevent repeat initialization
    initialized = true;

    // add data
    populateData();
}

bool Strategy::hasCompleted()
{
    return currentDate > endDate;
}

void Strategy::checkParams()
{
    if (startDate >= endDate)
        throw logic_error("Start date must be less than end date");

    if (initialEquity <= 0)
        throw logic_error("Initial equity must be greater than 0");

    if (symbols.size() == 0)
        throw logic_error("Must load data for backtester");
}

vector<string> Strategy::getSymbols()
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");

    return symbols;
}

Date Strategy::getStartDate()
{
    return startDate;
}

Date Strategy::getEndDate()
{
    return endDate;
}

double Strategy::getBuyingPower()
{
    return buyingPower;
}

Date Strategy::getCurrentDate()
{
    return currentDate;
}

double Strategy::getEquity()
{
    return equity;
}

int Strategy::getWins()
{
    return wins;
}

int Strategy::getLosses()
{
    return losses;
}

unordered_map<string, vector<Bar>> Strategy::getMarket()
{
    return market;
}

Order* Strategy::createOrder(string symbol, int shares, Order::OrderType orderType, Position::PositionType positionType)
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");

    orders.push_back(new Order(currentDate, symbol, shares, orderType, positionType));
    return orders[orders.size() - 1];
}

Order* Strategy::createOrder(string symbol, int shares, double limit, Order::OrderType orderType, Position::PositionType positionType)
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");

    orders.push_back(new Order(currentDate, symbol, shares, limit, orderType, positionType));
    return orders[orders.size() - 1];
}

void Strategy::cancelOrders()
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");

    orders.clear();
}

int Strategy::getTransactionCount()
{
    return wins + losses;
}

double Strategy::getSumWinPercent()
{

    return sumWinPercent;
}

double Strategy::getSumLossPercent()
{
    return sumLossPercent;
}

vector<Order*> Strategy::getOrders()
{
    return orders;
}

vector<Position*> Strategy::getPositions()
{
    return positions;
}

int Strategy::getPositionCount(Position::PositionType positionType, string symbol)
{
    int count = 0;

    for (Position* pos : positions)
    {
        if (pos->getPositionType() != positionType)
            continue;

        if (pos->getSymbol() != symbol)
            continue;

        count += pos->getShares();
    }

    return count;
}

int Strategy::getPositionCount(string symbol)
{
    int count = 0;

    for (Position* pos : positions)
    {
        if (pos->getSymbol() != symbol)
            continue;

        count += pos->getShares();
    }

    return count;
}

int Strategy::getOrderCount(Order::OrderType orderType, Position::PositionType positionType, string symbol)
{
    int count = 0;

    for (Order* order : orders)
    {
        if (order->getPositionType() != positionType)
            continue;

        if (order->getOrderType() != orderType)
            continue;

        if (order->getSymbol() != symbol)
            continue;

        count += order->getShares();
    }

    return count;
}

int Strategy::getOrderCount(Position::PositionType positionType, string symbol)
{
    int count = 0;

    for (Order* order : orders)
    {
        if (order->getPositionType() != positionType)
            continue;

        if (order->getSymbol() != symbol)
            continue;

        count += order->getShares();
    }

    return count;
}

int Strategy::getOrderCount(string symbol)
{
    int count = 0;

    for (Order* order : orders)
    {
        if (order->getSymbol() != symbol)
            continue;

        count += order->getShares();
    }

    return count;
}

double Strategy::getAverageWin()
{
    if (wins == 0)
        return 0;

    return sumWinPercent / wins;
}

double Strategy::getAverageLoss()
{
    if (losses == 0)
        return 0;

    return sumLossPercent / losses;
}

double Strategy::getInitialEquity()
{
    return initialEquity;
}

double Strategy::getWinRate()
{
    if (wins + losses == 0)
        return 0;

    return (double)wins / (wins + losses) * 100;
}


int Strategy::getDuration()
{
    if (!initialized)
        throw logic_error("Strategy has not been initialized");

    return Date::getDayNumber(endDate) - Date::getDayNumber(startDate);
}

int Strategy::getTradingDuration()
{
    return tradingDays;
}

vector<double> Strategy::getReturnHistory()
{
    return returns;
}

vector<double> Strategy::getEquityHistory()
{
    return equities;
}

void Strategy::log(string content)
{
    logs.push_back(make_pair(currentDate, content));
}

void Strategy::displayLogs()
{
    print(fg(color::slate_gray), "Logs:\n");

    if (logs.size() == 0)
    {
        print("No logs recorded\n");
        return;
    }
    
    for (pair<Date, string> entry : logs)
        print("{}: {}\n", format(fg(color::cornflower_blue), format("{}", entry.first)), entry.second);
}