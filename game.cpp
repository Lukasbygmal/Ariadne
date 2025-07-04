#include "game.h"
#include "handle_action.h"
#include <iostream>
#include <string>
#include <cctype>
#include "parse_input.h"

std::random_device Game::rd;
std::mt19937 Game::rng(Game::rd());

Game::Game(int user_id_, sf::RenderWindow &win)
    : apiClient("http://localhost:5000", "local-test-key-123"),
      player("", 1, 1, 1, 1, 1, 1, 1),
      user_id(user_id_),
      dungeon("thal", "easy"),
      mode(GameMode::MENU),
      window(win)
{
    if (!apiClient.loadPlayer(player, user_id))
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
    if (!textFont.loadFromFile("Fredoka-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load textFont");
    }
    if (!titleFont.loadFromFile("Fredoka-Medium.ttf"))
    {
        throw std::runtime_error("Failed to load titleFont");
    }

    roomBackground.setSize(sf::Vector2f(460.f, 400.f));
    roomBackground.setFillColor(sf::Color(100, 100, 100, 200));
    roomBackground.setPosition(10.f, 30.f);

    statsBackground.setSize(sf::Vector2f(310.f, 210.f));
    statsBackground.setFillColor(sf::Color(100, 100, 100, 200));
    statsBackground.setPosition(480.f, 440.f);

    perksBackground.setSize(sf::Vector2f(150.f, 80.f));
    perksBackground.setFillColor(sf::Color(114, 188, 212, 200));
    perksBackground.setPosition(640.f, 350.f);

    yetToDecideBackground.setSize(sf::Vector2f(150.f, 80.f));
    yetToDecideBackground.setFillColor(sf::Color(120, 150, 100, 200));
    yetToDecideBackground.setPosition(480.f, 350.f);

    terminalBackground.setSize(sf::Vector2f(460.f, 160.f));
    terminalBackground.setFillColor(sf::Color(50, 50, 50, 220));
    terminalBackground.setPosition(10.f, 440.f);

    inputBox.setSize(sf::Vector2f(460.f, 40.f));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(10.f, 610.f);

    initializeText();
}

void Game::initializeText()
{
    titleText.setFont(titleFont);
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(20.f, 40.f);

    subTitleText.setFont(textFont);
    subTitleText.setCharacterSize(18);
    subTitleText.setFillColor(sf::Color::White);
    subTitleText.setPosition(24.f, 95.f);

    battleText.setFont(textFont);
    battleText.setCharacterSize(28);
    battleText.setFillColor(sf::Color::White);
    battleText.setPosition(24.f, 135.f);

    levelText.setFont(textFont);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(610.f, 460.f);

    goldText.setFont(textFont);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(610.f, 495.f);

    hpText.setFont(textFont);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(590.f, 600.f);

    strengthText.setFont(textFont);
    strengthText.setCharacterSize(20);
    strengthText.setFillColor(sf::Color::White);
    strengthText.setPosition(490.f, 540.f);

    agilityText.setFont(textFont);
    agilityText.setCharacterSize(20);
    agilityText.setFillColor(sf::Color::White);
    agilityText.setPosition(610.f, 540.f);

    armorText.setFont(textFont);
    armorText.setCharacterSize(20);
    armorText.setFillColor(sf::Color::White);
    armorText.setPosition(700.f, 540.f);

    terminalText.setFont(textFont);
    terminalText.setCharacterSize(18);
    terminalText.setFillColor(sf::Color::White);
    terminalText.setPosition(20.f, 440.f);

    inputText.setFont(textFont);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20.f, 620.f);
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            apiClient.savePlayer(player, user_id);
            window.close();
        }

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
            addMessage("Can't " + command + " now");
        }
    }
    else
    {
        addMessage("Invalid: " + command);
        std::cout << "Invalid command. Try again.\n";
    }
}

