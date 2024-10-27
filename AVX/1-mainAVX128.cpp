#include <SFML/Graphics.hpp>
#include <Mandelbrot_AVX128.hpp>


int main()
{
    Mandelbrot  mbrot = {};
    mbrotCtor(&mbrot);

    drawAss(&mbrot);
    
    mbrotDtor(&mbrot);
    return 0;
}