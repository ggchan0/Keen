CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = a.out
OBJS = part1.c part1_tests.c
all : $(MAIN)

$(MAIN) :  $(OBJS) part1.h
		$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

clean :
		rm *.o $(MAIN) core

