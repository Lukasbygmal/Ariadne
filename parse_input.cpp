#include "parse_input.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
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
        std::vector<std::string> valid_verbs = {"go", "rest", "investigate", "search", "open", "pray"};
        std::vector<std::string> valid_directions = {"north", "west", "east", "south"};
        std::vector<std::string> valid_objects = {"chest", "engraving", "corpse"};

        if (std::find(valid_verbs.begin(), valid_verbs.end(), words[0]) == valid_verbs.end())
            return false;

        if (words[0] == "go" && words.size() == 2 &&
            std::find(valid_directions.begin(), valid_directions.end(), words[1]) != valid_directions.end())
        {
            action = Action(words[0], words[1]);
            return true;
        }
        else if (words[0] == "investigate" && words.size() == 2 &&
                 std::find(valid_objects.begin(), valid_objects.end(), words[1]) != valid_objects.end())
        {
            action = Action(words[0], std::nullopt, words[1]);
            return true;
        }
        else if (words[0] == "open" && words.size() == 2 && words[1] == "chest")
        {
            action = Action(words[0], std::nullopt, words[1]);
            return true;
        }
        else if ((words[0] == "search" || words[0] == "rest" || words[0] == "pray") && words.size() == 1)
        {
            action = Action(words[0]);
            return true;
        }
        return false;
    }

    case GameMode::MENU:
    {
        std::vector<std::string> valid_commands = {"enter"};
        std::vector<std::string> valid_dungeons = {"easy", "medium", "hard"};

        if (words.size() == 2 &&
            words[0] == "enter" &&
            std::find(valid_dungeons.begin(), valid_dungeons.end(), words[1]) != valid_dungeons.end())
        {
            action = Action(words[0], std::nullopt, words[1]);
            return true;
        }
        return false;
    }

    case GameMode::BATTLE:
    {
        action = Action(words[0],std::nullopt, std::nullopt);
        return false;
    }

    default:
        return false;
    }
}