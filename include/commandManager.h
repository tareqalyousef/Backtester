#pragma once
#include <command.h>
#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using std::unique_ptr;

/**
 * @brief Class that contains and executes all commands.
 */
class CommandManager {
    private:
        /**
         * @brief List of all commands available for the command-line.
         */
        vector<Command*> commands;

    public:
        /**
         * @brief Construct a new CommandManager object.
         */
        CommandManager();

        /**
         * @brief Destroy the Comman Manager object.
         */
        ~CommandManager();

        /**
         * @brief Parses the given arguments and executes a matching command.
         */
        void parse(vector<string> args);

        /**
         * @brief Add a command.
         */
        void addCommand(Command* command);

        /**
         * @brief Get list of all commands available for the command-line.
         */
        vector<Command*> getCommands();
};