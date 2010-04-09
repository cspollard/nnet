run: nnet.o main.o
	clang nnet.o main.o draw.o -o run -lm

main.o: main.c
	clang main.c -c -o main.o

nnet.o: nnet.c
	clang nnet.c -c -o nnet.o 

draw.o: draw.c
	clang draw.c -c -o draw.o 
