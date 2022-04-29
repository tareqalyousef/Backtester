#pragma once
#include <functional>
#include <string>

using std::function;
using std::string;
using std::unordered_map;
using std::vector;

/**
 * @brief Abstract class used to build commands that can be executed on command-line.
 */
class Command {
    private:
        /**
         * @brief Identifier for command on command-line.
         */
        const string name;

        /**
         * @brief Description for command.
         */
        const string description;

        /**
         * @brief List of mandatory parameters (in order).
         */
        vector<string> parameters;

    public:
        /**
         * @brief Construct a new Command object.
         */
        Command(string name, string description, vector<string> parameters);

        /**
         * @brief Virtual function used to execute the command.
         */
        virtual void run(unordered_map<string, string> params) = 0;

        /**
         * @brief Get the command name.
         */
        string getName() const;

        /**
         * @brief Get the command description.
         */
        string getDescription() const;

        /**
         * @brief Get the command parameter list.
         */
        vector<string> getParameters() const;
};