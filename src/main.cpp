#include <vector>
#include <string>
#include <driver.h>
#include <commandManager.h>
#include <helpCommand.h>
#include <listCommand.h>
#include <backtestCommand.h>
#include <buildCommand.h>

using std::vector;
using std::string;

int main(int argc, char *argv[])
{
    // driver for backtests
    Driver driver({ });

    // build all commands
    CommandManager manager;
    manager.addCommand(new HelpCommand(manager));
    manager.addCommand(new ListCommand(driver));
    manager.addCommand(new BacktestCommand(driver));
    manager.addCommand(new BuildCommand());

    // parse input
    manager.parse(vector<string>(argv + 1, argv + argc));
}