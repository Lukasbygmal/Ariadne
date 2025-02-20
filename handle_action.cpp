#include <iostream>
#include "handle_action.h"

void handle_action(const Action &action, Dungeon &dungeon)
{
    if (action.verb == "go" && action.direction)
    {
        std::cout << "Go(" << *action.direction << ")" << std::endl;
        if (action.direction == "east")
        {
            dungeon.goEast();
        }
        if (action.direction == "west")
        {
            dungeon.goWest();
        }
        if (action.direction == "north")
        {
            dungeon.goNorth();
        }
        if (action.direction == "south")
        {
            dungeon.goSouth();
        }
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
