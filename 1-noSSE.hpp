enum WindowSettings
{
    width  = 2400,//1200,
    heigth = 1600//800
};

enum ERRORS
{
    nullptrPixel = 1,
    nullptrArray = 2,
};


#define PIXELS_CHECK(Pixels)                        \
{                                                   \
    if (!Pixels)                                    \
    {                                               \
        printf("Array with pixels is nullptr\n");   \
        return ERRORS::nullptrArray;                \
    }                                               \
}


int  getGetColor  (double x, double y);
int  fillImage    (uint32_t  *Pixels);
int  fillString   (uint32_t  *Pixels, double x0, double y0);
int  drawAss      (uint32_t  *Pixels); 

