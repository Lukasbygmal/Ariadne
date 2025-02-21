#ifndef GAME_UI_H
#define GAME_UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Game;

class GameUI
{
public:
    GameUI();
    void initialize();
    void render(sf::RenderWindow &window, const Game &game);
    void handleInput(const sf::Event &event, Game &game);
    const std::string &getCurrentInput() const { return userInput; }

private:
    void initializeText();
    void updateStats(const Game &game);
    void renderXPBar(sf::RenderWindow &window, const Game &game);

    sf::Font font;
    std::string userInput;

    sf::RectangleShape roomBackground;
    sf::RectangleShape statsBackground;
    sf::RectangleShape terminalBackground;
    sf::RectangleShape inputBox;

    sf::Text roomText;
    sf::Text levelText;
    sf::Text hpText;
    sf::Text strengthText;
    sf::Text intelligenceText;
    sf::Text agilityText;
    sf::Text armorText;
    sf::Text terminalText;
    sf::Text inputText;

    static const int totalSegments = 20;
    static const int segmentWidth = 49;
    static const int segmentHeight = 10;
};

#endif