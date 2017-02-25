all : utilitrees.o directrees.o
	gcc -std=gnu99 -pedantic -Wall utilitrees.o directrees.o -o tree

directrees.o : directrees.c
	gcc -std=gnu99 -pedantic -Wall directrees.c -c

utilitrees.o : utilitrees.h utilitrees.c
	gcc -std=gnu99 -pedantic -Wall utilitrees.c -c
