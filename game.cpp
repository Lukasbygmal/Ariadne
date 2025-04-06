#include "game.h"
#include "handle_action.h"
#include <iostream>
#include <string>
#include "parse_input.h"

std::random_device Game::rd;
std::mt19937 Game::rng(Game::rd());

Game::Game()
    : player("Gimli"), user_id(3), dungeon(6, 2), mode(GameMode::MENU), window(sf::VideoMode(1000, 800), "Ariadne"), dbManager("localhost", "root", "", "ariadne")
{
    if (!dbManager.loadPlayer(player, 3))
    {
        std::cout << "Failed to load player data!" << std::endl;
    }
    else
    {
        std::cout << "Player data loaded successfully!" << std::endl;
    }

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
    roomText.setCharacterSize(28);
    roomText.setFillColor(sf::Color::White);
    roomText.setPosition(20.f, 180.f);

    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(790.f, 590.f);

    goldText.setFont(font);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(790.f, 640.f);

    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(760.f, 620.f);

    strengthText.setFont(font);
    strengthText.setCharacterSize(20);
    strengthText.setFillColor(sf::Color::White);
    strengthText.setPosition(640.f, 680.f);

    intelligenceText.setFont(font);
    intelligenceText.setCharacterSize(20);
    intelligenceText.setFillColor(sf::Color::White);
    intelligenceText.setPosition(840.f, 680.f);

    agilityText.setFont(font);
    agilityText.setCharacterSize(20);
    agilityText.setFillColor(sf::Color::White);
    agilityText.setPosition(640.f, 720.f);

    armorText.setFont(font);
    armorText.setCharacterSize(20);
    armorText.setFillColor(sf::Color::White);
    armorText.setPosition(840.f, 720.f);

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
        std::cout << "Passed parse_input\n";
        if (handle_action(result, *this))
        {
            std::cout << "Passed handle_action\n";
        }
        else
        {
            addMessage("Can't " + command + " now\n");
        }
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
        sf::Time end_time = sf::seconds(12.0f);
        if (time > end_time || (correct_attacks >= attacks && battle_mode) || (correct_parrys >= parrys && !battle_mode))
        {
            if (!battle_mode)
            {
                endRound();
            }
            else
            {
                switchToDefense();
            }
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
        roomText.setString("The Ink & Anvil Tavern \n \n You can buy stats!");
    }
    else if (mode == GameMode::BATTLE)
    {
        if (battle_mode)
        {
            roomText.setString(current_word);
            roomText.setFillColor(sf::Color::Green);
        }

        if (!battle_mode)
        {
            roomText.setString(current_word);
            roomText.setFillColor(sf::Color::Red);
        }
    }

    levelText.setString("Lvl " + std::to_string(player.getLevel()));
    goldText.setString(std::to_string(player.getGold()) + " G");
    hpText.setString(std::to_string(player.getCurrentHP()) + " / " + std::to_string(player.getMaxHP()) + "HP");

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
    window.draw(goldText);
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

bool Game::buyHP()
{ // magic number that needs to be revisited for balancing
    int cost = 10;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseHP(word_length);
        addMessage("Bought HP");
        return true;
    }
    return false;
}

bool Game::buyStrength()
{ // magic number that needs to be revisited for balancing
    int cost = 10;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseStrength(word_length);
        addMessage("Bought Strength");
        return true;
    }
    return false;
}

bool Game::buyArmor()
{ // magic number that needs to be revisited for balancing
    int cost = 10;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseArmor(1);
        addMessage("Bought Armor");
        return true;
    }
    return false;
}

bool Game::buyAgility()
{ // magic number that needs to be revisited for balancing
    int cost = 10;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseAgility(1);
        addMessage("Bought Agility");
        return true;
    }
    return false;
}

void Game::enterDungeon(int size, int difficulty)
{
    dungeon = Dungeon(size, difficulty);
    healMaxPlayer();
    changeMode(GameMode::DUNGEON);
}

void Game::leaveDungeon()
{
    changeMode(GameMode::MENU);
    healMaxPlayer();
    addMessage("You escaped the dungeon\n");
}

