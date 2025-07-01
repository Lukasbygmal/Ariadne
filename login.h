#ifndef LOGIN_H
#define LOGIN_H

#include <SFML/Graphics.hpp>
#include <string>

class Login
{
public:
    Login(sf::RenderWindow &win);
    bool run(int &user_id);
    void draw();
    void handleEvent(const sf::Event &event, bool &tryLogin, bool &submitCode);

private:
    sf::RenderWindow &window;
    sf::Font font;
    
    sf::Text titleText;
    sf::RectangleShape buttonBox;
    sf::Text loginButton;
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Text promptText;
    sf::RectangleShape submitButton;
    sf::Text submitButtonText;
    sf::Text errorText;
    
    std::string inputString;
    bool waitingForCode;
    bool inputActive;

    void drawInputElements();

    void handleEvents(bool &tryLogin, bool &submitCode);
    void handleMouseClick(const sf::Event::MouseButtonEvent &mouse, bool &tryLogin, bool &submitCode);
    void handleKeyPress(const sf::Event::KeyEvent &key, bool &tryLogin, bool &submitCode);
    void handleTextInput(const sf::Event::TextEvent &text);

    void setInputActive(bool active);
    void pasteFromClipboard();
    void handleBackspace();

    void initiateLogin();
    bool processCodeSubmission(int &user_id);

    void clearInput();
    void clearError();
    void setError(const std::string &message);
};

#endif