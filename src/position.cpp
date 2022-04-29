#include <position.h>

Position::Position(Date timestamp, string symbol, int shares, PositionType positionType, double price)
    : timestamp(timestamp), symbol(symbol), shares(shares), positionType(positionType), price(price) {}

int Position::getShares()
{
    return shares;
}

Date Position::getTimestamp()
{
    return timestamp;
}

string Position::getSymbol()
{
    return symbol;
}

double Position::getPrice()
{
    return price;
}

Position::PositionType Position::getPositionType()
{
    return positionType;
}