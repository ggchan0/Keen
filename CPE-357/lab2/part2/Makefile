CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = a.out
OBJS = part2.c part2_tests.c
all : $(MAIN)

$(MAIN) : $(OBJS) part2.h
		$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

clean :
		rm *.o $(MAIN) core

