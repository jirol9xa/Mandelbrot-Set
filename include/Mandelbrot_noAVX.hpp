#ifndef MANDELBROT_HPP
    #define MANDELBROT_HPP

    struct Mandelbrot
    {

        enum errors
        {
            nullptr_pixel = 1,
            nullptr_array = 2,
            quit_cmd      = 3
        };

        const int width  = 1200;      // window
        const int heigth = 800;       //settings

        const int n_max     = 256;
        const double r_max  = 4.;

        double dx    = 6. / width, dy = 4. / heigth;
        double scale = 1.;
        double xC    = 0., yC = 0.;        // coordinates of center of picture

        uint32_t *Pixels = nullptr;
    };

    #define PIXELS_CHECK(mbrot)                         \
    {                                                   \
        if (!mbrot->Pixels)                             \
        {                                               \
            printf("Array with pixels is nullptr\n");   \
            return mbrot->errors::nullptr_array;        \
        }                                               \
    }


    int  mbrotCtor    (Mandelbrot *mbrot);
    int  mbrotDtor    (Mandelbrot *mbrot);
    int  getGetColor  (Mandelbrot *mbrot, double x, double y);
    int  fillImage    (Mandelbrot *mbrot);
    int  fillString   (Mandelbrot *mbrot, double x0, double y0);
    int  drawAss      (Mandelbrot *mbrot); 

#endif