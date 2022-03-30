#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>


int main()
{
    Mandelbrot  mbrot = {};
    mbrotCtor(&mbrot);

    drawAss(&mbrot);
    
    mbrotDtor(&mbrot);
    return 0;
}