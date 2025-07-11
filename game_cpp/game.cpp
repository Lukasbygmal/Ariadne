#include "game.hpp"
#include "handle_action.hpp"
#include <string>
#include <cctype>
#include <chrono>
#include "parse_input.hpp"
#include "Colors.hpp"

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
        showErrorAndExit("Failed to load player data!\nPlease check your connection and try again.");
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

    lastHighscoreUpdate = std::chrono::steady_clock::time_point{};

    roomBackground.setSize(sf::Vector2f(460.f, 400.f));
    roomBackground.setFillColor(Colors::PrimaryBackground);
    roomBackground.setPosition(10.f, 30.f);

    statsBackground.setSize(sf::Vector2f(310.f, 210.f));
    statsBackground.setFillColor(Colors::PrimaryBackground);
    statsBackground.setPosition(480.f, 440.f);

    helpBackground.setSize(sf::Vector2f(150.f, 80.f));
    helpBackground.setFillColor(Colors::TerminalBackground);
    helpBackground.setPosition(640.f, 350.f);

    leaderboardBackground.setSize(sf::Vector2f(150.f, 80.f));
    leaderboardBackground.setFillColor(Colors::Treasure);
    leaderboardBackground.setPosition(480.f, 350.f);

    terminalBackground.setSize(sf::Vector2f(460.f, 160.f));
    terminalBackground.setFillColor(Colors::TerminalBackground);
    terminalBackground.setPosition(10.f, 440.f);

    inputBox.setSize(sf::Vector2f(460.f, 40.f));
    inputBox.setFillColor(Colors::Text);
    inputBox.setPosition(10.f, 610.f);

    initializeText();
    initializeMap();
}

void Game::initializeText()
{
    titleText.setFont(titleFont);
    titleText.setCharacterSize(28);
    titleText.setFillColor(Colors::Text);
    titleText.setPosition(20.f, 40.f);

    subTitleText.setFont(textFont);
    subTitleText.setCharacterSize(18);
    subTitleText.setFillColor(Colors::Text);
    subTitleText.setPosition(24.f, 95.f);

    battleText.setFont(textFont);
    battleText.setCharacterSize(28);
    battleText.setFillColor(Colors::Text);
    battleText.setPosition(24.f, 135.f);

    levelText.setFont(textFont);
    levelText.setCharacterSize(24);
    levelText.setFillColor(Colors::Text);
    levelText.setPosition(610.f, 460.f);

    goldText.setFont(textFont);
    goldText.setCharacterSize(24);
    goldText.setFillColor(Colors::Treasure);
    goldText.setPosition(610.f, 495.f);

    hpText.setFont(textFont);
    hpText.setCharacterSize(20);
    hpText.setFillColor(Colors::Danger);
    hpText.setPosition(590.f, 600.f);

    strengthText.setFont(textFont);
    strengthText.setCharacterSize(20);
    strengthText.setFillColor(Colors::Text);
    strengthText.setPosition(490.f, 540.f);

    agilityText.setFont(textFont);
    agilityText.setCharacterSize(20);
    agilityText.setFillColor(Colors::Text);
    agilityText.setPosition(610.f, 540.f);

    armorText.setFont(textFont);
    armorText.setCharacterSize(20);
    armorText.setFillColor(Colors::Text);
    armorText.setPosition(700.f, 540.f);

    listDungeonsText.setFont(textFont);
    listDungeonsText.setCharacterSize(20);
    listDungeonsText.setFillColor(Colors::Text);
    listDungeonsText.setPosition(490.f, 30.f);

    helpText.setFont(textFont);
    helpText.setCharacterSize(20);
    helpText.setFillColor(Colors::Text);
    helpText.setPosition(650.f, 360.f);

    leaderboardText.setFont(textFont);
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(Colors::Text);
    leaderboardText.setPosition(490.f, 360.f);

    terminalText.setFont(textFont);
    terminalText.setCharacterSize(18);
    terminalText.setFillColor(Colors::Text);
    terminalText.setPosition(20.f, 440.f);

    inputText.setFont(textFont);
    inputText.setCharacterSize(18);
    inputText.setFillColor(Colors::Background);
    inputText.setPosition(20.f, 620.f);
}

