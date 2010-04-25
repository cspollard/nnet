run: cvgl.o hnet.o main.o
	clang cvgl.o hnet.o main.o -o run -lm \
		`pkg-config opencv gl glu --cflags --libs` \
		`sdl-config --cflags --libs`

main.o: main.c
	clang main.c -c -o main.o

hnet.o: hnet.c
	clang hnet.c -c -o hnet.o

cvgl.o: cvgl.c
	clang cvgl.c -c -o cvgl.o
