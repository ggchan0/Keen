#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv) {
   int converted_num;
   pid_t pid1, pid2;
   int status;
   int print_evens = 0;
   char *args[3];
   args[0] = "fe_test";
   args[1] = argv[1];
   args[2] = NULL;
   if (argc != 2) {
      fprintf(stderr, "Requires 2 arguments, %d given.\n", argc);
      exit(EXIT_FAILURE);
   }
   converted_num = atoi(argv[1]);
   if (converted_num == 0) {
      fprintf(stderr, "%d is not a valid integer.\n", converted_num);
      exit(EXIT_FAILURE);
   }
   if ((pid1 = fork()) < 0) {
      fprintf(stderr, "First fork failed\n");
      exit(EXIT_FAILURE);
   } else {
      if (pid1 == 0) {
         if (print_evens == 0) {
            execvp("./print_evens", args);
            perror("Failed\n");
         } else {
            execvp("./print_odds", args);
            perror("Failed\n");
         }
      }
      print_evens++;
   }
   if ((pid2 = fork()) < 0) {
      fprintf(stderr, "Second fork failed\n");
      exit(EXIT_FAILURE);
   } else {
      if (pid2 == 0) {
         if (print_evens == 0) {
            execvp("./print_evens", args);
            perror("Failed\n");
         } else {
            execvp("./print_odds", args);
            perror("Failed\n");
         }
      }
      print_evens++;
   }
   wait(&status);
   wait(&status);
   return 0;
}
