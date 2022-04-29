#pragma once
#include "date.h"
#include <string>
#include "position.h"

/**
 * @brief Represents an order to open or close a position.
 */
class Order {
    public:
        /**
         * @brief Denotes an open or close position.
         */
        enum OrderType { OPEN, CLOSE };

        /**
         * @brief Denotes a market or limit order type.
         */
        enum ActionType { LIMIT, MARKET };

    private:
        /**
         * @brief What is our limit? A value of -1 indicates a market order.
         */
        double limit = -1;

        /**
         * @brief How many shares are we interested in?
         */
        int shares;

        /**
         * @brief At what date was the order placed?
         */
        Date timestamp;

        /**
         * @brief What symbol are we trying to open or close a position with?
         */
        string symbol;

        /**
         * @brief Are we trying to open or close a position?
         */
        OrderType orderType;

        /**
         * @brief Are we trying to open or close a position at market or limit?
         */
        ActionType actionType;

        /**
         * @brief Are we trying to go long or short?
         */
        Position::PositionType positionType;

        /**
         * @brief Ensure all parameters are valid.
         */
        void checkParams();

    public:
        /**
         * @brief Construct a new Order object.
         */
        Order(Date timestamp, string symbol, int shares, OrderType orderType, Position::PositionType positionType);

        /**
         * @brief Construct a new Order object
         */
        Order(Date timestamp, string symbol, int shares, double limit, OrderType orderType, Position::PositionType positionType);

        /**
         * @brief Construct a new Order object.
         */
        Order(Order* old, int newShares);

        /**
         * @brief Get the limit for a limit order.
         */
        double getLimit();

        /**
         * @brief Get the number of shares we are interested in.
         */
        int getShares();

        /**
         * @brief Get the time the order was placed.
         */
        Date getTimestamp();

        /**
         * @brief Get the underlying asset.
         */
        string getSymbol();

        /**
         * @brief Get the order type (open or close).
         */
        OrderType getOrderType();

        /**
         * @brief Get the action type (market or limit).
         */
        ActionType getActionType();

        /**
         * @brief Get the position type (long or short).
         */
        Position::PositionType getPositionType();
};