void Game::checkRoomHazards()
{
    Room *current = dungeon.returnCurrentRoom();
    std::optional<Trap *> trap = current->getTrap();
    if (trap)
    {
        int damage = trap.value()->getDamage();
        damage = player.receiveDamage(damage);
        std::string trap_name = trap.value()->to_string();
        addMessage("You triggered a " + trap_name + " trap! You take " + std::to_string(damage) + " damage.\n");
        current->removeTrap();
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
    battle_mode = true;
    changeMode(GameMode::BATTLE);
    // change current_word
    word_length = 3; // this,
    attacks = 8;     // this and
    parrys = 6;      // this should probably depend on monster/difficulty
    correct_attacks = 0;
    correct_parrys = 0;
    changeCurrentWord(word_length);
}

void Game::handleBattleInput(const std::string &input)
{
    if (battle_mode) // for attack
    {
        if (input == current_word)
        {
            correct_attacks++;
            changeCurrentWord(word_length);
        }
    }
    else // for parry
    {
        std::string reverse_word(current_word.rbegin(), current_word.rend());
        if (input == reverse_word)
        {
            correct_parrys++;
            changeCurrentWord(word_length);
        }
    }
}

void Game::changeCurrentWord(int length)
{

    const std::string characters = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::uniform_int_distribution<> dist(0, characters.length() - 1);

    current_word.clear();
    current_word.reserve(length);

    for (int i = 0; i < length; ++i)
    {
        current_word += characters[dist(rng)];
    }
    std::cout << "Current_word == " << current_word << "\n";
    // for now just make it into "test"
}

void Game::switchToDefense()
{
    battle_mode = false;
    changeCurrentWord(word_length);
    battleClock.restart();
}

void Game::damageMonster(int damage)
{ // should maybe make the damage done random later
    dungeon.getCurrentRoom().getMonster().value()->receiveDamage(damage);
}

void Game::endRound()
{
    std::cout << "EndRound() \n";
    int player_damage = player.getStrength();
    int total_player_damage = 0;
    for (int i = 0; i < correct_attacks; i++)
    {
        // deal damage to monster
        total_player_damage += player_damage;
        damageMonster(player_damage);
    }
    addMessage("You did " + std::to_string(correct_attacks) + " attacks for " + std::to_string(total_player_damage) + " damage! \n");

    int missed_parry = parrys - correct_parrys; // need to think if i want 2 for loops, one for parry and one for succesful monster attack?
    int monster_damage = dungeon.getCurrentRoom().getMonster().value()->getStrength();
    int total_monster_damage = 0;
    for (int i = 0; i < parrys; i++)
    {
        if (i < correct_parrys)
        {
            total_monster_damage += monster_damage;
            player.receiveDamageParry(monster_damage); // again should make it random eventually
        }
        else
        {
            total_monster_damage += monster_damage;
            player.receiveDamage(monster_damage); // again should make it random eventually
        }

        // deal damage to player
    }
    addMessage("You parried " + std::to_string(correct_parrys) + "/" + std::to_string(parrys) + "attacks, took " + std::to_string(total_monster_damage) + " damage! \n");

    // if player is dead (do something)
    if (!player.isAlive())
    {
        addMessage("You died, lost gold and XP \n");
        player.dead();
        changeMode(GameMode::MENU);
    }

    // if monster hp ==0: endBattle()
    if (dungeon.getCurrentRoom().getMonster().value()->getHP() <= 0)
    {
        endBattle();
        return;
    }

    battleClock.restart();
    battle_mode = true;
    changeCurrentWord(word_length);
    correct_attacks = 0;
    correct_parrys = 0;
}

void Game::endBattle()
{
    std::cout << "EndBattle() correct_attack:" << correct_attacks << "\n";
    std::cout << "EndBattle() correct_parry:" << correct_parrys << "\n";
    changeMode(GameMode::DUNGEON);
    int xp_reward = dungeon.getCurrentRoom().killMonster();
    player.receiveXP(xp_reward);
    roomText.setFillColor(sf::Color::White);
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

void Game::healMaxPlayer()
{
    player.healToMax();
}

GameMode Game::getMode() const
{
    return mode;
}

const Player &Game::getPlayer() const
{
    return player;
}

Player &Game::getPlayer()
{
    return player;
}

Dungeon &Game::getDungeon()
{
    return dungeon;
}