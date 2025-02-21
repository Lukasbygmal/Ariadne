#include "game.h"
#include "handle_action.h"
#include <iostream>

Game::Game()
    : player("legend27"), dungeon(6, 2), mode(GameMode::DUNGEON), window(sf::VideoMode(1000, 800), "Ariadne")
{
    terminalMessages.push_back("Welcome to the dungeon!\n");
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
        handle_action(result, dungeon);
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
    //not sure what to do here
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