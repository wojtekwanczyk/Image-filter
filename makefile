
CC = gcc
CFLAGS = -g -O2 -std=gnu99 -Wall -pthread -lrt


all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f *.o main


test1:
	./main 1 f14.ascii.pgm filter.txt out.ascii.pgm
	./main 2 f14.ascii.pgm filter.txt out.ascii.pgm
	./main 4 f14.ascii.pgm filter.txt out.ascii.pgm
	./main 8 f14.ascii.pgm filter.txt out.ascii.pgm

test2:
	./main 1 mountain.ascii.pgm filter.txt out.ascii.pgm
	./main 2 mountain.ascii.pgm filter.txt out.ascii.pgm
	./main 4 mountain.ascii.pgm filter.txt out.ascii.pgm
	./main 8 mountain.ascii.pgm filter.txt out.ascii.pgm


test3:
	./main 1 f14.ascii.pgm filter2.txt out.ascii.pgm
	./main 2 f14.ascii.pgm filter2.txt out.ascii.pgm
	./main 4 f14.ascii.pgm filter2.txt out.ascii.pgm
	./main 8 f14.ascii.pgm filter2.txt out.ascii.pgm

test4:
	./main 1 mountain.ascii.pgm filter2.txt out.ascii.pgm
	./main 2 mountain.ascii.pgm filter2.txt out.ascii.pgm
	./main 4 mountain.ascii.pgm filter2.txt out.ascii.pgm
	./main 8 mountain.ascii.pgm filter2.txt out.ascii.pgm


test5:
	./main 1 dragon.ascii.pgm filter2.txt out.ascii.pgm
	./main 2 dragon.ascii.pgm filter2.txt out.ascii.pgm
	./main 4 dragon.ascii.pgm filter2.txt out.ascii.pgm
	./main 8 dragon.ascii.pgm filter2.txt out.ascii.pgm

test6:
	./main 1 dragon.ascii.pgm filter3.txt out.ascii.pgm