void Game::addMessage(const std::string &message)
{
    const size_t maxMessageLength = 55;
    std::string truncatedMessage = message;
    if (truncatedMessage.length() > maxMessageLength)
    {
        truncatedMessage = truncatedMessage.substr(0, maxMessageLength) + "...";
    }
    terminalMessages.push_back(truncatedMessage);
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
        sf::Time end_time = sf::seconds(round_time);
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
        battleText.setString("");
        subTitleText.setString("");
        titleText.setString(dungeon.to_string() + dungeon.getCurrentRoom().to_string());
    }
    else if (mode == GameMode::MENU)
    {
        battleText.setString("");
        titleText.setString("The Ink & Anvil Tavern \nBuy stats or enter a dungeon! \n");
        std::string list_dungeons = "\nDungeons easy/medium/hard:\nThal   Vorn \nEzra   Kurn \nZamo   Druv \nMalq   Xelv \nOrmh   Griv \nFend   Quar \nBlen   Xoth \nMerk   Zenk";
        subTitleText.setString(list_dungeons);
    }
    else if (mode == GameMode::BATTLE)
    {
        Monster *monster = dungeon.getCurrentRoom().getMonster();
        if (monster)
        {
            titleText.setString("Battle with " + monster->to_string() + "\n");
            subTitleText.setString("Strength: " + std::to_string(monster->getStrength()) + " HP: " + std::to_string(monster->getHP()));
        }
        std::string display_words;
        for (const std::string &word : word_queue)
        {
            display_words += word + "\n";
        }

        battleText.setString(display_words);
        if (battle_mode)
        {
            battleText.setFillColor(sf::Color::Green);
        }

        if (!battle_mode)
        {
            battleText.setFillColor(sf::Color::Red);
        }
    }

    levelText.setString("Lvl " + std::to_string(player.getLevel()));
    goldText.setString(std::to_string(player.getGold()) + " G");
    hpText.setString(std::to_string(player.getCurrentHP()) + " / " + std::to_string(player.getMaxHP()) + " HP");

    strengthText.setString("Strength\n      " + std::to_string(player.getStrength()));
    agilityText.setString("Agility\n    " + std::to_string(player.getAgility()));
    armorText.setString("Armor\n    " + std::to_string(player.getArmor()));

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
    window.draw(titleText);
    window.draw(subTitleText);
    window.draw(battleText);
    window.draw(statsBackground);
    window.draw(perksBackground);
    window.draw(yetToDecideBackground);
    window.draw(levelText);
    window.draw(goldText);
    window.draw(hpText);
    window.draw(strengthText);
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
        player.increaseHP(5);
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
        player.increaseStrength(1);
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

void Game::enterDungeon(std::string dungeon_name, std::string string_difficulty)
{
    dungeon = Dungeon(dungeon_name, string_difficulty);
    healMaxPlayer();
    changeMode(GameMode::DUNGEON);
}

void Game::leaveDungeon()
{
    if (!apiClient.savePlayer(player, user_id))
    {
        std::cout << "Failed to save player data!" << std::endl;
    }
    else
    {
        std::cout << "Player data saved successfully!" << std::endl;
    }
    changeMode(GameMode::MENU);
    healMaxPlayer();
    addMessage("You escaped the dungeon");
}

