#include "bar.h"

Bar::Bar()
    : open(-1), high(-1), low(-1), close(-1), volume(-1), timestamp(Date()) {}

Bar::Bar(double open, double high, double low, double close, long volume, Date date)
    : open(open), high(high), low(low), close(close), volume(volume), timestamp(date) {}

double Bar::getOpen() const
{
    return open;
}

double Bar::getHigh() const
{
    return high;
}

double Bar::getLow() const
{
    return low;
}

double Bar::getClose() const
{
    return close;
}

long Bar::getVolume() const
{
    return volume;
}

Date Bar::getTimestamp() const
{
    return timestamp;
}