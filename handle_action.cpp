#include <iostream>
#include "handle_action.h"

void handle_action(const Action &action)
{
    if (action.verb == "go" && action.direction)
    {
        std::cout << "Go(" << *action.direction << ")" << std::endl;
    }
    else if (action.verb == "investigate" && action.object)
    {
        std::cout << "Investigate(" << *action.object << ")" << std::endl;
    }
    else if (action.verb == "open" && action.object)
    {
        std::cout << "Open(" << *action.object << ")" << std::endl;
    }
    else if (action.verb == "search")
    {
        std::cout << "Search()" << std::endl;
    }
    else if (action.verb == "rest")
    {
        std::cout << "Rest()" << std::endl;
    }
    else if (action.verb == "pray")
    {
        std::cout << "Pray()" << std::endl;
    }
    else
    {
        std::cout << "Invalid action." << std::endl;
    }
}
