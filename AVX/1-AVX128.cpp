#include <SFML/Graphics.hpp>
#include <Mandelbrot_AVX128.hpp>
#include "stdio.h"
#include <arm_neon.h>
#include <cassert>
#include <cmath>
#include <cstdlib>


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


// static __m128i getColor  (float x0, float y0, __m128i n_max, __m128 r_max);
static int     fillString(Mandelbrot *mbrot, float x0, float y0);


int  mbrotCtor(Mandelbrot *mbrot)
{
    if (mbrot->Pixels)
    {
        printf("!!!ERROR Struct Mandelbrot already exist !!!\n");
        PRINT_LINE;
        return -1;
    }

    mbrot->Pixels    = (uint32_t *) std::aligned_alloc(128 / 8, WIDTH(mbrot) * HEIGTH(mbrot) * sizeof(uint32_t));
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


static int32x4_t getColor(float32x4_t x0, float32x4_t y0, const int n_max, float32x4_t r_max)
{
    int n = 0;

    float32x4_t x = x0, y = y0;
    int32x4_t N = vdupq_n_s32(0);

    for ( ; n < n_max; ++n)
    {
        float32x4_t X  = vmulq_f32(x, x),
                    Y  = vmulq_f32(y, y),
                    XY = vmulq_f32(x, y);

        // compare less equal
        uint32x4_t cmp = vcltq_f32(vaddq_f32(X, Y), r_max);
        if (
            !vgetq_lane_u32(cmp, 0) && !vgetq_lane_u32(cmp, 1) &&
            !vgetq_lane_u32(cmp, 2) && !vgetq_lane_u32(cmp, 3)
        ) { 
            return N;
        }

        // Mb need explicitly convert cmp to signed integer type?
        N = vsubq_s32(N, cmp);

        x = vaddq_f32(vsubq_f32(X, Y),   x0);
        y = vaddq_f32(vaddq_f32(XY, XY), y0);
    }

    return N;
}


int fillImage(Mandelbrot *mbrot)
{
    assert(mbrot);
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

    PIXELS(mbrot) -= width * heigth;

    return 0;
}


static int fillString(Mandelbrot *mbrot, float x0, float y0)
{
    PIXELS_CHECK(mbrot);

    int n_max  = N_MAX(mbrot);
    float32x4_t r_max = vdupq_n_f32(R_MAX(mbrot));
    float32x4_t dx    = vdupq_n_f32(DX(mbrot));
    float32x4_t _3210 = {3.f, 2.f, 1.f, 0.f};
    uint32_t *Pixels = PIXELS(mbrot);
    
    float32x4_t x00     = vaddq_f32(vdupq_n_f32(x0), vmulq_f32(_3210, dx)), 
                y00     = vdupq_n_f32(y0); 
    float32x4_t _4 = vdupq_n_f32(4.f);
    [[maybe_unused]] float32x4_t _255 = vdupq_n_f32(255.f);

    for (int xi = 0; xi < WIDTH(mbrot); xi += 4, x00 = vaddq_f32(x00, vmulq_f32(dx, _4)))
    {
        int32x4_t n = getColor(x00, y00, n_max, r_max);
        
        uint32_t *pn = (uint32_t *) &n;
        
        for (int i = 0; i < 4; ++i)
        {
            int pix = pn[i]; 
            Pixels[xi + i]  = 0xFF000000 + sin(pix) * (2 << 12) + pow(pix, 2) * (2 << 5) + tan(pix) * (2 << 10);
        }
    }

    return 0;
}
