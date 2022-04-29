#include <commandManager.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <helpCommand.h>
#include <command.h>
#include <typeinfo>

using std::decay_t;
using fmt::print;
using fmt::color;
using fmt::format;

CommandManager::CommandManager() { }

void CommandManager::parse(vector<string> args)
{
    if (args.size() == 0)
    {
        print("{}{}", format(fg(color::red), "\nError: "), format("No arguments provided\n\n", args.size()));
        return;
    }

    for (int i = 0; i < commands.size(); i++)
    {
        if (commands[i]->getName() == args[0])
        {
            // erase first arg to get command arguments
            args.erase(args.begin());
            
            // do we have the right number?
            if (commands[i]->getParameters().size() != args.size()) {
                print(fg(color::red), "\nError: ");
                print("Incorrect number of arguments (given {}, expected {})\n\n", args.size(), commands[i]->getParameters().size());
                return;
            }

            // build params for command
            unordered_map<string, string> params;
            for (int j = 0; j < args.size(); j++)
                params[commands[i]->getParameters()[j]] = args[j];

            // execute command
            commands[i]->run(params);
            return;
        }
    }

    print(fg(color::red), "\nError: ");
    print("Could not parse command\n\n", args.size());
}

void CommandManager::addCommand(Command* command)
{
    commands.push_back(command);
}

CommandManager::~CommandManager()
{
    for (int i = 0; i < commands.size(); i++)
    {
        delete commands[i];
    }
}

vector<Command*> CommandManager::getCommands()
{
    return commands;
}