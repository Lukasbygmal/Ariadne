#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "parse_input.h"
#include "handle_action.h"
#include "room.h"
#include "player.h"
#include "dungeon.h"

int main()
{
    Player player("legend27");
    Dungeon dungeon(6, 2);
    GameMode mode = DUNGEON;

    const int totalSegments = 20;
    const int segmentWidth = 49;
    const int segmentHeight = 10;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Ariadne");

    sf::Font font;
    if (!font.loadFromFile("new_athena.ttf"))
    {
        return -1;
    }

    sf::RectangleShape roomBackground(sf::Vector2f(600.f, 400.f));
    roomBackground.setFillColor(sf::Color(100, 100, 100, 200));
    roomBackground.setPosition(10.f, 170.f);

    sf::Text roomText;
    roomText.setFont(font);
    roomText.setCharacterSize(20);
    roomText.setFillColor(sf::Color::White);
    roomText.setPosition(20.f, 180.f);

    sf::RectangleShape statsBackground(sf::Vector2f(370.f, 210.f));
    statsBackground.setFillColor(sf::Color(100, 100, 100, 200));
    statsBackground.setPosition(620.f, 580.f);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(800.f, 590.f);

    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(800.f, 620.f);

    sf::Text strengthText;
    strengthText.setFont(font);
    strengthText.setCharacterSize(20);
    strengthText.setFillColor(sf::Color::White);
    strengthText.setPosition(640.f, 660.f);

    sf::Text intelligenceText;
    intelligenceText.setFont(font);
    intelligenceText.setCharacterSize(20);
    intelligenceText.setFillColor(sf::Color::White);
    intelligenceText.setPosition(840.f, 660.f);

    sf::Text agilityText;
    agilityText.setFont(font);
    agilityText.setCharacterSize(20);
    agilityText.setFillColor(sf::Color::White);
    agilityText.setPosition(640.f, 700.f);

    sf::Text armorText;
    armorText.setFont(font);
    armorText.setCharacterSize(20);
    armorText.setFillColor(sf::Color::White);
    armorText.setPosition(840.f, 700.f);

    std::string userInput;

    sf::RectangleShape terminalBackground(sf::Vector2f(600.f, 160.f));
    terminalBackground.setFillColor(sf::Color(50, 50, 50, 220));
    terminalBackground.setPosition(10.f, 580.f);

    sf::RectangleShape inputBox(sf::Vector2f(600.f, 40.f));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(10.f, 750.f);

    std::vector<std::string> terminalMessages = {
        "Welcome to the dungeon!\n"};

    sf::Text terminalText;
    terminalText.setFont(font);
    terminalText.setCharacterSize(18);
    terminalText.setFillColor(sf::Color::White);
    terminalText.setPosition(20.f, 590.f);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20.f, 760.f);

    roomText.setString(dungeon.getCurrentRoom().to_string());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !userInput.empty())
                {
                    userInput.pop_back();
                }
                else if (event.text.unicode == '\r')
                {
                    Action result("null");

                    if (parse_input(userInput, result))
                    {
                        terminalMessages.push_back("You " + userInput + "\n");
                        std::cout << "Success!\n";
                        handle_action(result, dungeon);
                        roomText.setString(dungeon.getCurrentRoom().to_string());
                    }
                    else
                    {
                        terminalMessages.push_back("You realize you cannot " + userInput + " and start questioning your sanity!\n");
                        std::cout << "Invalid command. Try again.\n";
                    }
                    userInput.clear();

                    if (terminalMessages.size() > 7)
                        terminalMessages.erase(terminalMessages.begin());
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 127)
                {
                    userInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        levelText.setString("Lvl " + std::to_string(player.getLevel()));
        levelText.setPosition(800.f - levelText.getGlobalBounds().width / 2, 590.f);

        hpText.setString(std::to_string(player.getCurrentHP()) + " / " + std::to_string(player.getMaxHP()) + "HP");
        hpText.setPosition(800.f - hpText.getGlobalBounds().width / 2, 620.f);

        strengthText.setString("Strength: " + std::to_string(player.getStrength()));
        intelligenceText.setString("Intelligence: " + std::to_string(player.getIntelligence()));
        agilityText.setString("Agility: " + std::to_string(player.getAgility()));
        armorText.setString("Armor: " + std::to_string(player.getArmor()));

        int filledSegments = static_cast<int>((static_cast<float>(player.getXP()) / player.getXPToMax()) * totalSegments);

        std::string terminalDisplay;
        for (const auto &msg : terminalMessages)
        {
            terminalDisplay += msg + "\n";
        }

        terminalText.setString(terminalDisplay);
        inputText.setString("> " + userInput);

        window.clear();

        for (int i = 0; i < totalSegments; i++)
        {
            sf::RectangleShape segment(sf::Vector2f(segmentWidth - 6, segmentHeight));
            segment.setPosition(10.f + i * segmentWidth, 10.f);

            if (i < filledSegments)
                segment.setFillColor(sf::Color(0, 255, 0));
            else
                segment.setFillColor(sf::Color(50, 50, 50));

            window.draw(segment);
        }
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

    return 0;
}