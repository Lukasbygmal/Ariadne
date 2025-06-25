#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Login
{
public:
    Login(sf::RenderWindow &window);
    // Returns true if login was successful, sets user_id
    bool run(int &user_id);

private:
    sf::RenderWindow &window;
    sf::Font font;
    sf::Text titleText, loginButton, errorText;
    sf::RectangleShape buttonBox;
    void draw();
    void handleEvent(const sf::Event &event, bool &tryLogin);
    bool dummyApiCall(int &user_id);
};