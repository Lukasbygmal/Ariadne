#ifndef GAME_H
#define GAME_H
/**
 * @file game.h
 * @brief Core game engine managing UI, state, and gameplay mechanics
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>
#include <deque>
#include "player.h"
#include "dungeon.h"
#include "api_client.h"

/**
 * @enum GameMode
 * @brief Represents the current state of the game
 */

enum class GameMode
{
    MENU,
    DUNGEON,
    BATTLE
};

/**
 * @class Game
 * @brief Main game controller handling UI, state management, and gameplay flow
 *
 * This class orchestrates the entire game experience, managing:
 * - Player data persistence through API calls
 * - SFML-based UI rendering and input handling
 * - Game state transitions (menu, dungeon, battle)
 * - Real-time typing-based combat system
 * - Dungeon exploration mechanics
 *
 * The game uses a command-line style interface within an SFML window,
 * with different UI layouts for each game mode.
 */

class Game
{
private:
    APIClient apiClient;
    static std::mt19937 rng;
    static std::random_device rd;
    Player player;
    int user_id;
    Dungeon dungeon;
    GameMode mode;

    // Battle system state
    std::deque<std::string> word_queue;
    int word_length_attack;
    int word_length_parry;
    int attacks;
    int parrys;
    int correct_attacks;
    int correct_parrys;
    float round_time;
    bool battle_mode; // true for attack, false for parry

    // SFML UI components
    sf::RenderWindow &window;
    std::vector<std::string> terminalMessages;
    sf::Clock battleClock;

    // Fonts
    sf::Font titleFont;
    sf::Font textFont;

    // UI Background shapes
    sf::RectangleShape roomBackground;
    sf::RectangleShape statsBackground;
    sf::RectangleShape perksBackground;
    sf::RectangleShape yetToDecideBackground;
    sf::RectangleShape terminalBackground;
    sf::RectangleShape inputBox;

    // UI Text elements
    sf::Text titleText;
    sf::Text subTitleText;
    sf::Text battleText;
    sf::Text levelText;
    sf::Text goldText;
    sf::Text hpText;
    sf::Text strengthText;
    sf::Text agilityText;
    sf::Text armorText;
    sf::Text perksText;
    sf::Text yetToDecideText;
    sf::Text listDungeonsText;
    sf::Text terminalText;
    sf::Text inputText;

    // Map rendering
    sf::RectangleShape mapBackground;
    std::vector<std::vector<sf::RectangleShape>> mapTiles;
    const float MAP_DISPLAY_SIZE = 310.0f;

    // XP bar constants
    const int totalSegments = 20;
    const float segmentWidth = 39.2;
    const float segmentHeight = 10;

    std::string userInput;

    /**
     * @brief Initialize all UI elements and load fonts
     * @throws std::runtime_error if fonts cannot be loaded
     */
    void initializeUI();

    /**
     * @brief Set up text properties for all UI text elements
     */
    void initializeText();

    /**
     * @brief Initialize map display based on current dungeon
     */
    void initializeMap();

    /**
     * @brief Update UI text content based on current game state
     */
    void updateUI();

    /**
     * @brief Render player XP bar as segmented progress indicator
     */
    void renderXPBar();

    /**
     * @brief Apply damage to current room's monster
     * @param damage Amount of damage to deal
     */
    void damageMonster(int damage);

    /**
     * @brief Restore player to maximum HP
     */
    void healMaxPlayer();

    /**
     * @brief Render dungeon map with room types and player position
     * @note Magenta is used for debug
     */
    void renderMap();

    /**
     * @brief Display error message and exit application
     * @param message Error message to display
     */
    void showErrorAndExit(const std::string &message);

public:
    /**
     * @brief Construct game instance and load player data
     * @param user_id Unique player identifier for data persistence
     * @param window Reference to SFML render window
     * @throws std::runtime_error if player data cannot be loaded
     */
    Game(int user_id, sf::RenderWindow &window);

    /**
     * @brief Process all pending SFML events
     *
     * Handles window close events and delegates input to handleInput()
     */
    void processEvents();