void Game::initializeMap()
{
    sf::Vector2f mapPosition = {480.0f, 30.0f};
    mapBackground.setSize(sf::Vector2f(MAP_DISPLAY_SIZE, MAP_DISPLAY_SIZE));
    mapBackground.setFillColor(Colors::MapBackground);
    mapBackground.setPosition(mapPosition);

    auto dungeonMapData = dungeon.dungeonMap();
    int mapSize = dungeonMapData.size();

    float tileSize = MAP_DISPLAY_SIZE / mapSize;

    float tilePadding = tileSize * 0.4;
    float roomSize = tileSize - tilePadding;

    mapTiles.resize(mapSize);
    for (int i = 0; i < mapSize; ++i)
    {
        mapTiles[i].resize(mapSize);
    }

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            mapTiles[i][j].setSize(sf::Vector2f(roomSize, roomSize));
            mapTiles[i][j].setPosition(
                mapPosition.x + j * tileSize + tilePadding / 2.0f,
                mapPosition.y + i * tileSize + tilePadding / 2.0f);
        }
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            if (!apiClient.savePlayer(player, user_id))
            {
                showErrorAndExit("Failed to save player data!\n Unlucky!");
            }
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
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left && mode == GameMode::MENU)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Check if help button was clicked
            if (helpBackground.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                showingHelp = true;
            }
            // Check if leaderboard button was clicked
            else if (leaderboardBackground.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                showingHelp = false;
                updateHighscoreDisplay();
            }
        }
    }
}

void Game::handleCommand(const std::string &command)
{
    Action result("null");
    if (parse_input(command, result, mode))
    {
        if (!handle_action(result, *this))
        {
            addMessage("Can't " + command + " now");
        }
    }
    else
    {
        addMessage("Invalid: " + command);
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
        listDungeonsText.setString("");
        titleText.setString(dungeon.to_string() + dungeon.getCurrentRoom().to_string());
    }
    else if (mode == GameMode::MENU)
    {
        battleText.setString("");
        titleText.setString("The Ink & Anvil Tavern \nBuy stats or enter a dungeon! \n");
        // Kinda ugly way to display dungeons, but it works for now
        std::string list_dungeons = "Dungeons:\nThal            Vorn\nEzra            Kurn \nZamo            Druv \nMalq            Xelv \nOrmh            Griv \nFend            Quar \nBlen            Xoth \nMerk            Zenk";
        listDungeonsText.setString(list_dungeons);

        if (showingHelp)
        {
            subTitleText.setString("\nHow to play:\nIn tavern:\nbuy [amount] [hp/strength/agility/armor]\nenter [dungeon name] [easy/medium/hard]\nIn dungeon:\ngo [west/north/south/east]\nopen chest\nexit\nIn battle:\nType the green text to attack\nType the red text backwards to parry\n");
        }

        else
        {
            std::string highscoreDisplay = "\nLeaderboard:\n";
            for (size_t i = 0; i < cachedHighscores.size() && i < 10; ++i) // Show top 10
            {
                try
                {
                    std::string name = cachedHighscores[i]["name"].get<std::string>();
                    int lvl = cachedHighscores[i]["lvl"].get<int>();
                    int xp = cachedHighscores[i]["xp"].get<int>();
                    highscoreDisplay += std::to_string(i + 1) + ". " + name + " - lvl: " + std::to_string(lvl) + " - XP: " + std::to_string(xp) + "\n";
                }
                catch (const std::exception &e)
                {
                    highscoreDisplay += std::to_string(i + 1) + ". Error\n";
                }
            }
            if (cachedHighscores.empty())
            {
                highscoreDisplay += "Leaderboard not available\n";
            }
            subTitleText.setString(highscoreDisplay);
        }
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
            battleText.setFillColor(Colors::Player);
        }

        if (!battle_mode)
        {
            battleText.setFillColor(Colors::Danger);
        }
    }

    leaderboardText.setString("Leaderboard");
    helpText.setString("Help");

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
        segment.setFillColor(i < filledSegments ? Colors::Player : Colors::PrimaryBackground);
        window.draw(segment);
    }
}

void Game::updateHighscoreDisplay()
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(now - lastHighscoreUpdate);

    // Only fetch if more than 30 seconds have passed or if we have no cached data
    if (timeSinceLastUpdate.count() >= 30 || cachedHighscores.empty())
    {
        cachedHighscores = apiClient.getHighscore();
        lastHighscoreUpdate = now;
    }
}

