#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <Mandelbrot_AVX.hpp>
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


int  mbrotCtor(Mandelbrot *mbrot)
{
    if (mbrot->Pixels)
    {
        printf("!!!ERROR Struct Mandelbrot already exist !!!\n");
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
        return -1;
    }

    free(mbrot->Pixels);

    return 0;
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && SCALE(mbrot) > 0.05)        
    {
        SCALE(mbrot) -= 0.01f;
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))        
    {
        SCALE(mbrot) += 0.01f; 
        DX(mbrot)    *= SCALE(mbrot); DY(mbrot) *= SCALE(mbrot);
    }

    int       n_max  = N_MAX(mbrot);
    __m256    r_max  = R_MAX(mbrot);
    __m256    dx     = _mm256_set1_ps(DX(mbrot));
    __m256   _7_0    = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);

    int   heigth = HEIGTH(mbrot);
    int   width  = WIDTH(mbrot);
    float Dx     = DX(mbrot);
    float Dy     = DY(mbrot);
    float Xc     = XC(mbrot);
    float Yc     = YC(mbrot);

    for (int yi = 0; yi < heigth; ++yi, PIXELS(mbrot) += width)
    {
        float  x0 = (-1.f * WIDTH(mbrot)              / 2) * Dx + Xc * (6.f / width);
        float  y0 = ((float) yi - 1.f * HEIGTH(mbrot) / 2) * Dy + Yc * (4.f / heigth);

        __m256   x00     = _mm256_add_ps(_mm256_set1_ps(x0), _mm256_mul_ps(_7_0, dx)), 
                 y00     = _mm256_set1_ps(y0); 
        __m256   _8      = _mm256_set1_ps(8.f);
        __m256   _255    = _mm256_set1_ps(255.f);

        for (int xi = 0; xi < width; xi += 8, x00 = _mm256_add_ps(x00, _mm256_mul_ps(dx, _8)))
        {
            int n = 0;

            __m256 x = x00, y = y00;
            __m256i N = _mm256_setzero_si256();

            for ( ; n < n_max; ++n)
            {
                __m256 X = _mm256_mul_ps(x, x), Y = _mm256_mul_ps(y, y), XY = _mm256_mul_ps(x, y);

                __m256 cmp = _mm256_cmp_ps(_mm256_add_ps(X, Y), r_max, _CMP_LE_OS);
                int mask   = _mm256_movemask_ps(cmp);

                if (!mask)  break;

                N = _mm256_sub_epi32(N, _mm256_castps_si256(cmp));

                x = _mm256_add_ps(_mm256_sub_ps(X, Y),   x00);
                y = _mm256_add_ps(_mm256_add_ps(XY, XY), y00);
            }

            uint32_t *pn = (uint32_t *) &N;
            uint32_t *Pixels = PIXELS(mbrot);
        
            for (int i = 0; i < 8; ++i)
            {
                int pix = pn[i]; 
                Pixels[xi + i]  = 0xFF000000 + pix;//sin(pix) * (2 << 20) + pow(pix, 2) * (2 << 11) + tan(pix) * (2 << 15);
            }
        }
    }

    return 0;
}