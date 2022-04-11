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

### My laptop has AMD ryzen 5 4500u installed. Therefore, all performance results are carried out on this processor. The measurement of the number of constructed frames was carried out for 5 seconds
|    |noAVX|AVXdoubles|AVXfloats|AVX doubles cycle without drawing in SFML|
|:--:|:---:|:--------:|:-------:|:--------------:|
|scale = 1; Xc = 0; Yc = 0    | 25 | 343 | 204 | 435  |
|scale = 1,25; Xc= 0; Yc = 0  | 17 | 242 | 144 | 298  |
|scale = 2; Xc = -100; Yc = 0 | 7  | 114 | 62  | 126  |
|scale = 3,3; Xc = 150; Yc = 0| 7  | 115 | 62  | 127  |
|scale = 0,5; Xc = 0; Yc = 0  | 82 | 717 | 553 | 1248 |


# Conclusion

### Using AVX instructions on double gives a 1288% performance gain, using AVX on float by 721%. To, exclude the influence of the graphics library, tests were conducted without drawing (AVX float loops), which gave an increase of 1626%. It can be concluded that the contribution of the graphics library is quite significant (performance decline by 338%)