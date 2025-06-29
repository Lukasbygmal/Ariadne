#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "login.h"

int main()
{
    const unsigned int window_width = 800;
    const unsigned int window_height = 660;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Ariadne");
    int user_id = -1;

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
        std::cerr << "Error: " << e.what() << std::endl;
        // Exit code 2: Exception occurred
        return 2;
    }

    // Exit code 0: Normal exit
    return 0;
}