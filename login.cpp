#include "login.h"
#include <iostream>

Login::Login(sf::RenderWindow &win) : window(win)
{
    font.loadFromFile("new_athena.ttf");
    titleText.setFont(font);
    titleText.setString("Welcome to Ariadne!");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(115, 60);

    buttonBox.setSize(sf::Vector2f(180, 60));
    buttonBox.setPosition(310, 340);
    buttonBox.setFillColor(sf::Color(100, 100, 100, 200));

    loginButton.setFont(font);
    loginButton.setString("Login");
    loginButton.setCharacterSize(42);
    loginButton.setFillColor(sf::Color::White);
    loginButton.setPosition(345, 340);

    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(200, 360);
}

void Login::draw()
{
    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(buttonBox);
    window.draw(loginButton);

    if (!errorText.getString().isEmpty())
        window.draw(errorText);

    window.display();
}

void Login::handleEvent(const sf::Event &event, bool &tryLogin)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        auto mouse = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (buttonBox.getGlobalBounds().contains(mouse))
            tryLogin = true;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Return)
        {
            tryLogin = true;
        }
    }
}

bool Login::dummyApiCall(int &user_id)
{
    user_id = 7;
    return true;
}

bool Login::run(int &user_id)
{
    bool loggedIn = false;
    errorText.setString("");
    while (window.isOpen() && !loggedIn)
    {
        sf::Event event;
        bool tryLogin = false;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }
            handleEvent(event, tryLogin);
        }
        if (tryLogin)
        {
            if (dummyApiCall(user_id))
            {
                loggedIn = true;
            }
            else
            {
                errorText.setString("Login failed!");
            }
        }
        draw();
    }
    return loggedIn;
}