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

        const int     n_max  = 256;
        const __m256d r_max  = _mm256_set1_pd (4.);

        double scale = 1.;
        double dx    = 6. / width  * scale, 
               dy    = 4. / heigth * scale;
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
    int  fillImage    (Mandelbrot *mbrot);
    int  drawAss      (Mandelbrot *mbrot); 

#endif