all : sfind.o find_utils.o
	gcc -std=gnu99 -pedantic -Wall sfind.o find_utils.o -o sfind

sfind.o : sfind.c
	gcc -std=gnu99 -pedantic -Wall -c sfind.c

find_utils.o : find_utils.h find_utils.c
	gcc -std=gnu99 -pedantic -Wall -c find_utils.c

