#include "handle_action.h"
#include <stdexcept>

bool handle_action(const Action &action, Game &game)
{
    switch (game.getMode())
    {
    case GameMode::MENU:
        if (action.verb == "enter" && action.direction && action.object)
        {
            game.enterDungeon(*action.direction, *action.object);
            game.addMessage("You enter " + *action.direction + " " + *action.object);
            return true;
        }
        else if (action.verb == "buy" && action.direction && action.object)
        {
            // This is to make sure it is a number
            try
            {
                int amount = std::stoi(*action.direction);
                if (amount <= 0 || amount >= 1000)
                {
                    return false; // Invalid number range
                }

                if (*action.object == "strength")
                {
                    if (game.buyStrength(amount))
                    {
                        return true;
                    }
                    return false;
                }
                else if (*action.object == "hp")
                {
                    if (game.buyHP(amount))
                    {
                        return true;
                    }
                    return false;
                }
                else if (*action.object == "armor")
                {
                    if (game.buyArmor(amount))
                    {
                        return true;
                    }
                    return false;
                }
                else if (*action.object == "agility")
                {
                    if (game.buyAgility(amount))
                    {
                        return true;
                    }
                    return false;
                }
            }
            catch (const std::exception &)
            {
                return false; // Invalid number format
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
                    game.addMessage("You go east");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "west")
            {
                if (dungeon.goWest())
                {
                    game.addMessage("You go west");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "north")
            {
                if (dungeon.goNorth())
                {
                    game.addMessage("You go north");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
            else if (*action.direction == "south")
            {
                if (dungeon.goSouth())
                {
                    game.addMessage("You go south");
                    game.checkRoomHazards();
                    return true;
                }
                return false;
            }
        }
        else if (action.verb == "open" && action.object)
        {
            if (game.getDungeon().getCurrentRoom().getChest())
            {
                int gold = game.getDungeon().getCurrentRoom().openChest();
                game.getPlayer().receiveGold(gold);
                game.addMessage("You open the chest and recieve " + std::to_string(gold) + " gold");
                return true;
            }
            return false;
        }
        else if (action.verb == "exit")
        {
            game.leaveDungeon();
            return true;
        }
        else
        {
            return false;
        }

    case GameMode::BATTLE:
        game.handleBattleInput(action.verb); // All input treated as typing for battle system
        return true;                         // Battle input always "succeeds"
    }
    return false; // this should never happen, if it does handle or parse is wrong
}
