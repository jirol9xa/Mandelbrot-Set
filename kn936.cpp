#include <SFML/Graphics.hpp>

int main()
{
    sf::Clock clock;
    float second = clock.getElapsedTime().asSeconds();

    for (;;)
    {
        system("mplayer kn936.MP4");

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))    break;
    }


    return 0;
}