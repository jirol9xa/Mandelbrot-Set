#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <1-noSSE.hpp>


int main()
{
    uint32_t Pixels[WindowSettings::heigth * WindowSettings::width] = {};

    //for (int i = 0; i < WindowSettings::heigth * WindowSettings::width; ++i)
    //{
    //    printf("%d ", Pixels[i]);
    //}

    drawAss(Pixels);
    
    return 0;
}