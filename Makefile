CC = clang++ -std=c++2a

I_FLAG		= -I include/ -I/opt/homebrew/Cellar/sfml/2.6.1/include
DEBUG_FLAGS = -O0 -fsanitize=address,undefined -Wall -g
SFML_FLAGS 	= -L/opt/homebrew/Cellar/sfml/2.6.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system
C_FLAGS		= -c -O3 -march=native

all:	noAVX

noAVX: 1-main.o 1-noAVX.o drawNoAVX.o
	$(CC)	1-main.o    1-noAVX.o 	 drawNoAVX.o 	  -o withoutavx 	    $(SFML_FLAGS) $(DEBUG_FLAGS)
AVX128: 1-mainAVX128.o 1-AVX128.o drawAVX128.o
	$(CC) 1-mainAVX128.o 1-AVX128.o drawAVX128.o   -o  avx128 $(SFML_FLAGS) $(DEBUG_FLAGS)
clear:
	rm -rf *.o


1-main.o: 		noAVX/1-main.cpp
	$(CC) 	-c noAVX/1-main.cpp   		   	 $(I_FLAG) $(DEBUG_FLAGS)
1-noAVX.o:		noAVX/1-noAVX.cpp
	$(CC)	-c noAVX/1-noAVX.cpp  			 $(I_FLAG) $(DEBUG_FLAGS)
drawNoAVX.o:	noAVX/drawNoAVX.cpp
	$(CC)	-c noAVX/drawNoAVX.cpp			 $(I_FLAG) $(DEBUG_FLAGS)

1-mainAVX128.o:	AVX/1-mainAVX128.cpp
	$(CC)	$(C_FLAGS) 	AVX/1-mainAVX128.cpp $(I_FLAG) $(DEBUG_FLAGS)
1-AVX128.o: 	AVX/1-AVX128.cpp
	$(CC)	$(C_FLAGS)	AVX/1-AVX128.cpp  $(I_FLAG) $(DEBUG_FLAGS)
drawAVX128.o:		AVX/drawAVX128.cpp
	$(CC)	$(C_FLAGS)	AVX/drawAVX128.cpp   $(I_FLAG) $(DEBUG_FLAGS)
