#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Ariadne");

    sf::Font font;
    if (!font.loadFromFile("new_athena.ttf"))
    {
        return -1;
    }

    sf::RectangleShape statsBackground(sf::Vector2f(200.f, 120.f));
    statsBackground.setFillColor(sf::Color(100, 100, 100, 200));
    statsBackground.setPosition(10.f, 10.f);

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(18);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(20.f, 20.f);
    statsText.setLineSpacing(1.5f);
    statsText.setString(
        "LVL: 5\n"
        "HP: 100\n"
        "STR: 15\n"
        "DEX: 12\n"
        "AGI: 14\n"
        "ARMOR: 8");

    std::string userInput;

    sf::RectangleShape terminalBackground(sf::Vector2f(600.f, 160.f));
    terminalBackground.setFillColor(sf::Color(50, 50, 50, 220));
    terminalBackground.setPosition(10.f, 580.f);

    sf::RectangleShape inputBox(sf::Vector2f(600.f, 40.f));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(10.f, 750.f);

    std::vector<std::string> terminalMessages = {
        "Welcome to the dungeon!\n"
    };

    sf::Text terminalText;
    terminalText.setFont(font);
    terminalText.setCharacterSize(18);
    terminalText.setFillColor(sf::Color::White);
    terminalText.setPosition(20.f, 590.f);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20.f, 760.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !userInput.empty())
                {
                    userInput.pop_back();
                }
                else if (event.text.unicode == '\r')
                {
                    terminalMessages.push_back("> " + userInput + "\n");
                    userInput.clear();

                    if (terminalMessages.size() > 7)
                        terminalMessages.erase(terminalMessages.begin());
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 127)
                {
                    userInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        std::string terminalDisplay;
        for (const auto& msg : terminalMessages)
        {
            terminalDisplay += msg + "\n";
        }

        terminalText.setString(terminalDisplay);
        inputText.setString("> " + userInput);

        window.clear();
        window.draw(statsBackground);
        window.draw(statsText);
        window.draw(terminalBackground);
        window.draw(terminalText);
        window.draw(inputBox);
        window.draw(inputText);
        window.display();
    }

    return 0;
}
