#include <iostream>
#include "handle_action.h"

bool handle_action(const Action &action, Game &game)
{
    switch (game.getMode())
    {
    case GameMode::MENU:
        if (action.verb == "enter" && action.object)
        {
            if (*action.object == "easy")
            {
                game.enterDungeon(4, 1);
                game.addMessage("You enter easy\n");
                return true;
            }
            else if (*action.object == "medium")
            {
                game.enterDungeon(6, 3);
                game.addMessage("You enter medium\n");
                return true;
            }
            else if (*action.object == "hard")
            {
                game.enterDungeon(8, 5);
                game.addMessage("You enter hard\n");
                return true;
            }
        }
        else if (action.verb == "buy" && action.object)
        {
            if (*action.object == "strength")
            {
                if (game.buyStrength())
                {
                    return true;
                }
                return false;
            }
            else if (*action.object == "hp")
            {

                if (game.buyHP())
                {
                    return true;
                }
                return false;
            }
            else if (*action.object == "armor")
            {
                if (game.buyArmor())
                {
                    return true;
                }
                return false;
            }
            else if (*action.object == "agility")
            {
                if (game.buyAgility())
                {
                    return true;
                }
                return false;
            }
        }

        return false;

    case GameMode::DUNGEON:
        if (action.verb == "go" && action.direction)
        {
            Dungeon &dungeon = game.getDungeon();
            if (*action.direction == "east")
            {
                if (dungeon.goEast())
                {
                    game.addMessage("You go east\n");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "west")
            {
                if (dungeon.goWest())
                {
                    game.addMessage("You go west\n");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "north")
            {
                if (dungeon.goNorth())
                {
                    game.addMessage("You go north\n");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "south")
            {
                if (dungeon.goSouth())
                {
                    game.addMessage("You go south\n");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
        }
        else if (action.verb == "investigate" && action.object)
        {
            std::cout << "Investigate(" << *action.object << ")" << std::endl;
            return true;
        }
        else if (action.verb == "open" && action.object)
        {
            std::cout << "Open(" << *action.object << ")" << std::endl;
            if (game.getDungeon().getCurrentRoom().getChest())
            {
                int gold = game.getDungeon().getCurrentRoom().openChest();
                game.getPlayer().receiveGold(gold);
                game.addMessage("You open the chest and recieve " + std::to_string(gold) + " gold\n");
                return true;
            }
            return false;
        }
        else if (action.verb == "search")
        {
            std::cout << "Search()" << std::endl;
            return true;
        }
        else if (action.verb == "rest")
        {
            std::cout << "Rest()" << std::endl;
            return true;
        }
        else if (action.verb == "pray")
        {
            std::cout << "Pray()" << std::endl;
            return true;
        }
        else if (action.verb == "exit")
        {
            game.leaveDungeon();
            std::cout << "ExitDungeon()" << std::endl;
            return true;
        }
        else
        {
            std::cout << "This should never happen!" << std::endl;
            return false;
        }

    case GameMode::BATTLE:
        game.handleBattleInput(action.verb);
        return true;
    }
    std::cout << "This should never happen!" << std::endl;
    return false; // this should never happen, if it does handle or parse is wrong
}
