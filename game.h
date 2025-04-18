#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>
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
    std::string current_word;
    int word_length;
    int attacks;
    int parrys;
    int correct_attacks;
    int correct_parrys;
    bool battle_mode; // true for attack, false for parry
    sf::RenderWindow window;
    std::vector<std::string> terminalMessages;
    sf::Clock battleClock;

    sf::Font font;
    sf::RectangleShape roomBackground;
    sf::RectangleShape statsBackground;
    sf::RectangleShape terminalBackground;
    sf::RectangleShape inputBox;
    sf::Text roomText;
    sf::Text levelText;
    sf::Text goldText;
    sf::Text hpText;
    sf::Text strengthText;
    sf::Text intelligenceText;
    sf::Text agilityText;
    sf::Text armorText;
    sf::Text terminalText;
    sf::Text inputText;

    std::string userInput;

    const int totalSegments = 20;
    const float segmentWidth = 49;
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
    void enterDungeon(int size, int difficulty);
    void leaveDungeon();
    void checkRoomHazards();

    void startBattle();
    void handleBattleInput(const std::string &input);
    void changeCurrentWord(int length);
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