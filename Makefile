CC = g++

I_FLAG		= -I include/
DEBUG_FLAGS = -fsanitize=address,leak,undefined -Wall -g
SFML_FLAGS 	= -lsfml-graphics -lsfml-window -lsfml-system
AVX_FLAGS	= ‑D__INTEL_COMPILER_USE_INTRINSIC_PROTOTYPES 


all:	noAVX

AVX:   1-mainAVX.o 1-AVX.o drawAVX.o
	$(CC)	1-mainAVX.o 1-AVX.o 	 drawAVX.o -o avx   $(SFML_FLAGS) $(DEBUG_FLAGS)
noAVX: 1-main.o 1-noAVX.o drawNoAVX.o
	$(CC)	1-main.o    1-noAVX.o 	 drawNoAVX.o -o noavx $(SFML_FLAGS) $(DEBUG_FLAGS)
clear:
	rm -rf *.o


1-main.o: 		noAVX/1-main.cpp
	$(CC) 	-c 	noAVX/1-main.cpp   	$(I_FLAG)
1-noAVX.o:		noAVX/1-noAVX.cpp
	$(CC)	-c	noAVX/1-noAVX.cpp  	$(I_FLAG)
drawNoAVX.o:	noAVX/drawNoAVX.cpp
	$(CC)	-c 	noAVX/drawNoAVX.cpp	$(I_FLAG)
1-mainAVX.o:	AVX/1-mainAVX.cpp
	$(CC)	-mavx2 -c 	AVX/1-mainAVX.cpp 	$(I_FLAG) 
1-AVX.o:		AVX/1-AVX.cpp
	$(CC)	-mavx2 -c	AVX/1-AVX.cpp		$(I_FLAG) 
drawAVX.o:		AVX/drawAVX.cpp
	$(CC)	-mavx2 -c 	AVX/drawAVX.cpp		$(I_FLAG) 