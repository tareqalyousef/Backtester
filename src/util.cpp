#include "util.h"
#include <cmath>

using std::to_string;

double Util::percentDifference(double a, double b)
{
    double percentChange = b / a;
    return (percentChange * 100) - 100;
}

double Util::roundToIncrement(double value, double increment)
{
    return floor(value * increment + 0.5) / increment;
}