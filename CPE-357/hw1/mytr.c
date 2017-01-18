#include <stdio.h>
#include <string.h>

int get_mode(int argc, char ** args) {
   int i;
   int code = 0;
   for (i = 0; i < argc; i++) {
	char *arg = *(args + i);
	printf("%s\n", arg);
	if (strcmp(arg, "-d") == 0) {
	   code = 1;
	}
   }
   return code;
}

int main(int argc, char **argv) {
   if (argc != 3) {
	printf("There are not enough arguments given\n");
	return 1;
   }
   int x = get_mode(argc, argv);
   printf("%d\n", x);
   int c;
   while ((c = getchar()) != EOF) {
	putchar(c);
   }
   return 0;
}