void Game::renderMap()
{
    window.draw(mapBackground);

    // Only draw tiles when in dungeon mode
    if (mode == GameMode::DUNGEON)
    {
        auto dungeonMapData = dungeon.dungeonMap();
        int mapSize = dungeonMapData.size();

        for (int i = 0; i < mapSize; ++i)
        {
            for (int j = 0; j < mapSize; ++j)
            {
                sf::Color tileColor;
                switch (dungeonMapData[i][j])
                {
                case 0:
                    tileColor = Colors::PrimaryBackground;
                    break; // NORMAL room
                case 1:
                    tileColor = Colors::Danger;
                    break; // BOSS room
                case 2:
                    tileColor = Colors::Treasure;
                    break; // TREASURE room
                case 3:
                    tileColor = Colors::Background;
                    break; // BLOCKED room
                case 4:
                    tileColor = Colors::Player;
                    break; // Player position
                default:
                    tileColor = sf::Color::Magenta;
                    break; // Debug
                }
                mapTiles[i][j].setFillColor(tileColor);
                window.draw(mapTiles[i][j]);
            }
        }
    }
}
void Game::render()
{
    updateUI();

    window.clear();

    renderXPBar();
    renderMap();

    window.draw(roomBackground);
    window.draw(titleText);
    window.draw(subTitleText);
    window.draw(battleText);
    window.draw(statsBackground);
    window.draw(helpBackground);
    window.draw(leaderboardBackground);
    window.draw(levelText);
    window.draw(goldText);
    window.draw(hpText);
    window.draw(strengthText);
    window.draw(agilityText);
    window.draw(armorText);
    window.draw(helpText);
    window.draw(leaderboardText);
    window.draw(listDungeonsText);
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

bool Game::buyHP(int amount)
{ // magic number that needs to be revisited for balancing
    int cost = 20 * amount;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseHP(1 * amount);
        addMessage("Bought " + std::to_string(amount) + " HP");
        healMaxPlayer();
        return true;
    }
    return false;
}

bool Game::buyStrength(int amount)
{ // magic number that needs to be revisited for balancing
    int cost = 100 * amount;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseStrength(1 * amount);
        addMessage("Bought " + std::to_string(amount) + " Strength");
        return true;
    }
    return false;
}

bool Game::buyArmor(int amount)
{ // magic number that needs to be revisited for balancing
    int cost = 100 * amount;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseArmor(1 * amount);
        addMessage("Bought " + std::to_string(amount) + " Armor");
        return true;
    }
    return false;
}

bool Game::buyAgility(int amount)
{ // magic number that needs to be revisited for balancing
    int cost = 100 * amount;
    if (player.getGold() >= cost)
    {
        player.decreaseGold(cost);
        player.increaseAgility(1 * amount);
        addMessage("Bought " + std::to_string(amount) + " Agility");
        return true;
    }
    return false;
}

void Game::enterDungeon(std::string dungeon_name, std::string string_difficulty)
{
    dungeon = Dungeon(dungeon_name, string_difficulty);
    initializeMap();
    healMaxPlayer();
    changeMode(GameMode::DUNGEON);
}

void Game::leaveDungeon()
{
    if (!apiClient.savePlayer(player, user_id))
    {
        showErrorAndExit("Failed to save player data!\n Sadly exiting");
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
    float round_time_scaling = (dungeon.getDifficulty() / 30) * 3;      // Scale: 0.1-0.3 seconds per difficulty point
    round_time = 3 - round_time_scaling + (player.getAgility() * 0.05); // Base 3s, reduced by difficulty, increased by agility
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
        if (i < correct_parrys)
        {
            actual_damage = player.receiveDamageParry(hit);
            total_monster_damage += actual_damage;
        }
        else
        {
            actual_damage = player.receiveDamage(hit);
            total_monster_damage += actual_damage;
        }
    }
    addMessage("You parried " + std::to_string(correct_parrys) + "/" + std::to_string(parrys) + "attacks, took " + std::to_string(total_monster_damage) + " damage!");

    if (!player.isAlive())
    {
        addMessage("You died, lost gold and XP!");
        player.dead();
        titleText.setFillColor(Colors::Text); // check if needed after change todo
        changeMode(GameMode::MENU);
        healMaxPlayer();
    }

    // this long if statement is if monster is dead
    if (monster)
    {
        if (monster->getHP() <= 0)
        {
            addMessage(monster->to_string() + " slayed!");
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
    changeMode(GameMode::DUNGEON);
    int xp_reward = dungeon.getCurrentRoom().killMonster();
    bool did_level = player.receiveXP(xp_reward);
    if (did_level)
    {
        addMessage("You leveled up and gained some stats!");
    }
    titleText.setFillColor(Colors::Text); // check if needed after change todo
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

void Game::showErrorAndExit(const std::string &message)
{
    window.clear(Colors::Background);

    // Create error text
    sf::Text errorText;
    errorText.setFont(textFont); // Use default font if loading fails
    errorText.setString(message);
    errorText.setCharacterSize(24);
    errorText.setFillColor(Colors::Danger);
    errorText.setPosition(50, 200);

    // Show error for 5 seconds
    sf::Clock timer;
    while (timer.getElapsedTime().asSeconds() < 5.0f)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                exit(1);
            }
        }

        window.clear(Colors::Background);
        window.draw(errorText);
        window.display();
    }

    exit(1);
}