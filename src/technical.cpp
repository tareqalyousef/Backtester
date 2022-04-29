#include <technical.h>
#include <cmath>

double Technical::standardDeviation(vector<double> input)
{
    if (input.size() == 0)
        return 0;

    double sum = 0;
    double mean = 0;
    double sd = 0.0;

    for (double num : input)
        sum += num;

    mean = sum / input.size();

    for (double num : input)
        sd += pow(num - mean, 2);

    return sqrt(sd / input.size());
}

double Technical::mean(vector<double> input)
{
    return sum(input) / input.size();
}

double Technical::sum(vector<double> input)
{
    double sum = 0;

    for (double num : input)
        sum += num;

    return sum;
}