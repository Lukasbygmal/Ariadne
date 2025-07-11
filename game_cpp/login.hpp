/**
 * @file login.h
 * @brief User authentication interface for GitHub OAuth login
 * @author [Your Name]
 * @date [Current Date]
 * @version 1.0
 *
 * This file contains the Login class declaration which provides a graphical
 * user interface for authenticating users via GitHub OAuth. The class handles
 * the complete login flow including initiating OAuth, collecting authorization
 * codes, and processing authentication tokens.
 *
 * @note Requires SFML graphics library and a valid GitHub OAuth application
 * @depends SFML/Graphics.hpp, auth.h, Colors.hpp
 */
#ifndef LOGIN_H
#define LOGIN_H

#include <SFML/Graphics.hpp>
#include <string>
/**
 * @class Login
 * @brief Graphical user interface for GitHub OAuth authentication
 *
 * The Login class provides a complete user authentication interface using GitHub OAuth.
 * It manages the two-phase login process: initiating the OAuth flow and collecting
 * the authorization code from the user. The class renders all UI elements using SFML
 * and handles user input including keyboard and mouse events.
 *
 * The authentication flow consists of:
 * 1. User clicks "GitHub Login" button
 * 2. OAuth flow is initiated (opens browser)
 * 3. User pastes authorization code from browser
 * 4. Code is exchanged for user ID via auth module
 *
 */
class Login
{
public:
    Login(sf::RenderWindow &win);
    /**
     * @brief Runs the complete login process with event handling
     *
     * Enters the main login loop, handling user events and managing the authentication
     * flow. This method blocks until the user either successfully logs in or closes
     * the window. It manages both phases of the login process: OAuth initiation and
     * code submission.
     *
     * @param[out] user_id Reference to store the authenticated user's ID upon success
     * @return true if login was successful, false if window was closed or login failed
     * @note This method blocks until login completion or window closure
     */
    bool run(int &user_id);
    void draw();
    void handleEvent(const sf::Event &event, bool &tryLogin, bool &submitCode);

private:
    sf::RenderWindow &window;
    sf::Font titleFont;
    sf::Font textFont;

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