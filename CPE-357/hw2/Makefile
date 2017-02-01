CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = guess
OBJS = guess.o utilitrees.o text_utils.o
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

guess.o : guess.c
	$(CC) $(CFLAGS) -c guess.c 

utilitrees.o : utilitrees.c utilitrees.h
	$(CC) $(CFLAGS) -c utilitrees.c

text_utils.o : text_utils.c text_utils.h
	$(CC) $(CFLAGS) -c text_utils.c

clean :
	rm *.o $(MAIN) core