void Game::checkRoomHazards()
{
    Room &current = dungeon.getCurrentRoom();
    Trap *trap = current.getTrap();
    if (trap)
    {
        int damage = trap->getDamage();
        damage = player.receiveDamage(damage);
        std::string trap_name = trap->to_string();
        addMessage("You triggered a " + trap_name + " trap! You take " + std::to_string(damage) + " damage.");
        current.removeTrap();
    }
    Monster *monster = dungeon.getCurrentRoom().getMonster();
    if (monster)
    {
        std::string monster_name = monster->to_string();
        addMessage("A " + monster_name + " appears before you! Prepare for battle!");
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
    word_length_attack = dungeon.getWordLength();
    word_length_parry = word_length_attack - 1;
    attacks = dungeon.getWords();
    parrys = attacks - 2;
    correct_attacks = 0;
    correct_parrys = 0;
    round_time = 5.0 + player.getAgility() * 0.05; // should depend on monster, and needs balancing
    resetWordQueue(attacks, word_length_attack);
}

void Game::handleBattleInput(const std::string &input)
{
    if (word_queue.empty())
        return;

    const std::string &current_word = word_queue.front();

    if (battle_mode) // for attack
    {
        if (input == current_word)
        {
            correct_attacks++;
            word_queue.pop_front();
        }
    }
    else // for parry
    {
        std::string reverse_word(current_word.rbegin(), current_word.rend());
        if (input == reverse_word)
        {
            correct_parrys++;
            word_queue.pop_front();
        }
    }
}

std::string Game::createWord(int length)
{
    std::string new_word;
    const std::string characters = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::uniform_int_distribution<> dist(0, characters.length() - 1);

    new_word.reserve(length);

    for (int i = 0; i < length; ++i)
    {
        new_word += characters[dist(rng)];
    }
    std::cout << "new_word == " << new_word << "\n";
    return new_word;
}

void Game::resetWordQueue(int words, int length)
{
    word_queue.clear();
    for (int i = 0; i < words; i++)
    {
        word_queue.push_back(createWord(length));
    }
}

void Game::switchToDefense()
{
    battle_mode = false;
    userInput.clear(); // clear terminal input
    resetWordQueue(parrys, word_length_parry);
    battleClock.restart();
}

void Game::damageMonster(int damage)
{
    Monster *monster = dungeon.getCurrentRoom().getMonster();
    if (monster)
    {
        monster->receiveDamage(damage);
    }
}

void Game::endRound()
{
    std::cout << "EndRound() \n";
    userInput.clear();                                      // clear terminal input
    std::uniform_real_distribution<float> dist(0.5f, 1.5f); // might want to make 1 for attack and one for def, if perks affect this
    float amplifier = 1;
    int hit;
    int player_damage = player.damage(); // should make it random eventually
    int total_player_damage = 0;
    for (int i = 0; i < correct_attacks; i++)
    {
        // deal damage to monster
        amplifier = dist(rng);
        hit = player_damage * amplifier;
        total_player_damage += hit;
        std::cout << "amplifier " << amplifier << "\n";
        std::cout << "hit(player) " << hit << "\n";

        damageMonster(hit);
    }
    addMessage("You did " + std::to_string(correct_attacks) + " attacks for " + std::to_string(total_player_damage) + " damage!");

    Monster *monster = dungeon.getCurrentRoom().getMonster();
    int monster_damage = 0;
    if (monster)
    {
        monster_damage = monster->getStrength();
    }
    int total_monster_damage = 0;
    int actual_damage;
    for (int i = 0; i < parrys; i++)
    {
        amplifier = dist(rng);
        hit = monster_damage * amplifier;
        std::cout << "amplifier " << amplifier << "\n";
        std::cout << "hit(monster) " << hit << "\n";
        if (i < correct_parrys)
        {
            actual_damage = player.receiveDamageParry(hit);
            std::cout << "Parried -" << actual_damage << "\n";
            total_monster_damage += actual_damage;
        }
        else
        {
            actual_damage = player.receiveDamage(hit);
            std::cout << "Not parried -" << actual_damage << "\n";
            total_monster_damage += actual_damage;
        }
    }
    addMessage("You parried " + std::to_string(correct_parrys) + "/" + std::to_string(parrys) + "attacks, took " + std::to_string(total_monster_damage) + " damage!");

    if (!player.isAlive())
    {
        addMessage("You died, lost gold and XP!");
        player.dead();
        titleText.setFillColor(sf::Color::White); // check if needed after change
        changeMode(GameMode::MENU);
    }

    // this long if statement is if monster is dead
    if (monster)
    {
        if (monster->getHP() <= 0)
        {
            endBattle();
            return;
        }
    }

    battleClock.restart();
    battle_mode = true;
    resetWordQueue(attacks, word_length_attack);
    correct_attacks = 0;
    correct_parrys = 0;
}

void Game::endBattle()
{
    std::cout << "EndBattle() correct_attack:" << correct_attacks << "\n";
    std::cout << "EndBattle() correct_parry:" << correct_parrys << "\n";
    changeMode(GameMode::DUNGEON);
    int xp_reward = dungeon.getCurrentRoom().killMonster();
    bool did_level = player.receiveXP(xp_reward);
    if (did_level)
    {
        addMessage("You leveled up and gained some stats!");
    }
    titleText.setFillColor(sf::Color::White); // check if needed after change
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