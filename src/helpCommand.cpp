#include <helpCommand.h>
#include "fmt/core.h"
#include "fmt/color.h"

using fmt::print;
using fmt::color;
using fmt::format;

HelpCommand::HelpCommand(CommandManager& manager)
    : Command("help", "Show all commands and descriptions", {}), manager(manager) {}

void HelpCommand::run(unordered_map<string, string> params)
{
    vector<Command*> commands = manager.getCommands();

    print("\n");
    print(fg(color::slate_gray), format("{:<30}", "Command"));
    print(fg(color::slate_gray), "Description\n");

    for (int i = 0; i < commands.size(); i++)
    {
        string usage = commands[i]->getName();
        for (int j = 0; j < commands[i]->getParameters().size(); j++)
            usage += " <" + commands[i]->getParameters()[j] + ">";

        print(fg(color::lime_green), format("{:<30}", usage));
        print("{}\n", commands[i]->getDescription());
    }

    fmt::print("\n");
}