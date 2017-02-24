all : print_evens print_odds fe_test

print_evens : print_evens.c
	gcc -Wall -pedantic -ansi print_evens.c -o print_evens

print_odds : print_odds.c
	gcc -Wall -pedantic -ansi print_odds.c -o print_odds

fe_test : fe_test.c print_odds print_evens
	gcc -Wall -pedantic -ansi fe_test.c -o fe_test
