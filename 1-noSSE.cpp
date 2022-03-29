#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>
#include "stdio.h"
#include <cmath>


#define BLACK 0x000000FF
#define WHITE 0xFFFFFFFF


const int n_max     = 256;
const double r_max  = 100.;

double dx     = 6. / WindowSettings::width, dy = 4. / WindowSettings::heigth;
double  scale = 1.;   // mb not used
double xC = 0., yC = 0.;


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
            //printf("Here n = %d\n", n);
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))     xC -= 10.;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    xC += 10.;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))       yC -= 10.;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))     yC += 10.;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))        
    {
        scale -= 0.1;
        dx *= scale; dy *= scale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))        
    {
        scale += 0.1; 
        dx *= scale; dy *= scale;
    }

    for (int yi = 0; yi < WindowSettings::heigth; ++yi, Pixels += WindowSettings::width)
    {
        double  x0 = (-1. * WindowSettings::width / 2 + xC) * dx;
        double  y0 = ((double) yi - 1. * WindowSettings::heigth / 2 + yC) * dy;

        fillString(Pixels, x0, y0);
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
        Pixels[xi] = (sin(n)) * 256 + n + tan(n) * 0x00228322  + tan(n * n + log10(n + 1) * 512) + cos(pow(tan(n * n), 2)) + 0xFF000000;
    }

    return 0;
}
