#include "parse_input.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

bool parse_input(const std::string &input, std::tuple<bool, bool> &result)
{
    std::vector<std::string> valid_verbs = {"go", "rest", "investigate", "search", "open", "pray",};
    std::vector<std::string> valid_directions = {"left", "right", "middle"};
    std::vector<std::string> valid_objects = {"chest", "engraving", "corpse"};

    std::istringstream iss(input);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }

    if (words.empty())
    {
        result = std::make_tuple(false, true);
        return false;
    }

    if (std::find(valid_verbs.begin(), valid_verbs.end(), words[0]) == valid_verbs.end())
    {
        result = std::make_tuple(false, true);
        return false;
    }

    // Looks ugly with so many if statements but i suppose this is what "AI" is :P

    if (words[0] == "go")
    {
        if (words.size() != 2 || std::find(valid_directions.begin(), valid_directions.end(), words[1]) == valid_directions.end())
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }
    else if (words[0] == "investigate")
    {
        if (words.size() != 2 || std::find(valid_objects.begin(), valid_objects.end(), words[1]) == valid_objects.end())
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }
    else if (words[0] == "open")
    {
        if (words.size() != 2 || words[1] != "chest")
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }
    else if (words[0] == "search")
    {
        if (words.size() != 1)
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }
    else if (words[0] == "rest")
    {
        if (words.size() != 1)
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }
    else if (words[0] == "pray")
    {
        if (words.size() != 1)
        {
            result = std::make_tuple(false, true);
            return false;
        }
    }

    result = std::make_tuple(true, true);
    return true;
}
