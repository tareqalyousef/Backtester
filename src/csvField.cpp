#include <csvField.h>
#include <exception>
#include <fmt/core.h>

using std::stod;
using std::stoi;
using std::stol;

CSVField::CSVField(string value)
    : value(value) {}

CSVField::CSVField() {}

string CSVField::get() 
{
    return value;
}

double CSVField::getDouble() 
{
    return stod(value);
}

int CSVField::getInt() 
{
    return stoi(value);
}

long CSVField::getLong() 
{
    return stol(value);
}

Date CSVField::getDate()
{
    return Date::fromString(value);
}