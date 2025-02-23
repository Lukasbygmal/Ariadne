#include "game_ui.h"
#include "game.h"
#include <iostream>

GameUI::GameUI()
{
    if (!font.loadFromFile("new_athena.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }
}

void GameUI::initialize()
{
    roomBackground.setSize(sf::Vector2f(600.f, 400.f));
    roomBackground.setFillColor(sf::Color(100, 100, 100, 200));
    roomBackground.setPosition(10.f, 170.f);

    statsBackground.setSize(sf::Vector2f(370.f, 210.f));
    statsBackground.setFillColor(sf::Color(100, 100, 100, 200));
    statsBackground.setPosition(620.f, 580.f);

    terminalBackground.setSize(sf::Vector2f(600.f, 160.f));
    terminalBackground.setFillColor(sf::Color(50, 50, 50, 220));
    terminalBackground.setPosition(10.f, 580.f);

    inputBox.setSize(sf::Vector2f(600.f, 40.f));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(10.f, 750.f);

    initializeText();
}

void GameUI::initializeText()
{
    roomText.setFont(font);
    roomText.setCharacterSize(20);
    roomText.setFillColor(sf::Color::White);
    roomText.setPosition(20.f, 180.f);

    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(800.f, 590.f);

    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(800.f, 620.f);

    strengthText.setFont(font);
    strengthText.setCharacterSize(20);
    strengthText.setFillColor(sf::Color::White);
    strengthText.setPosition(640.f, 660.f);

    intelligenceText.setFont(font);
    intelligenceText.setCharacterSize(20);
    intelligenceText.setFillColor(sf::Color::White);
    intelligenceText.setPosition(840.f, 660.f);

    agilityText.setFont(font);
    agilityText.setCharacterSize(20);
    agilityText.setFillColor(sf::Color::White);
    agilityText.setPosition(640.f, 700.f);

    armorText.setFont(font);
    armorText.setCharacterSize(20);
    armorText.setFillColor(sf::Color::White);
    armorText.setPosition(840.f, 700.f);

    terminalText.setFont(font);
    terminalText.setCharacterSize(18);
    terminalText.setFillColor(sf::Color::White);
    terminalText.setPosition(20.f, 590.f);

    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20.f, 760.f);
}

void GameUI::handleInput(const sf::Event &event, Game &game)
{
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b' && !userInput.empty())
        {
            userInput.pop_back();
        }
        else if (event.text.unicode == '\r')
        {
            game.handleCommand(userInput);
            userInput.clear();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            userInput += static_cast<char>(event.text.unicode);
        }
    }
}

void GameUI::updateStats(const Game &game)
{
    const Player &player = game.getPlayer();
    if (game.getMode() == GameMode::DUNGEON)
    {
        roomText.setString(game.getDungeon().getCurrentRoom().to_string());
    }
    else
    {
        roomText.setString("The Ink & Anvil Tavern");
    }

    levelText.setString("Lvl " + std::to_string(player.getLevel()));
    levelText.setPosition(800.f - levelText.getGlobalBounds().width / 2, 590.f);

    hpText.setString(std::to_string(player.getCurrentHP()) + " / " + std::to_string(player.getMaxHP()) + "HP");
    hpText.setPosition(800.f - hpText.getGlobalBounds().width / 2, 620.f);

    strengthText.setString("Strength: " + std::to_string(player.getStrength()));
    intelligenceText.setString("Intelligence: " + std::to_string(player.getIntelligence()));
    agilityText.setString("Agility: " + std::to_string(player.getAgility()));
    armorText.setString("Armor: " + std::to_string(player.getArmor()));

    std::string terminalDisplay;
    for (const auto &msg : game.getMessages())
    {
        terminalDisplay += msg + "\n";
    }
    terminalText.setString(terminalDisplay);
    inputText.setString("> " + userInput);
}

void GameUI::renderXPBar(sf::RenderWindow &window, const Game &game)
{
    const Player &player = game.getPlayer();
    int filledSegments = static_cast<int>((static_cast<float>(player.getXP()) / player.getXPToMax()) * totalSegments);

    for (int i = 0; i < totalSegments; i++)
    {
        sf::RectangleShape segment(sf::Vector2f(segmentWidth - 6, segmentHeight));
        segment.setPosition(10.f + i * segmentWidth, 10.f);
        segment.setFillColor(i < filledSegments ? sf::Color(0, 255, 0) : sf::Color(50, 50, 50));
        window.draw(segment);
    }
}

void GameUI::render(sf::RenderWindow &window, const Game &game)
{
    updateStats(game);

    window.clear();

    renderXPBar(window, game);

    window.draw(roomBackground);
    window.draw(roomText);
    window.draw(statsBackground);
    window.draw(levelText);
    window.draw(hpText);
    window.draw(strengthText);
    window.draw(intelligenceText);
    window.draw(agilityText);
    window.draw(armorText);
    window.draw(terminalBackground);
    window.draw(terminalText);
    window.draw(inputBox);
    window.draw(inputText);

    window.display();
}