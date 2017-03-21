all : serve_me.o web_utils.o simple_net.o
	gcc -pedantic -std=gnu99 -Wall simple_net.o serve_me.o web_utils.o -o httpd

serve_me.o : serve_me.c
	gcc -pedantic -std=gnu99 -Wall -c serve_me.c

web_utils.o : web_utils.h web_utils.c
	gcc -pedantic -std=gnu99 -Wall -c web_utils.c

simple_net.o : simple_net.c simple_net.h
	gcc -pedantic -std=gnu99 -Wall -c simple_net.c
