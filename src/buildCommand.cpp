#include <buildCommand.h>
#include "fmt/core.h"
#include "fmt/color.h"

using std::logic_error;
using fmt::print;
using fmt::color;

BuildCommand::BuildCommand()
    : Command("build", "Build project", {}) {}

void BuildCommand::run(unordered_map<string, string> params)
{
#ifdef _WIN32
        throw logic_error("Command has not been implemented for windows!");
#endif

    system("cmake ../ && make");
    print(fg(color::slate_gray), "\nProject built\n\n");
}