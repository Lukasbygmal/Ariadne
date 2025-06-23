#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>
#include <deque>
#include "player.h"
#include "dungeon.h"
#include "db_manager.h"

enum class GameMode
{
    MENU,
    DUNGEON,
    BATTLE
};

class Game
{
private:
    DatabaseManager dbManager;
    static std::mt19937 rng;
    static std::random_device rd;
    Player player;
    int user_id;
    Dungeon dungeon;
    GameMode mode;
    std::deque<std::string> word_queue;
    int word_length_attack;
    int word_length_parry;
    int attacks;
    int parrys;
    int correct_attacks;
    int correct_parrys;
    float round_time;
    bool battle_mode; // true for attack, false for parry
    sf::RenderWindow window;
    std::vector<std::string> terminalMessages;
    sf::Clock battleClock;

    sf::Font font;
    sf::RectangleShape roomBackground;
    sf::RectangleShape statsBackground;
    sf::RectangleShape perksBackground;
    sf::RectangleShape yetToDecideBackground;
    sf::RectangleShape terminalBackground;
    sf::RectangleShape inputBox;
    sf::Text titleText;
    sf::Text subTitleText;
    sf::Text levelText;
    sf::Text goldText;
    sf::Text hpText;
    sf::Text strengthText;
    sf::Text agilityText;
    sf::Text armorText;
    sf::Text terminalText;
    sf::Text inputText;

    std::string userInput;

    const int totalSegments = 20;
    const float segmentWidth = 39.2;
    const float segmentHeight = 10;

    void initializeUI();
    void initializeText();
    void updateUI();
    void renderXPBar();
    void damageMonster(int damage);
    void healMaxPlayer();

public:
    Game();

    void processEvents();
    void handleInput(const sf::Event &event);
    void handleCommand(const std::string &command);
    void addMessage(const std::string &message);
    void update();
    void render();
    void run();

    void changeMode(GameMode newMode);
    bool buyHP();
    bool buyStrength();
    bool buyArmor();
    bool buyAgility();
    void enterDungeon(std::string dungeon_name, std::string string_difficulty);
    void leaveDungeon();
    void checkRoomHazards();

    void startBattle();
    void handleBattleInput(const std::string &input);
    std::string createWord(int length);
    void resetWordQueue(int words, int length);
    void switchToDefense();
    void endRound();
    void endBattle();

    GameMode getMode() const;
    const Player &getPlayer() const;
    Player &getPlayer();
    Dungeon &getDungeon();
    const std::vector<std::string> &getMessages() const;
};

#endif