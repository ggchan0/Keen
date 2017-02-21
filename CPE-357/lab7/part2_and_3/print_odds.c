#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int isOdd(int num) {
   return num % 2 == 1;
}

int main(int argc, char ** argv) {
   int converted_num;
   int start = 1;
   printf("Running odds\n");
   if (argc != 2) {
      fprintf(stderr, "Requires 2 arguments, %d given.\n", argc);
      exit(EXIT_FAILURE);
   }
   converted_num = atoi(argv[1]);
   if (converted_num == 0) {
      fprintf(stderr, "%d is not a valid integer.\n", converted_num);
      exit(EXIT_FAILURE);
   }
   while (start <= converted_num) {
      if (isOdd(start) == 1) {
         printf("%d\n", start);
      }
      start++;
   }
   return 0;
}
