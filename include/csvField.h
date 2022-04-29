#pragma once
#include <string>
#include <date.h>

using std::string;

/**
 * @brief Represents a single field in a .csv file.
 */
class CSVField {
    private:
        /**
         * @brief The parsed value as a string.
         */
        string value;

    public:
        /**
         * @brief Construct a new CSVField object.
         */
        CSVField();

        /**
         * @brief Construct a new CSVField object.
         */
        CSVField(string value);

        /**
         * @brief Get the original parsed string.
         */
        string get();

        /**
         * @brief Get the value parsed as a double
         */
        double getDouble();

        /**
         * @brief Get the value parsed as a long
         */
        long getLong();

        /**
         * @brief Get the value parsed as an integer.
         */
        int getInt();

        /**
         * @brief Get the value parsed as a date.
         */
        Date getDate();
};