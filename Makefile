CC = g++
I_FLAG = -I ./
DEBUG_FLAGS  = -fsanitize=address,leak,undefined -Wall -g


all:	noSSE

build:

SSE:

noSSE: 1-main.o 1-noSSE.o draw.o
	$(CC)	1-main.o 1-noSSE.o draw.o -o graph -lsfml-graphics -lsfml-window -lsfml-system $(DEBUG_FLAGS)
clear:
	rm -rf *.o


1-main.o: 		1-main.cpp
	$(CC) 	-c 	1-main.cpp	$(I_FLAG)
1-noSSE.o:		1-noSSE.cpp
	$(CC)	-c	1-noSSE.cpp $(I_FLAG)
draw.o:			draw.cpp
	$(CC)	-c 	draw.cpp	$(I_FLAG)
test.o:			test.cpp
	$(CC)	-c	test.cpp	$(I_FLAG)
