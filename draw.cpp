#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>


int  drawAss      (uint32_t *Pixels)
{
    PIXELS_CHECK(Pixels);

    sf::RenderWindow window(sf::VideoMode(WindowSettings::width, WindowSettings::heigth), "BlackAss");
    
    //sf::Texture texture;
    //texture.create(WindowSettings::width, WindowSettings::heigth);          // creating texture
    //
    //sf::Sprite  sprite(texture);                                            // creating sprite that will display texture

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)    window.close();

        }

        sf::Image image;
        image.create(WindowSettings::width, WindowSettings::heigth, (const uint8_t*) Pixels);    // creating image before drawing in window 
        image.saveToFile("Van-Cock.png");
        
        sf::Texture texture;
        texture.loadFromFile("Van-Cock.png");

        sf::Sprite sprite;
        sprite.setTexture(texture);

        //texture.update(Pixels);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
