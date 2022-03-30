#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>


int  drawAss      (Mandelbrot *mbrot)
{
    PIXELS_CHECK(mbrot);

    sf::RenderWindow window(sf::VideoMode(mbrot->width, mbrot->heigth), "BlackAss");
    
    sf::Texture texture;
    texture.create(mbrot->width, mbrot->heigth);    // creating texture
    
    sf::Sprite  sprite(texture);                                                    // creating sprite that will display texture

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)    window.close();

        }

        if (fillImage(mbrot))   window.close();

        texture.update((const uint8_t *) (mbrot->Pix_begin));

        window.clear();
        window.draw(sprite);
        window.display();

        mbrot->Pixels = mbrot->Pix_begin;

    }

    return 0;
}
