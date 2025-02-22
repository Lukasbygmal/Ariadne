#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "dungeon.h"
#include "parse_input.h"
#include "game_ui.h"
#include <SFML/Graphics.hpp>
#include <string>

class Game
{
public:
    Game();
    void run();

    const Player &getPlayer() const { return player; }
    const Dungeon &getDungeon() const { return dungeon; }
    Dungeon &getDungeon() { return dungeon; }
    GameMode getMode() const { return mode; }

    void handleCommand(const std::string &command);
    void addMessage(const std::string &message);
    const std::vector<std::string> &getMessages() const { return terminalMessages; }
    void enterDungeon(int size, int difficulty);
    void leaveDungeon();

private:
    void changeMode(GameMode newMode);
    void processEvents();
    void update();

    Player player;
    Dungeon dungeon;
    GameMode mode;
    GameUI ui;
    std::vector<std::string> terminalMessages;
    sf::RenderWindow window;
};

#endif