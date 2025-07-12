#include "parse_input.hpp"
#include <vector>
#include <sstream>
#include <cctype>

bool parse_input(const std::string &input, Action &action, GameMode mode)
{
    std::istringstream iss(input);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }

    if (words.empty())
        return false;

    switch (mode)
    {
    case GameMode::DUNGEON:
    {
        std::vector<std::string> valid_verbs = {"go", "open", "exit"}; // lowkey don't need this?
        std::vector<std::string> valid_directions = {"north", "west", "east", "south"};
        std::vector<std::string> valid_objects = {"chest"};

        if (std::find(valid_verbs.begin(), valid_verbs.end(), words[0]) == valid_verbs.end()) // like is it really worth doing this check?
            return false;

        if (words[0] == "go" && words.size() == 2 &&
            std::find(valid_directions.begin(), valid_directions.end(), words[1]) != valid_directions.end())
        {
            action = Action(words[0], words[1]);
            return true;
        }
        else if (words[0] == "open" && words.size() == 2 && words[1] == "chest")
        {
            action = Action(words[0], std::nullopt, words[1]);
            return true;
        }
        else if ((words[0] == "exit") && words.size() == 1)
        {
            action = Action(words[0]);
            return true;
        }
        return false;
    }

    case GameMode::MENU:
    {
        std::vector<std::string> valid_commands = {"enter", "buy"}; // lowkey  don't need this?!
        std::vector<std::string> valid_dungeons = {
            "thyra", "nykos", "kalon", "draka",
            "zarex", "lethi", "teren", "xenor",
            "myros", "velor", "elvon", "oryne",
            "phyra", "voran", "eriph", "zeryn"};

        std::vector<std::string> valid_items = {"strength", "hp", "armor", "agility"};

        if (words.size() == 2 &&
            words[0] == "enter" &&
            std::find(valid_dungeons.begin(), valid_dungeons.end(), words[1]) != valid_dungeons.end())
        {
            action = Action(words[0], words[1]);
            return true;
        }
        else if (words.size() == 3 && words[0] == "buy" && std::find(valid_items.begin(), valid_items.end(), words[2]) != valid_items.end())
        {
            action = Action(words[0], words[1], words[2]);
            return true;
        }
        return false;
    }

    case GameMode::BATTLE:
    {
        action = Action(words[0], std::nullopt, std::nullopt);
        return true;
    }

    default:
        return false;
    }
}