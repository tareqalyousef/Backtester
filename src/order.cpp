#include <order.h>
#include <iostream>

using std::logic_error;

Order::Order(Date timestamp, string symbol, int shares, OrderType orderType, Position::PositionType positionType)
    : timestamp(timestamp), symbol(symbol), shares(shares), orderType(orderType), positionType(positionType)
{
    actionType = MARKET;
    checkParams();
}

Order::Order(Date timestamp, string symbol, int shares, double limit, OrderType orderType, Position::PositionType positionType)
    : timestamp(timestamp), symbol(symbol), shares(shares), limit(limit), orderType(orderType), positionType(positionType)
{
    actionType = LIMIT;
    checkParams();
}

Order::Order(Order* old, int newShares)
{
    shares = old->getShares();
    timestamp = old->getTimestamp();
    symbol = old->getSymbol();
    orderType = old->getOrderType();
    actionType = old->getActionType();
    positionType = old->getPositionType();

    if (actionType == LIMIT)
        limit = old->getLimit();

    checkParams();
}
double Order::getLimit()
{
    if (actionType != LIMIT)
        throw logic_error("Order is not a limit");
    
    return limit;
}

int Order::getShares()
{
    return shares;
}

Date Order::getTimestamp()
{
    return timestamp;
}

string Order::getSymbol()
{
    return symbol;
}

Order::OrderType Order::getOrderType()
{
    return orderType;
}

Order::ActionType Order::getActionType()
{
    return actionType;
}

Position::PositionType Order::getPositionType()
{
    return positionType;
}

void Order::checkParams()
{
    if (shares < 1)
        throw logic_error("Number of shares must be greater than 0");

    if (actionType == LIMIT && limit <= 0)
        throw logic_error("Limit must be greater than zero");
}