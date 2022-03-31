#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <Mandelbrot_AVX.hpp>
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


static __m256i getColor  (double x0, double y0, __m256i n_max, __m256d r_max);
static int     fillString(Mandelbrot *mbrot, double x0, double y0);


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


static __m256i getColor(__m256d x0, __m256d y0, const int n_max, __m256d r_max)
{
    int n = 0;

    __m256d x = x0, y = y0;
    __m256i N = _mm256_setzero_si256();

    for ( ; n < n_max; ++n)
    {
        __m256d X = _mm256_mul_pd(x, x), Y = _mm256_mul_pd(y, y), XY = _mm256_mul_pd(x, y);

        //printf("X2 + Y2 = %lg\n", X + Y);

        __m256d cmp = _mm256_cmp_pd(_mm256_add_pd(X, Y), r_max, _CMP_LE_OS);
        int mask    = _mm256_movemask_pd(cmp);
        
        if (!mask)
        {
            return N = _mm256_mul_epi32(N, _mm256_set1_epi64x(-1));
        }

        N = _mm256_sub_epi64(N, _mm256_castpd_si256(cmp));

        x = _mm256_add_pd(_mm256_sub_pd(X, Y),   x0);
        y = _mm256_add_pd(_mm256_add_pd(XY, XY), y0);
    }

    return N;
}


int fillImage(Mandelbrot *mbrot)
{
    PIXELS_CHECK(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))   return QUIT_CDM(mbrot);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))     
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) -= 10. : XC(mbrot) -= 0.1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? XC(mbrot) += 10. : XC(mbrot) += 0.1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))       
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) -= 10. : YC(mbrot) -= 0.1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? YC(mbrot) += 10. : YC(mbrot) += 0.1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) )//&& SCALE(mbrot) > 0.5)        
    {
        SCALE(mbrot) -= 0.01;
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))        
    {
        SCALE(mbrot) += 0.01; 
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }

    for (int yi = 0; yi < HEIGTH(mbrot); ++yi, PIXELS(mbrot) += WIDTH(mbrot))
    {
        double  x0 = (-1. * WIDTH(mbrot)               / 2) * DX(mbrot) + XC(mbrot) * (6. / mbrot->width);
        double  y0 = ((double) yi - 1. * HEIGTH(mbrot) / 2) * DY(mbrot) + YC(mbrot) * (4. / mbrot->heigth);

        fillString(mbrot, x0, y0);
    }

    return 0;
}


static int fillString(Mandelbrot *mbrot, double x0, double y0)
{
    PIXELS_CHECK(mbrot);

    int       n_max  = N_MAX(mbrot);
    __m256d   r_max  = R_MAX(mbrot);
    __m256d   dx     = _mm256_set1_pd(DX(mbrot));
    __m256d  _3210   = _mm256_set_pd(3., 2., 1., 0.);
    uint32_t *Pixels = PIXELS(mbrot);
    
    __m256d   x00    = _mm256_add_pd(_mm256_set1_pd(x0), _mm256_mul_pd(_3210, dx)), 
              y00    = _mm256_set1_pd(y0); 
    __m256d   _4     = _mm256_set1_pd(4.);
    __m256    _255   = _mm256_set1_ps(255.);

    for (int xi = 0; xi < WIDTH(mbrot); xi += 4, x00 = _mm256_add_pd(x00, _mm256_mul_pd(dx, _4)))
    {
        __m256i n = getColor(x00, y00, n_max, r_max);
        
        uint64_t *pn = (uint64_t *) &n;
        
        for (int i = 0; i < 4; ++i)
        {
            int pix = pn[i]; 
            Pixels[xi + i]  = 0xFF000000 + sin(pix) * (2 << 20) + pow(pix, 2) * (2 << 11) + tan(pix) * (2 << 15);
            //printf("color = %x\n", Pixels[xi]);
        }
    }

    return 0;
}
