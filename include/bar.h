#pragma once
#include "date.h"

/**
 * @brief Represents a tradebar for a single day.
 */
class Bar {
    private:
        /**
         * @brief Value of underlying asset at open.
         */
        const double open;

        /**
         * @brief Highest daily value of underlying asset.
         */
        const double high;

        /**
         * @brief Lowest daily value of underlying asset.
         */
        const double low;

        /**
         * @brief Value of underlying asset at close.
         */
        const double close;
        
        /**
         * @brief Volume of underlying asset for day.
         */
        const long volume;

        /**
         * @brief Date for this bar.
         */
        const Date timestamp;

    public:
        /**
         * @brief Construct a new Bar object.
         */
        Bar();

        /**
         * @brief Construct a new Bar object.
         */
        Bar(double open, double high, double low, double close, long volume, Date timestamp);

        /**
         * @brief get the value of underlying asset at open.
         */
        double getOpen() const;

        /**
         * @brief Get the highest daily value of underlying asset.
         */
        double getHigh() const;

        /**
         * @brief Get the lowest daily value of underlying asset.
         */
        double getLow() const;

        /**
         * @brief Get the value of underlying asset at close.
         */
        double getClose() const;
        
        /**
         * @brief Get the volume of underlying asset for day.
         */
        long getVolume() const;

        /**
         * @brief Get the date for this bar.
         */
        Date getTimestamp() const;
};