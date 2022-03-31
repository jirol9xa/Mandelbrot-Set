#include <SFML/Graphics.hpp>
#include <Mandelbrot_noAVX.hpp>


int  drawAss      (Mandelbrot *mbrot)
{
    PIXELS_CHECK(mbrot);

    int pix_amount = mbrot->heigth * mbrot->width;

    sf::RenderWindow window(sf::VideoMode(mbrot->width, mbrot->heigth), "BlackAss");
    
    sf::Texture texture;
    texture.create(mbrot->width, mbrot->heigth);    // creating texture
    
    sf::Sprite  sprite(texture);                                                    // creating sprite that will display texture

    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::Text FPS("0", font, 20);

    sf::Clock     clock;
    double        fps          = 0;
    std::string   str;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)    window.close();

        }

        if (fillImage(mbrot))   window.close();

        mbrot->Pixels -= pix_amount;

        texture.update((const uint8_t *) (mbrot->Pixels));

        window.clear();
        window.draw(sprite);
        window.display();

        fps          = 1. / (clock.getElapsedTime().asSeconds());
        clock.restart();

        str = std::to_string(fps);
        
        FPS.setString(str);
        window.draw(FPS);
        window.display();
    }

    return 0;
}
