#pragma once
#include <command.h>
#include <commandManager.h>

/**
 * @brief Command to build project.
 */
class BuildCommand : public Command {
    public:
        /**
         * @brief Construct a new BuildCommand object.
         */
        BuildCommand();

        /**
         * @brief Build project.
         */
        void run(unordered_map<string, string> params);
};