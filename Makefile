CC=gcc
CFLAGS=-lm -Wall -lncurses
IN=include
OBJS= src/ui.c src/ec_graphs.c src/draw_nc.c src/utils.c
all:
	$(CC) main.c $(OBJS) $(CFLAGS)

elf:
	$(CC) main.c $(OBJS) $(CFLAGS) -no-pie

command:
	echo "$(CC) main.c $(OBJS) $(CFLAGS)"

clean:
	rm a.out

