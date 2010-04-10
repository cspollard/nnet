run: nnet.o main.o draw.o
	clang main.o nnet.o draw.o -g -o run -lm -lGL `sdl-config --cflags --libs` -pthread

main.o: main.c
	clang main.c -c -o main.o -pthread

nnet.o: nnet.c
	clang nnet.c -c -o nnet.o 

draw.o: draw.c
	clang draw.c -c -o draw.o 
