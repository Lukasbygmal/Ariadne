#pragma once
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

class Player;

class APIClient
{
private:
    std::string base_url;
    std::string api_key;
    CURL *curl;

    // Callback function for writing received data
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    // Helper function to make HTTP requests
    std::string makeRequest(const std::string &url, const std::string &method,
                            const std::string &json_data = "");

public:
    APIClient(const std::string &base_url, const std::string &api_key);
    ~APIClient();

    bool loadPlayer(Player &player, int user_id);
    bool savePlayer(const Player &player, int user_id);
    bool createPlayer(const Player &player);
    bool doesPlayerExist(const std::string &playerName);
    std::vector<nlohmann::json> getAllPlayers(int page = 1, int per_page = 10);
    std::vector<nlohmann::json> getHighscore();
};