    /**
     * @brief Handle individual SFML input events
     * @param event SFML event to process
     */
    void handleInput(const sf::Event &event);

    /**
     * @brief Process user command string
     * @param command User input string to parse and execute
     */
    void handleCommand(const std::string &command);

    /**
     * @brief Add message to terminal display with length limit
     * @param message Message to add (truncated if > 55 characters)
     */
    void addMessage(const std::string &message);

    /**
     * @brief Update game state based on current mode
     *
     * Primarily handles battle timing and round transitions
     */
    void update();

    /**
     * @brief Render complete game UI
     *
     * Updates UI content and draws all visual elements
     */
    void render();

    /**
     * @brief Main game loop - runs until window closes
     */
    void run();

    /**
     * @brief Change current game mode
     * @param newMode Target game mode
     */
    void changeMode(GameMode newMode);

    /**
     * @brief Purchase HP stat increase
     * @param amount Number of HP points to buy
     * @return true if purchase successful, false if insufficient gold
     * @note uses magic number for price
     */
    bool buyHP(int amount);

    /**
     * @brief Purchase strength stat increase
     * @param amount Number of strength points to buy
     * @return true if purchase successful, false if insufficient gold
     * @note uses magic number for price
     */
    bool buyStrength(int amount);

    /**
     * @brief Purchase armor stat increase
     * @param amount Number of armor points to buy
     * @return true if purchase successful, false if insufficient gold
     * @note uses magic number for price
     */
    bool buyArmor(int amount);

    /**
     * @brief Purchase agility stat increase
     * @param amount Number of agility points to buy
     * @return true if purchase successful, false if insufficient gold
     * @note uses magic number for price
     */
    bool buyAgility(int amount);

    /**
     * @brief Enter specified dungeon with difficulty
     * @param dungeon_name Name of dungeon to enter
     * @param string_difficulty Difficulty level ("easy", "medium", "hard")
     */
    void enterDungeon(std::string dungeon_name, std::string string_difficulty);

    /**
     * @brief Exit current dungeon and return to menu
     *
     * Saves player data and heals player to maximum
     */
    void leaveDungeon();

    /**
     * @brief Check and handle room hazards (traps, monsters)
     *
     * Processes trap damage and initiates combat if monster present
     */
    void checkRoomHazards();

    /**
     * @brief Initialize battle state and start combat
     *
     * Sets up word queue, timing, and battle parameters based on dungeon difficulty
     * @note uses magic numbers for round time scaling with difficulty and agility
     */
    void startBattle();

    /**
     * @brief Process user input during battle
     * @param input User typed string to check against current word
     */
    void handleBattleInput(const std::string &input);

    /**
     * @brief Generate random word for battle system
     * @param length Number of characters in generated word
     * @return Random alphanumeric string
     */
    std::string createWord(int length);

    /**
     * @brief Reset word queue with new random words
     * @param words Number of words to generate
     * @param length Length of each word
     */
    void resetWordQueue(int words, int length);

    /**
     * @brief Switch from attack to parry phase in battle
     */
    void switchToDefense();

    /**
     * @brief End current battle round and calculate damage
     *
     * Processes attack/parry results and determines battle outcome
     */
    void endRound();

    /**
     * @brief End battle and return to dungeon exploration
     *
     * Awards XP and handles level progression
     */
    void endBattle();

    /**
     * @brief Get current game mode
     * @return Current GameMode enum value
     */
    GameMode getMode() const;

    /**
     * @brief Get read-only reference to player
     * @return const reference to Player object
     */
    const Player &getPlayer() const;

    /**
     * @brief Get mutable reference to player
     * @return reference to Player object
     */
    Player &getPlayer();

    /**
     * @brief Get reference to current dungeon
     * @return reference to Dungeon object
     */
    Dungeon &getDungeon();

    /**
     * @brief Get terminal message history
     * @return const reference to message vector
     */
    const std::vector<std::string> &getMessages() const;
};

#endif