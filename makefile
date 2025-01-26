CC=gcc
HEADERS=sdl2wrapper.h
OBJS=sdl2wrapper.o utils.o
CFLAGS=-O2
LIBS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf

all: slime

####################

slime: slime.o $(OBJS)
	$(CC) $+ -o $@ -lm $(LIBS)

slime.o: slime.c $(HEADERS)
	$(CC) -c $< -o $@  $(CFLAGS)

sdl2wrapper.o: sdl2wrapper.c $(HEADERS)
	$(CC) -c $< -o $@  $(CFLAGS)

utils.o: utils.c $(HEADERS)
	$(CC) -c $< -o $@  $(CFLAGS)

####################

clean:
	rm -f -v *.o slime
