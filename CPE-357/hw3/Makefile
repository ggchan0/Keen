Smake : shake_and_smake.o smake_structures.o text_utils.o
	gcc -ansi -pedantic -Wall shake_and_smake.o smake_structures.o text_utils.o -o Smake

shake_and_smake.o : shake_and_smake.c
	gcc -ansi -pedantic -Wall -c shake_and_smake.c

smake_structures.o : smake_structures.c smake_structures.h
	gcc -ansi -pedantic -Wall -c smake_structures.c

text_utils.o : text_utils.c text_utils.h
	gcc -ansi -pedantic -Wall -c text_utils.c

clean :
	rm *.o Smake
