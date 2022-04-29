#pragma once
#include <csvField.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

using std::string;
using std::unordered_map;
using std::ifstream;
using std::vector;

/**
 * @brief Parses .csv files.
 */
class CSVReader {
    private:
        /**
         * @brief Relative path to file.
         */
        string path;

        /**
         * @brief Input file stream to parse data.
         */
        ifstream file;

        /**
         * @brief All columns in the .csv file.
         */
        vector<string> headers = vector<string>();

    public:
        /**
         * @brief Construct a new CSVReader object.
         */
        CSVReader(string path);

        /**
         * @brief Destroy the CSVReader object.
         */
        ~CSVReader();

        /**
         * @brief Do we have another line to parse?
         */
        bool hasNext();

        /**
         * @brief Returns the .csv row as a map using headers as keys.
         */
        unordered_map<string, CSVField> getRow();
};