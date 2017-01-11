CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = a.out
DEPS = part6.c part6_tests.c part6.h

all : $(MAIN)

$(MAIN) : $(DEPS)
	$(CC) $(CFLAGS) part6_tests.c
	./$(MAIN)

clean :
	rm *.o $(MAIN) core
