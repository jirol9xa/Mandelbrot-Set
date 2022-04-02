#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <Mandelbrot_AVX128.hpp>
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


static __m128i getColor  (float x0, float y0, __m128i n_max, __m128 r_max);
static int     fillString(Mandelbrot *mbrot, float x0, float y0);


int  mbrotCtor(Mandelbrot *mbrot)
{
    if (mbrot->Pixels)
    {
        printf("!!!ERROR Struct Mandelbrot already exist !!!\n");
        PRINT_LINE;
        return -1;
    }

    mbrot->Pixels    = (uint32_t *) calloc(WIDTH(mbrot) * HEIGTH(mbrot) , sizeof(uint32_t));
    mbrot->Pixels    = mbrot->Pixels;
    
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


static __m128i getColor(__m128 x0, __m128 y0, const int n_max, __m128 r_max)
{
    int n = 0;

    __m128 x = x0, y = y0;
    __m128i N = _mm_setzero_si128();

    for ( ; n < n_max; ++n)
    {
        __m128 X = _mm_mul_ps(x, x), Y = _mm_mul_ps(y, y), XY = _mm_mul_ps(x, y);

        __m128 cmp = _mm_cmple_ps(_mm_add_ps(X, Y), r_max);
        int mask    = _mm_movemask_ps(cmp);
        
        if (!mask)  return N;

        N = _mm_sub_epi32   (N, _mm_castps_si128(cmp));

        x = _mm_add_ps(_mm_sub_ps(X, Y),   x0);
        y = _mm_add_ps(_mm_add_ps(XY, XY), y0);
    }

    return N;
}


int fillImage(Mandelbrot *mbrot)
{
    PIXELS_CHECK(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))   return QUIT_CDM(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))     
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) -= 10.f : XC(mbrot) -= 0.1f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) += 10.f : XC(mbrot) += 0.1f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))       
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) -= 10.f : YC(mbrot) -= 0.1f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) += 10.f : YC(mbrot) += 0.1f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))     
    {
        SCALE(mbrot) -= 0.01f;
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))        
    {
        SCALE(mbrot) += 0.01f; 
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }

    int   heigth = HEIGTH(mbrot);
    int   width  = WIDTH(mbrot);
    float Dx     = DX(mbrot);
    float Dy     = DY(mbrot);
    float Xc     = XC(mbrot);
    float Yc     = YC(mbrot);

    for (int yi = 0; yi < heigth; ++yi, PIXELS(mbrot) += width)
    {
        float  x0 = (-1.f * width              / 2) * Dx + Xc * (6.f / width);
        float  y0 = ((float) yi - 1.f * heigth / 2) * Dy + Yc * (4.f / heigth);

        fillString(mbrot, x0, y0);
    }

    return 0;
}


static int fillString(Mandelbrot *mbrot, float x0, float y0)
{
    PIXELS_CHECK(mbrot);

    int       n_max  = N_MAX(mbrot);
    __m128    r_max  = R_MAX(mbrot);
    __m128    dx     = _mm_set1_ps(DX(mbrot));
    __m128   _3210   = _mm_set_ps(3.f, 2.f, 1.f, 0.f);
    uint32_t *Pixels = PIXELS(mbrot);
    
    __m128   x00     = _mm_add_ps(_mm_set1_ps(x0), _mm_mul_ps(_3210, dx)), 
             y00     = _mm_set1_ps(y0); 
    __m128   _4      = _mm_set1_ps(4.f);
    __m128   _255    = _mm_set1_ps(255.f);

    for (int xi = 0; xi < WIDTH(mbrot); xi += 4, x00 = _mm_add_ps(x00, _mm_mul_ps(dx, _4)))
    {
        __m128i n = getColor(x00, y00, n_max, r_max);
        
        uint32_t *pn = (uint32_t *) &n;
        
        for (int i = 0; i < 4; ++i)
        {
            int pix = pn[i]; 
            Pixels[xi + i]  = 0xFF000000 + sin(pix) * (2 << 20) + pow(pix, 2) * (2 << 11) + tan(pix) * (2 << 15);
        }
    }

    return 0;
}
