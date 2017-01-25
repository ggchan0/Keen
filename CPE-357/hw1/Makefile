CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = mytr
OBJS = mytr.c
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

clean :
	rm *.o $(MAIN) core
