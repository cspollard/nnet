run: hnet.o main.o
	clang hnet.o main.o -o run -lm

main.o: main.c
	clang main.c -c -o main.o

hnet.o: hnet.c
	clang hnet.c -c -o hnet.o 
