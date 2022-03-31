#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <Mandelbrot_noAVX.hpp>


int main()
{
    Mandelbrot  mbrot = {};
    mbrotCtor(&mbrot);

    drawAss(&mbrot);
    
    mbrotDtor(&mbrot);
    return 0;
}