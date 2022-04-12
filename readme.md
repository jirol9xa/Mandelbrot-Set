# Mandelbroto set visualizer

### This repository is designed to draw the Mandelbrot set using the SFML graphics library. The repository contains 4 implementations of frame calculation:
<br/>

1. Simple implementation using functions
2. Implementation using AVX instructions based on double values
3. Implementation on AVX with float values

<br/>

### To control the picture
Zoom in - A, zoom out - Z, the movement is carried out using the arrows., Space + "arrows" - fast movement.

<br/>

### Installation
    git clone https://https://github.com/jirol9xa/Mandelbrot-Set

For running noAVX version  

    make noAVX && ./noavx

For running AVX doubles

    make AVXfor && ./avxfor

For running AVX floats loop

    make AVX128for && ./avx128for

# Performance research

### My laptop has AMD ryzen 5 4500u installed. Here are performance tests when rendering each pixel 1000 times to avoid the error introduced by the graphics library. The time is measured by rendering 3 frames for each version. Such a large number of constructions were chosen in order to reduce the error introduced by the use of the graphics library by less than 5%.
|noAVX   |noAVX -O3 |AVXdoubles -O3|AVXfloats -O3|AVX doubles cycle without drawing in SFML -O3|
|:------:|:--------:|:------------:|:-----------:|:-------------------------------------------:|
| 361.86 | 142.961  | 23.2755      | 39.6995     | 22.9232                                     |


# Conclusion

### Using AVX instructions on double speeds up the program 6 times (compared to the version without AVX, but with -O3), using float - 3.6 times. In turn, just compiling with the optimization flag -O3 already speeds up the execution process by 2.5 times. Due to the large number of constructions of one frame, the error introduced by the graphics library is 1.5%.