#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>
#include "stdio.h"
#include <cmath>


#define BLACK 0x000000FF
#define WHITE 0xFFFFFFFF


const int n_max     = 256;
const double dx     = 6. / WindowSettings::width, dy = 4. / WindowSettings::heigth;
const double r_max  = 100.;
const double  scale = 1.;   // mb not used


int getColor(double x0, double y0)
{
    int n = 0;

    double x = x0, y = y0;
    for ( ; n < n_max; ++n)
    {
        double X = x * x, Y = y * y, XY = x * y;

        //printf("X2 + Y2 = %lg\n", X + Y);

        if (X + Y >= r_max)
        {
            printf("Here n = %d\n", n);
            return n;
        }

        x = X - Y  + x0;
        y = 2 * XY + y0;
    }

    return 0;
}


int fillImage(uint32_t *Pixels)
{
    PIXELS_CHECK(Pixels);

    for (int yi = 0; yi < WindowSettings::heigth; ++yi, Pixels += WindowSettings::width)
    {
        double  x0 = (-1. * WindowSettings::width / 2) * dx * scale;
        double  y0 = ((double) yi - 1. * WindowSettings::heigth / 2) * dy * scale;

        fillString(Pixels, x0, y0);
        //printf("One more string\n");
    }

    return 0;
}


int fillString(uint32_t *Pixels, double x0, double y0)
{
    PIXELS_CHECK(Pixels);

    for (int xi = 0; xi < WindowSettings::width; ++xi, x0 += dx)
    {
        //printf("one more pixel\n");
        int n = getColor(x0, y0);
        Pixels[xi] = 0xFF + (sin(n) + n * n + tan(n)) * 16;
    }

    return 0;
}











//int main(const int argc, const char *argv[])
//{
//    sf::RenderWindow window(sf::VideoMode(600, 800), "Mondel cheta");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Red);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)    window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}