#pragma once
#include <command.h>
#include <commandManager.h>

/**
 * @brief Command to show all command usages.
 */
class HelpCommand : public Command {
    private:
        /**
         * @brief Contains list of all commands.
         */
        CommandManager& manager;

    public:
        /**
         * @brief Construct a new HelpCommand object.
         */
        HelpCommand(CommandManager& manager);

        /**
         * @brief Show all command names, usages, and descriptions.
         */
        void run(unordered_map<string, string> params);
};