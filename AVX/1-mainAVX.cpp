#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <Mandelbrot_AVX.hpp>


int main()
{
    Mandelbrot  mbrot = {};
    mbrotCtor(&mbrot);

    drawAss(&mbrot);
    
    mbrotDtor(&mbrot);
    return 0;
}