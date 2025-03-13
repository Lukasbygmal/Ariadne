#include <iostream>
#include "handle_action.h"

void handle_action(const Action &action, Game &game)
{
    switch (game.getMode())
    {
    case GameMode::MENU:
        if (action.verb == "enter" && action.object)
        {
            if (*action.object == "easy")
            {
                game.enterDungeon(4, 1);
            }
            else if (*action.object == "medium")
            {
                game.enterDungeon(6, 3);
            }
            else if (*action.object == "hard")
            {
                game.enterDungeon(8, 5);
            }
        }
        break;

    case GameMode::DUNGEON:
        if (action.verb == "go" && action.direction)
        {
            std::cout << "Go(" << *action.direction << ")" << std::endl;
            Dungeon &dungeon = game.getDungeon();
            if (*action.direction == "east")
            {
                dungeon.goEast();
            }
            else if (*action.direction == "west")
            {
                dungeon.goWest();
            }
            else if (*action.direction == "north")
            {
                dungeon.goNorth();
            }
            else if (*action.direction == "south")
            {
                dungeon.goSouth();
            }
            game.checkRoomHazards();
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
        else if (action.verb == "exit")
        {
            game.leaveDungeon();
            std::cout << "ExitDungeon()" << std::endl;
        }
        else
        {
            std::cout << "Invalid action." << std::endl;
        }
        break;

    case GameMode::BATTLE:
        game.handleBattleInput(action.verb);
        break;
    }
}
