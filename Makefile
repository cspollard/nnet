run: nnet.o main.o
	clang nnet.o main.o -o run

main.o: main.c
	clang main.c -c -o main.o

nnet.o: nnet.c
	clang nnet.c -c -o nnet.o
