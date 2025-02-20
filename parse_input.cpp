#include "parse_input.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

bool parse_input(const std::string &input, Action &action)
{
    std::vector<std::string> valid_verbs = {"go", "rest", "investigate", "search", "open", "pray"};
    std::vector<std::string> valid_directions = {"north", "west", "east", "south"};
    std::vector<std::string> valid_objects = {"chest", "engraving", "corpse"};

    std::istringstream iss(input);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }

    if (words.empty() || std::find(valid_verbs.begin(), valid_verbs.end(), words[0]) == valid_verbs.end())
    {
        return false;
    }

    if (words[0] == "go" && words.size() == 2 && std::find(valid_directions.begin(), valid_directions.end(), words[1]) != valid_directions.end())
    {
        action = Action(words[0], words[1]);
        return true;
    }
    else if (words[0] == "investigate" && words.size() == 2 && std::find(valid_objects.begin(), valid_objects.end(), words[1]) != valid_objects.end())
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
