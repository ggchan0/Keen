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
   pid_t pid;
   int status;
   int start = 1;
   if (argc != 2) {
      fprintf(stderr, "Requires 2 arguments, %d given.\n", argc);
      exit(EXIT_FAILURE);
   }
   converted_num = atoi(argv[1]);
   if (converted_num == 0) {
      fprintf(stderr, "%d is not a valid integer.\n", converted_num);
      exit(EXIT_FAILURE);
   }
   if ((pid = fork()) >= 0) {
      if (pid == 0) {
         while (start <= converted_num) {
            if (isOdd(start) == 1) {
               printf("%d\n", start);
            }
            start++;
         }
         return 0;
      } else {
         while (start <= converted_num) {
            if (isOdd(start) == 0) {
               printf("\t%d\n", start);
            }
            start++;
         }
      }
   } else {
      fprintf(stderr, "Fork failed\n");
   }
   wait(&status);
   return 0;
}
