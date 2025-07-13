#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "login.hpp"

int main()
{
    const unsigned int window_width = 800;
    const unsigned int window_height = 660;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Ariadne", sf::Style::Close);

    window.setFramerateLimit(60);
    
    int user_id = -1;

    sf::Image icon;
    if (icon.loadFromFile("assets/WoA.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    Login login(window);
    if (!login.run(user_id))
    {
        // Exit code 1: Window closed or login failed
        return 1;
    }

    try
    {
        Game game(user_id, window);
        game.run();
    }
    catch (const std::exception &e)
    {
        // Exit code 2: Exception occurred
        return 2;
    }

    // Exit code 0: Normal exit
    return 0;
}