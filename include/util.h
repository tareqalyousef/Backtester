#pragma once
#include <string>

using std::string;

namespace Util
{
    /**
     * @brief Calculates the difference of percent change from 100.
     */
    double percentDifference(double a, double b);

    /**
     * @brief Round a number to the nearest provided increment.
     */
    double roundToIncrement(double value, double increment);
}