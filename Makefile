CC = g++

I_FLAG		= -I include/
DEBUG_FLAGS = -fsanitize=address,leak,undefined -Wall -g
SFML_FLAGS 	= -lsfml-graphics -lsfml-window -lsfml-system

all:	noAVX

AVX:   1-mainAVX.o 1-AVX.o drawAVX.o
	$(CC)	1-mainAVX.o 1-AVX.o 	 drawAVX.o 		  -o avx   	   $(SFML_FLAGS) 
AVX128: 1-mainAVX128.o 1-AVX128.o drawAVX128.o
	$(CC) 	1-mainAVX128.o 1-AVX128.o	 drawAVX128.o -o avx128    $(SFML_FLAGS) 
noAVX: 1-main.o 1-noAVX.o drawNoAVX.o
	$(CC)	1-main.o    1-noAVX.o 	 drawNoAVX.o 	  -o noavx 	   $(SFML_FLAGS) 
AVX128for: 1-mainAVX128.o 1-AVX128for.o drawAVX128.o
	$(CC) 1-mainAVX128.o 1-AVX128for.o drawAVX128.o  -o  avx128for $(SFML_FLAGS) 
clear:
	rm -rf *.o


1-main.o: 		noAVX/1-main.cpp
	$(CC) 	-c 	noAVX/1-main.cpp   		   	 $(I_FLAG)
1-noAVX.o:		noAVX/1-noAVX.cpp
	$(CC)	-c	noAVX/1-noAVX.cpp  			 $(I_FLAG)
drawNoAVX.o:	noAVX/drawNoAVX.cpp
	$(CC)	-c 	noAVX/drawNoAVX.cpp			 $(I_FLAG)

1-mainAVX.o:	AVX/1-mainAVX.cpp
	$(CC)	-mavx2 -c 	AVX/1-mainAVX.cpp 	 $(I_FLAG) 
1-mainAVX128.o:	AVX/1-mainAVX128.cpp
	$(CC)	-mavx2 -c 	AVX/1-mainAVX128.cpp $(I_FLAG)
1-AVX.o:		AVX/1-AVX.cpp
	$(CC)	-mavx2 -c	AVX/1-AVX.cpp	  	 $(I_FLAG) 
1-AVX128.o:		AVX/1-AVX128.cpp
	$(CC)	-mavx2 -c 	AVX/1-AVX128.cpp 	 $(I_FLAG)
1-AVX128for.o: 	AVX/1-AVX128for.cpp
	$(CC)	-mavx2 -c 	AVX/1-AVX128for.cpp  $(I_FLAG)
drawAVX.o:		AVX/drawAVX.cpp
	$(CC)	-mavx2 -c 	AVX/drawAVX.cpp		 $(I_FLAG) 
drawAVX128.o:		AVX/drawAVX128.cpp
	$(CC)	-mavx2 -c 	AVX/drawAVX128.cpp   $(I_FLAG) 
