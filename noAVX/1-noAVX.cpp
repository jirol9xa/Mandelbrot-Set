#include <SFML/Graphics.hpp>
#include <Mandelbrot_noAVX.hpp>
#include "stdio.h"
#include <cmath>


#define PIXELS(mbrot)       mbrot->Pixels
#define SCALE(mbrot)        mbrot->scale
#define N_MAX(mbrot)        mbrot->n_max
#define R_MAX(mbrot)        mbrot->r_max
#define DX(mbrot)           mbrot->dx
#define DY(mbrot)           mbrot->dy
#define XC(mbrot)           mbrot->xC
#define YC(mbrot)           mbrot->yC   
#define WIDTH(mbrot)        mbrot->width
#define HEIGTH(mbrot)       mbrot->heigth
#define NULL_PIXEL(mbrot)   mbrot->nullptr_pixel
#define NULL_ARRAY(mbrot)   mbrot->nullptr_array
#define QUIT_CDM(mbrot)     mbrot->quit_cmd

#define PRINT_LINE          printf("[%s:%d]\n", __func__, __LINE__)  


int  mbrotCtor(Mandelbrot *mbrot)
{
    if (mbrot->Pixels)
    {
        printf("!!!ERROR Struct Mandelbrot already exist !!!\n");
        PRINT_LINE;
        return -1;
    }

    mbrot->Pixels = (uint32_t *) calloc(WIDTH(mbrot) * HEIGTH(mbrot) , sizeof(uint32_t));
    
    return 0;
}


int mbrotDtor(Mandelbrot *mbrot)
{
    if (!mbrot->Pixels)
    {
        printf("!!! ERROR Struct Mandelbrot already destroyed or not exist !!!\n");
        PRINT_LINE;
        return -1;
    }

    free(mbrot->Pixels);

    return 0;
}


int getColor(double x0, double y0, int n_max, double r_max)
{
    int n = 0;

    double x = x0, y = y0;

    for ( ; n < n_max; ++n)
    {
        double X = x * x, Y = y * y, XY = x * y;

        if (X + Y >= r_max)
        {
            return n;
        }

        x = X - Y  + x0;
        y = 2 * XY + y0;
    }

    return 0;
}


int fillImage(Mandelbrot *mbrot)
{
    PIXELS_CHECK(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))   return QUIT_CDM(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))     
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) -= 10. : XC(mbrot) -= 0.2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) += 10. : XC(mbrot) += 0.2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))       
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) -= 10. : YC(mbrot) -= 0.2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) += 10. : YC(mbrot) += 0.2;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))        
    {
        SCALE(mbrot) -= 0.01;
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))        
    {
        SCALE(mbrot) += 0.01; 
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }

    int    heigth = HEIGTH(mbrot);
    int    width  = WIDTH(mbrot);
    double Dx     = DX(mbrot);
    double Dy     = DY(mbrot);
    double Xc     = XC(mbrot);
    double Yc     = YC(mbrot);

    for (int yi = 0; yi < heigth; ++yi, PIXELS(mbrot) += width)
    {
        double  x0 = (-1. * width               / 2) * Dx + Xc * (6. / width);
        double  y0 = ((double) yi - 1. * heigth / 2) * Dy + Yc * (4. / heigth);

        fillString(mbrot, x0, y0);
    }

    return 0;
}


int fillString(Mandelbrot *mbrot, double x0, double y0)
{
    PIXELS_CHECK(mbrot);

    int       n_max  = N_MAX(mbrot);
    double    dx     = DX(mbrot);
    double    r_max  = R_MAX(mbrot);
    uint32_t *Pixels = PIXELS(mbrot);

    for (int xi = 0; xi < WIDTH(mbrot); ++xi, x0 += dx)
    {
        int n = getColor(x0, y0, n_max, r_max);
        Pixels[xi] = 0xFF000000 + n;//sin(n) * (2 << 20) + pow(n, 2) * (2 << 11) + tan(n) * (2 << 15);
    }

    return 0;
}
