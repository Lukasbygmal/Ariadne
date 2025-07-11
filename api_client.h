/**
 * @file api_client.h
 * @brief HTTP client for player data management via REST API
 */
#pragma once
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <memory>

class Player;
/**
 * @class APIClient
 * @brief HTTP client for managing player data through REST API operations
 *
 * This class provides a C++ interface for interacting with a REST API that manages
 * player data including creation, retrieval, updates, and queries. It uses libcurl
 * for HTTP communications and nlohmann/json for JSON serialization/deserialization.
 *
 * The client handles authentication via API keys and provides methods for all
 * standard CRUD operations on player entities.
 */
class APIClient
{
private:
    std::string base_url;
    std::string api_key;
    CURL *curl;

    /**
     * @brief Callback function for writing received HTTP response data
     *
     * This static callback is used by libcurl to write response data into
     * a string buffer. Conforms to libcurl's CURLOPT_WRITEFUNCTION signature.
     *
     * @param contents Pointer to received data chunk
     * @param size Size of each data element
     * @param nmemb Number of data elements
     * @param userp User-defined pointer (expects std::string*)
     * @return Total bytes processed (size * nmemb)
     */
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    /**
     * @brief Performs HTTP request with specified method and optional JSON payload
     *
     * Internal helper method that handles the common HTTP request logic including
     * header setup, authentication, and response handling.
     *
     * @param url Complete URL for the request
     * @param method HTTP method ("GET", "POST", "PUT")
     * @param json_data Optional JSON payload for POST/PUT requests
     * @return Response body as string, empty string on failure
     * @note Logs errors to std::cerr on failure
     */
    std::string makeRequest(const std::string &url, const std::string &method,
                            const std::string &json_data = "");

public:
    /**
     * @brief Constructs APIClient with base URL and API key
     *
     * Initializes libcurl and sets up the HTTP client for making API requests.
     * The curl handle is created during construction.
     *
     * @param base_url Base URL for API endpoints (e.g., "https://api.example.com")
     * @param api_key Authentication key for API access
     * @note Logs initialization errors to std::cerr
     */
    APIClient(const std::string &base_url, const std::string &api_key);
    ~APIClient();

    /**
     * @brief Loads player data from API by user ID
     *
     * Retrieves player information from the API and populates the provided
     * Player object. The player's HP is automatically healed to maximum.
     *
     * @param player Reference to Player object to populate with loaded data
     * @param user_id Unique identifier for the player to load
     * @return true if player was successfully loaded, false on failure
     * @note Logs API and parsing errors to std::cerr
     */
    bool loadPlayer(Player &player, int user_id);

    /**
     * @brief Saves player data to API by user ID
     *
     * Sends current player state to the API for persistence using HTTP PUT.
     * All player attributes (name, level, XP, gold, stats) are saved.
     *
     * @param player Const reference to Player object to save
     * @param user_id Unique identifier for the player record to update
     * @return true if player was successfully saved, false on failure
     * @note Logs API and parsing errors to std::cerr
     */
    bool savePlayer(const Player &player, int user_id);

    /**
     * @brief Creates a new player record in the API
     *
     * Sends player data to create a new player entry using HTTP POST.
     * The API should handle user ID assignment for new players.
     *
     * @param player Const reference to Player object to create
     * @return true if player was successfully created, false on failure
     * @note Logs API and parsing errors to std::cerr
     */
    bool createPlayer(const Player &player);

    /**
     * @brief Checks if a player with the given name exists
     *
     * Queries the API to determine if a player with the specified name
     * already exists in the system.
     *
     * @param playerName Name of the player to check for existence
     * @return true if player exists, false if not found or on error
     * @note Logs API and parsing errors to std::cerr
     */
    bool doesPlayerExist(const std::string &playerName);

    /**
     * @brief Retrieves paginated list of all players
     *
     * Fetches a page of player records from the API with pagination support.
     *
     * @param page Page number to retrieve (1-based indexing)
     * @param per_page Number of players per page
     * @return Vector of JSON objects representing player data, empty on failure
     * @note Currently not used in the application
     * @note Logs API and parsing errors to std::cerr
     * @note Not currently used
     */
    std::vector<nlohmann::json> getAllPlayers(int page = 1, int per_page = 10);

    /**
     * @brief Retrieves high score leaderboard data
     *
     * Fetches the current high score rankings from the API.
     *
     * @return Vector of JSON objects representing high score entries, empty on failure
     * @note Currently not used in the application
     * @note Logs API and parsing errors to std::cerr
     * @note Not currently used
     */
    std::vector<nlohmann::json> getHighscore();
};