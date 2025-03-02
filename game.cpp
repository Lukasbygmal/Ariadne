#include "game.h"
#include "handle_action.h"
#include <iostream>
#include "parse_input.h"

Game::Game()
    : player("legend27"), dungeon(6, 2), mode(GameMode::MENU), window(sf::VideoMode(1000, 800), "Ariadne")
{
    initializeUI();
}

void Game::initializeUI()
{
    if (!font.loadFromFile("new_athena.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }

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

void Game::initializeText()
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

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        handleInput(event);
    }
}

void Game::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b' && !userInput.empty())
        {
            userInput.pop_back();
        }
        else if (event.text.unicode == '\r')
        {
            handleCommand(userInput);
            userInput.clear();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            userInput += static_cast<char>(event.text.unicode);
        }
    }
}

void Game::handleCommand(const std::string &command)
{
    Action result("null");
    if (parse_input(command, result, mode))
    {
        addMessage("You " + command + "\n");
        std::cout << "Success!\n";
        handle_action(result, *this);
    }
    else
    {
        addMessage("You realize you cannot " + command + " and start questioning your sanity!\n");
        std::cout << "Invalid command. Try again.\n";
    }
}

void Game::addMessage(const std::string &message)
{
    terminalMessages.push_back(message);
    if (terminalMessages.size() > 7)
    {
        terminalMessages.erase(terminalMessages.begin());
    }
}

void Game::update()
{
    if (mode == GameMode::BATTLE)
    {
        sf::Time time = battleClock.getElapsedTime();
        sf::Time end_time = sf::seconds(10.0f);
        if (time > end_time)
        {
            endRound();
        }
    }
}

void Game::updateUI()
{
    if (mode == GameMode::DUNGEON)
    {
        roomText.setString(dungeon.getCurrentRoom().to_string());
    }
    else if (mode == GameMode::MENU)
    {
        roomText.setString("The Ink & Anvil Tavern");
    }
    else if (mode == GameMode::BATTLE)
    {
        roomText.setString("You are in a battle!");
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
    for (const auto &msg : terminalMessages)
    {
        terminalDisplay += msg + "\n";
    }
    terminalText.setString(terminalDisplay);
    inputText.setString("> " + userInput);
}

void Game::renderXPBar()
{
    int filledSegments = static_cast<int>((static_cast<float>(player.getXP()) / player.getXPToMax()) * totalSegments);

    for (int i = 0; i < totalSegments; i++)
    {
        sf::RectangleShape segment(sf::Vector2f(segmentWidth - 6, segmentHeight));
        segment.setPosition(10.f + i * segmentWidth, 10.f);
        segment.setFillColor(i < filledSegments ? sf::Color(0, 255, 0) : sf::Color(50, 50, 50));
        window.draw(segment);
    }
}

void Game::render()
{
    updateUI();

    window.clear();

    renderXPBar();

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

void Game::changeMode(GameMode newMode)
{
    mode = newMode;
}

void Game::enterDungeon(int size, int difficulty)
{
    dungeon = Dungeon(size, difficulty);
    changeMode(GameMode::DUNGEON);
}

void Game::leaveDungeon()
{
    changeMode(GameMode::MENU);
    addMessage("You escaped the dungeon\n");
}

void Game::checkRoomHazards()
{
    Room *current = dungeon.returnCurrentRoom();
    std::optional<int> trap = current->getTrap();
    if (trap)
    {
        int damage = *trap;
        player.receiveDamage(damage);
        addMessage("You triggered a trap! You take " + std::to_string(damage) + " damage (before armor).\n");
    }
    std::optional<Monster *> monster = current->getMonster();
    if (monster)
    {
        std::string monster_name = monster.value()->to_string();
        addMessage("A " + monster_name + " appears before you! Prepare for battle!\n");
        startBattle();
    }
}

void Game::startBattle()
{
    std::cout << "startBattle()\n";
    // start timer
    battleClock.restart();
    changeMode(GameMode::BATTLE);
    // change current_word
    changeCurrentWord();
    // correct_words = 0
    correct_attacks = 0;
    correct_parry = 0;
}

void Game::handleBattleInput(const std::string &input)
{
    if (input == current_word) // should eventually have based on a flag if it is backwards (parry)
    {
        correct_attacks++; // or correct_parry
        changeCurrentWord();
    }
}

void Game::changeCurrentWord()
{
    current_word = "test";
    std::cout << "Current_word ==" << current_word << "\n";
    // for now just make it into "test"
}

void Game::endRound()
{
    std::cout << "EndRound() \n";
    // need to think how i do with attack and defense phase
    for (int i = 0; i < correct_attacks; i++)
    {
        // deal damage to monster
    }
    int missed_parry = 5 - correct_parry; // need to think if i want 2 for loops, one for parry and one for succesful monster attack?
    for (int i = 0; i < missed_parry; i++)
    {
        // deal damage to player
    }

    // if player is dead (do something)

    // if monster hp ==0: endBattle()
    if (true) // true for now so that i can end battle
    {
        endBattle();
        return;
    }

    battleClock.restart();
    // restart timer
    correct_attacks = 0;
    correct_parry = 0;
}

void Game::endBattle()
{
    std::cout << "EndBattle() correct_attack:" << correct_attacks << "\n";
    changeMode(GameMode::DUNGEON);
    // change mode to dungeon
    // award xp
    // remove monster from room
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

GameMode Game::getMode() const
{
    return mode;
}

const Player &Game::getPlayer() const
{
    return player;
}

Dungeon &Game::getDungeon()
{
    return dungeon;
}