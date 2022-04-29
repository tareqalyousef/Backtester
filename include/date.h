#pragma once
#include <string>
#include <functional>
#include <fmt/format.h>

using std::string;
using std::ostream;
using std::hash;
using fmt::formatter;
using fmt::format_to;

/**
 * @brief Represents a single calendar date.
 */
class Date
{
    private:
        /**
         * @brief Year of date.
         */
        int year;

        /**
         * @brief One-indexed integer representation of month of the year.
         */
        int month;

        /**
         * @brief Day of the month.
         */
        int day;

    public:
        /**
         * @brief Construct a new Date object.
         */
        Date();

        /**
         * @brief Construct a new Date object.
         */
        Date(int month, int day, int year);

        /**
         * @brief Get the number of days of a given month and year.
         */
        static int daysInMonth(int year, int month);

        /**
         * @brief Get the number of days between two dates.
         */
        static int daysBetween(Date first, Date second);

        /**
         * @brief Calculate day number according to Gregordian calendar.
         */
        static int getDayNumber(Date date);

        /**
         * @brief Construct a date from a string timestamp in MM/DD/YYYY format.
         */
        static Date fromString(string date);
    
        /**
         * @brief Get the year of date.
         */
        int getYear();

        /**
         * @brief Get the one-indexed integer representation of month of the year.
         */
        int getMonth();

        /**
         * @brief Get the day of the month.
         */
        int getDay();

        /**
         * @brief Used to hash a Date object.
         */
        struct DateHash
        {
            size_t operator()(const Date& date) const
            {
                return hash<int>{}(date.year) ^ hash<int>{}(date.month) ^ hash<int>{}(date.day);
            }
        };

        friend ostream& operator<<(ostream& os, const Date& date);
        int operator-(const Date& other) const;
        bool operator>(const Date& other) const;
        bool operator>=(const Date& other) const;
        bool operator<(const Date& other) const;
        bool operator<=(const Date& other) const;
        bool operator==(const Date& other) const;
        bool operator!=(const Date& other) const;
        Date& operator++();
        Date operator++(int);
};

namespace fmt
{
    /**
     * @brief Tells fmt how to print a Date object.
     */
    template<>
    struct formatter<Date>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(Date& date, FormatContext& ctx)
        {
            return format_to(ctx.out(), "{0}/{1}/{2}", date.getDay(), date.getMonth(), date.getYear());
        }
    };
}