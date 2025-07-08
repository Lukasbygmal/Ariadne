#include "api_client.h"
#include "player.h"

APIClient::APIClient(const std::string &base_url, const std::string &api_key)
    : base_url(base_url), api_key(api_key), curl(nullptr)
{

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "Failed to initialize CURL" << std::endl;
    }
}

APIClient::~APIClient()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t APIClient::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string APIClient::makeRequest(const std::string &url, const std::string &method,
                                   const std::string &json_data)
{
    if (!curl)
    {
        return "";
    }

    std::string response_string;
    struct curl_slist *headers = nullptr;

    // Set headers
    std::string auth_header = "X-API-Key: " + api_key;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set HTTP method
    if (method == "POST")
    {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
    }
    else if (method == "PUT")
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
    }
    else if (method == "GET")
    {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);

    if (res != CURLE_OK)
    {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    return response_string;
}

bool APIClient::loadPlayer(Player &player, int user_id)
{
    std::string url = base_url + "/api/player/" + std::to_string(user_id);
    std::string response = makeRequest(url, "GET");

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return false;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);

        if (!json_response["success"].get<bool>())
        {
            std::cerr << "API Error: " << json_response["error"].get<std::string>() << std::endl;
            return false;
        }

        auto player_data = json_response["player"];

        // Create player object with data from API
        Player loadedPlayer(
            player_data["name"].get<std::string>(),
            player_data["lvl"].get<int>(),
            player_data["xp"].get<int>(),
            player_data["gold"].get<int>(),
            player_data["max_hp"].get<int>(),
            player_data["strength_stat"].get<int>(),
            player_data["agility_stat"].get<int>(),
            player_data["armor_stat"].get<int>());

        loadedPlayer.healToMax();
        player = loadedPlayer;

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return false;
    }
}

bool APIClient::savePlayer(const Player &player, int user_id)
{
    std::string url = base_url + "/api/player/" + std::to_string(user_id);

    // Create JSON payload
    nlohmann::json json_data;
    json_data["name"] = player.getName();
    json_data["lvl"] = player.getLevel();
    json_data["xp"] = player.getXP();
    json_data["gold"] = player.getGold();
    json_data["max_hp"] = player.getMaxHP();
    json_data["strength_stat"] = player.getStrength();
    json_data["agility_stat"] = player.getAgility();
    json_data["armor_stat"] = player.getArmor();

    std::string response = makeRequest(url, "PUT", json_data.dump());

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return false;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);

        if (!json_response["success"].get<bool>())
        {
            std::cerr << "API Error: " << json_response["error"].get<std::string>() << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return false;
    }
}

bool APIClient::createPlayer(const Player &player)
{
    std::string url = base_url + "/api/player";

    // Create JSON payload
    nlohmann::json json_data;
    json_data["name"] = player.getName();
    json_data["lvl"] = player.getLevel();
    json_data["xp"] = player.getXP();
    json_data["gold"] = player.getGold();
    json_data["max_hp"] = player.getMaxHP();
    json_data["strength_stat"] = player.getStrength();
    json_data["agility_stat"] = player.getAgility();
    json_data["armor_stat"] = player.getArmor();

    std::string response = makeRequest(url, "POST", json_data.dump());

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return false;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);

        if (!json_response["success"].get<bool>())
        {
            std::cerr << "API Error: " << json_response["error"].get<std::string>() << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return false;
    }
}

bool APIClient::doesPlayerExist(const std::string &playerName)
{
    std::string url = base_url + "/api/player/exists/" + playerName;
    std::string response = makeRequest(url, "GET");

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return false;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);
        return json_response["exists"].get<bool>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return false;
    }
}
// Not used atm
std::vector<nlohmann::json> APIClient::getAllPlayers(int page, int per_page)
{
    std::vector<nlohmann::json> players;
    std::string url = base_url + "/api/players?page=" + std::to_string(page) + "&per_page=" + std::to_string(per_page);
    std::string response = makeRequest(url, "GET");

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return players;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);
        if (!json_response["success"].get<bool>())
        {
            std::cerr << "API Error: " << json_response["error"].get<std::string>() << std::endl;
            return players;
        }
        players = json_response["players"].get<std::vector<nlohmann::json>>();
        return players;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return players;
    }
}
// Not used atm
std::vector<nlohmann::json> APIClient::getHighscore()
{
    std::vector<nlohmann::json> highscores;
    std::string url = base_url + "/api/highscore";
    std::string response = makeRequest(url, "GET");

    if (response.empty())
    {
        std::cerr << "Failed to get response from API" << std::endl;
        return highscores;
    }

    try
    {
        nlohmann::json json_response = nlohmann::json::parse(response);
        if (!json_response["success"].get<bool>())
        {
            std::cerr << "API Error: " << json_response["error"].get<std::string>() << std::endl;
            return highscores;
        }
        highscores = json_response["highscores"].get<std::vector<nlohmann::json>>();
        return highscores;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        return highscores;
    }
}