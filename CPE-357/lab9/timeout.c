#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

void handleAlarm(int pid) {
   kill(pid, 9);
   exit(-1);
}

int main(int argc, char ** argv) {
   pid_t pid;
   int status;
   signal(SIGALRM, handleAlarm);
   alarm(atoi(argv[1]));
   pid = fork();
   if (pid == 0) {
      execvp(argv[2], argv + 2);
   }
   wait(&status);
   return status;
}
