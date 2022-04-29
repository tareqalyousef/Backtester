#pragma once
#include "date.h"
#include <string>

/**
 * @brief Represents a long or short position.
 */
class Position {
    public:
        /**
         * @brief Denotes a long or short position type.
         */
        enum PositionType { LONG, SHORT };

    private:
        /**
         * @brief How many shares do we have in this position?
         */
        int shares;

        /**
         * @brief At what date was the position opened?
         */
        Date timestamp;

        /**
         * @brief Symbol involved in this position.
         */
        string symbol;

        /**
         * @brief The price of the asset the instant the position was opened.
         */
        double price;

        /**
         * @brief Is this a short or long?
         */
        PositionType positionType;
    
    public:
        /**
         * @brief Construct a new Position object.
         */
        Position(Date timestamp, string symbol, int shares, PositionType positionType, double price);

        /**
         * @brief Get the number of shares involved in this position.
         */
        int getShares();

        /**
         * @brief Get the timestamp at which this position was opened.
         */
        Date getTimestamp();

        /**
         * @brief Get the symbol involved with this position.
         */
        string getSymbol();

        /**
         * @brief Get the price of this position the instant it was opened.
         */
        double getPrice();

        /**
         * @brief Get the position type (short or long).
         */
        PositionType getPositionType();
};