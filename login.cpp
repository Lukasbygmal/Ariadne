#include "login.h"
#include <iostream>
#include "auth.h"
#include <SFML/Window/Clipboard.hpp>

static const size_t MAX_INPUT_LENGTH = 50;

Login::Login(sf::RenderWindow &win) : window(win), waitingForCode(false), inputActive(false)
{
    if (!textFont.loadFromFile("Fredoka-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load textFont");
    }
    if (!titleFont.loadFromFile("Fredoka-Medium.ttf"))
    {
        throw std::runtime_error("Failed to load titleFont");
    }

    titleText.setFont(titleFont);
    titleText.setString("Welcome to Ariadne!");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(60, 60);

    buttonBox.setSize(sf::Vector2f(320, 60));
    buttonBox.setPosition(245, 340);
    buttonBox.setFillColor(sf::Color(100, 100, 100, 200));

    loginButton.setFont(titleFont);
    loginButton.setString("GitHub Login");
    loginButton.setCharacterSize(42);
    loginButton.setFillColor(sf::Color::White);
    loginButton.setPosition(285, 343);

    inputBox.setSize(sf::Vector2f(400, 40));
    inputBox.setPosition(180, 430);
    inputBox.setFillColor(sf::Color(50, 50, 50, 200));
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color(100, 100, 100));

    inputText.setFont(textFont);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(190, 440);

    promptText.setFont(textFont);
    promptText.setCharacterSize(20);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(180, 400);

    submitButton.setSize(sf::Vector2f(40, 40));
    submitButton.setPosition(590, 430);
    submitButton.setFillColor(sf::Color(0, 150, 0, 200));

    submitButtonText.setFont(titleFont);
    submitButtonText.setString(">");
    submitButtonText.setCharacterSize(20);
    submitButtonText.setFillColor(sf::Color::White);
    submitButtonText.setPosition(600, 435);

    errorText.setFont(textFont);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(200, 500);

    inputString = "";
}

void Login::draw()
{
    window.clear(sf::Color::Black);
    window.draw(titleText);

    if (!waitingForCode)
    {
        window.draw(buttonBox);
        window.draw(loginButton);
    }
    else
    {
        drawInputElements();
    }

    if (!errorText.getString().isEmpty())
        window.draw(errorText);

    window.display();
}

void Login::drawInputElements()
{
    window.draw(promptText);
    window.draw(inputBox);
    window.draw(inputText);
    window.draw(submitButton);
    window.draw(submitButtonText);
}

void Login::handleEvent(const sf::Event &event, bool &tryLogin, bool &submitCode)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        handleMouseClick(event.mouseButton, tryLogin, submitCode);
        break;

    case sf::Event::KeyPressed:
        handleKeyPress(event.key, tryLogin, submitCode);
        break;

    case sf::Event::TextEntered:
        handleTextInput(event.text);
        break;

    default:
        break;
    }
}

void Login::handleMouseClick(const sf::Event::MouseButtonEvent &mouse, bool &tryLogin, bool &submitCode)
{
    sf::Vector2f mousePos(mouse.x, mouse.y);

    if (!waitingForCode)
    {
        if (buttonBox.getGlobalBounds().contains(mousePos))
        {
            tryLogin = true;
        }
    }
    else
    {
        if (inputBox.getGlobalBounds().contains(mousePos))
        {
            setInputActive(true);
        }
        else if (submitButton.getGlobalBounds().contains(mousePos))
        {
            submitCode = true;
        }
        else
        {
            setInputActive(false);
        }
    }
}

void Login::handleKeyPress(const sf::Event::KeyEvent &key, bool &tryLogin, bool &submitCode)
{
    if (waitingForCode && inputActive)
    {
        if ((key.code == sf::Keyboard::V) && (key.control || key.system))
        {
            pasteFromClipboard();
        }
        else if (key.code == sf::Keyboard::Return)
        {
            submitCode = true;
        }
        else if (key.code == sf::Keyboard::Backspace)
        {
            handleBackspace();
        }
    }
    else if (!waitingForCode && key.code == sf::Keyboard::Return)
    {
        tryLogin = true;
    }
}

void Login::handleTextInput(const sf::Event::TextEvent &text)
{
    if (!waitingForCode || !inputActive)
        return;

    // Skip paste character (Ctrl+V generates unicode 22)
    if (text.unicode == 22)
        return;

    if (text.unicode >= 32 && text.unicode < 127) // Printable ASCII
    {
        if (inputString.length() < MAX_INPUT_LENGTH) // To make sure input is not too long
        {
            inputString += static_cast<char>(text.unicode);
            inputText.setString(inputString);
        }
    }
}

void Login::setInputActive(bool active)
{
    inputActive = active;
    inputBox.setOutlineColor(active ? sf::Color::White : sf::Color(100, 100, 100));
}

void Login::pasteFromClipboard()
{
    sf::String clipboard = sf::Clipboard::getString();
    std::string clipboardStr = clipboard.toAnsiString();

    size_t availableSpace = MAX_INPUT_LENGTH - inputString.length(); // To make sure input is not too long
    if (availableSpace > 0)
    {
        inputString += clipboardStr.substr(0, availableSpace);
        inputText.setString(inputString);
    }
}
void Login::handleBackspace()
{
    if (!inputString.empty())
    {
        inputString.pop_back();
        inputText.setString(inputString);
    }
}
bool Login::run(int &user_id)
{
    bool loggedIn = false;
    clearError();

    while (window.isOpen() && !loggedIn)
    {
        bool tryLogin = false;
        bool submitCode = false;

        handleEvents(tryLogin, submitCode);

        if (tryLogin)
        {
            initiateLogin();
        }

        if (submitCode && waitingForCode)
        {
            loggedIn = processCodeSubmission(user_id);
        }

        draw();
    }

    return loggedIn;
}

void Login::handleEvents(bool &tryLogin, bool &submitCode)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        handleEvent(event, tryLogin, submitCode);
    }
}

void Login::initiateLogin()
{
    auth::startGithubLogin();
    waitingForCode = true;
    setInputActive(true);
    clearInput();
    promptText.setString("Paste the code from the browser here:");
    clearError();
}

bool Login::processCodeSubmission(int &user_id)
{
    if (inputString.empty())
    {
        setError("Please enter a code first.");
        return false;
    }

    if (auth::exchangeCodeForUserId(inputString, user_id))
    {
        return true;
    }
    else
    {
        setError("Login failed! Invalid code/token.");
        clearInput();
        return false;
    }
}

void Login::clearInput()
{
    inputString = "";
    inputText.setString("");
}

void Login::clearError()
{
    errorText.setString("");
}

void Login::setError(const std::string &message)
{
    errorText.setString(message);
}