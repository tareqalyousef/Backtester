#include <command.h>


Command::Command(string name, string description, vector<string> parameters)
    : name(name), description(description), parameters(parameters) {}

string Command::getName() const
{
    return name;
}

string Command::getDescription() const
{
    return description;
}

vector<string> Command::getParameters() const
{
    return parameters;
}