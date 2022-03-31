#include <SFML/Graphics.hpp>

int main()
{
    sf::Image image;

    uint8_t *Pixels = (uint8_t *) calloc(1200*800, 4);
    
    image.create(1200, 800, Pixels);
    image.saveToFile("font.png");
    
    free(Pixels);
    return 0;
}