#include "game.h"
#include "handle_action.h"
#include <iostream>
#include <optional>

Game::Game()
    : player("legend27"), dungeon(6, 2), mode(GameMode::MENU), window(sf::VideoMode(1000, 800), "Ariadne") // Initialize dungeon with default values
{
    ui.initialize();
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        ui.handleInput(event, *this);
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
    // not sure what to do here
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
        int damage = *trap; //This should be changed once trap class is implemented
        player.receiveDamage(damage);
        addMessage("You triggered a trap! You take " + std::to_string(damage) + " damage (before armor).\n");
    }

    std::optional<int> monster = current->getMonster();
    if (monster)
    {
        addMessage("A monster appears before you! Prepare for battle!\n");
        changeMode(GameMode::BATTLE);
    }
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        ui.render(window, *this);
    }
}
