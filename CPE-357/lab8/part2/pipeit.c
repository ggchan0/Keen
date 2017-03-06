#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int getArgLength(char ** argv, int start, int end) {
   int count = 0;
   while (start < end && strcmp(argv[start], ";") != 0) {
      start++;
      count++;
   }
   return count;
}

char **createArgSet(char ** argv, int start, int length) {
   char **args = malloc(sizeof(char *) * length);
   int i;
   for (i = 0; i < length; i++) {
      args[i] = argv[start + i];
   }
   return args;
}

void printArgs(char **argv, int length) {
   int i;
   printf("---------\n");
   for (i = 0; i < length; i++) {
      printf("%s\n", argv[i]);
   }
}

pid_t checked_fork() {
   pid_t pid = fork();
   if (pid < 0) {
      perror("");
      exit(-1);
   }
   return pid;
}

int main(int argc, char ** argv) {
   int fds[2];
   pid_t pid_1, pid_2;
   char **args_1, **args_2;
   int arr_len_1 = getArgLength(argv, 1, argc);
   int arr_len_2 = getArgLength(argv, 2 + arr_len_1, argc);
   int start_1 = 1;
   int start_2 = 1 + start_1 + arr_len_1;
   /*
   printf("%d %d\n", arr_len_1, start_1);
   printf("%d %d\n", arr_len_2, start_2);
   printf("%s\n", argv[start_1]);
   printf("%s\n", argv[start_2]);
   */
   args_1 = createArgSet(argv, start_1, arr_len_1);
   args_2 = createArgSet(argv, start_2, arr_len_2);
   /*
   printArgs(args_1, arr_len_1);
   printArgs(args_2, arr_len_2);
   */
   pipe(fds);
   pid_1 = checked_fork();
   if (pid_1 == 0) {
      close(fds[0]);
      dup2(fds[1], 1);
      close(fds[0]);
      execvp(argv[start_1], args_1);
      perror("");
      exit(-1);
   }
   pid_2 = checked_fork();
   if (pid_2 == 0) {
      close(fds[1]);
      dup2(fds[0], 0);
      close(fds[1]);
      execvp(argv[start_2], args_2);
      perror("");
      exit(-1);
   }
   free(args_1);
   free(args_2);
   close(fds[0]);
   close(fds[1]);
   wait(NULL);
   wait(NULL);
   return 0;
}