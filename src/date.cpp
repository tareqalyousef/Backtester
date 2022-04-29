#include <date.h>
#include <stdexcept>
#include <fmt/core.h>
#include <vector>
#include <iostream>

using std::vector;
using std::invalid_argument;
using std::to_string;

Date::Date()
{
    Date(1, 1, 1900);
}

Date::Date(int month, int day, int year)
    : month(month), day(day), year(year)
{
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > Date::daysInMonth(year, month))
        throw invalid_argument("Invalid date");
}

int Date::daysInMonth(int year, int month)
{
    // leap year
    if (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) && month == 2)
        return 29;
    
    vector<int> days = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return days[month - 1];
}

Date Date::fromString(string date)
{
    int month = -1;
    int day = -1;
    int year = -1;

    string curr = "";
    for (int i = 0; i <= date.size(); i++)
    {
        if (date[i] == '-' || i == date.size())
        {
            if (year == -1)
                year = stoi(curr);
            else if (month == -1)
                month = stoi(curr);
            else if (day == -1)
                day = stoi(curr);

            curr = "";
            continue;
        }

        curr += date[i];
    }

    return Date(month, day, year);
}

int Date::getDayNumber(Date date)
{
    int m = (date.getMonth() + 9) % 12;
    int y = date.getYear() - m / 10;

    return 365 * y + y / 4 - y / 100 + y / 400 + (306 * m + 5) / 10 + (date.getDay() - 1);
}

int Date::getYear()
{
    return year;
}

int Date::getMonth()
{
    return month;
}

int Date::getDay()
{
    return day;
}

int Date::operator-(const Date& other) const
{
    Date larger = *this > other ? *this : other;
    Date smaller = *this > other ? other : *this;

    int count = 0;

    while (smaller != larger)
    {
        smaller++;
        count++;
    }
    
    return count;
}

bool Date::operator>(const Date& other) const
{
	if (year > other.year)
		return true;
    if (year < other.year)
        return false;

    if (month > other.month)
        return true;
    if (month < other.month)
        return false;

    if (day > other.day)
        return true;

	return false;
}

bool Date::operator<(const Date& other) const
{
	return !(*this > other) && !(*this == other);
}

bool Date::operator!=(const Date& other) const
{
	return !(*this == other);
}

bool Date::operator==(const Date& other) const
{
	return year == other.year && month == other.month && day == other.day;
}

bool Date::operator>=(const Date& other) const
{
	return *this == other || *this > other;
}

bool Date::operator<=(const Date& other) const
{
	return *this == other || *this < other;
}

Date& Date::operator++()
{
	day++;

    if (day > Date::daysInMonth(year, month)) {
        month++;
        day = 1;
    }

    if (month > 12) {
        year++;
        month = 1;
    }
    
	return *this;
}

Date Date::operator++(int)
{
    Date& old = *this;
    operator++();
    return old;
}

ostream& operator<<(ostream& os, const Date& date)
{
    return os << date.year << '/' << date.month << '/' << date.day;
}