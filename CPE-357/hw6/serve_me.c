#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "simple_net.h"

#define QUEUE_LIMIT 10
#define KB 1024

void handleSignal(int signo) {
   wait(NULL);
}

void setup(int signo) {
   struct sigaction handler;
   handler.sa_handler = handleSignal;
   if (sigemptyset(&handler.sa_mask) == -1) {
      perror("");
      exit(-1);
   }
   handler.sa_flags = SA_RESTART;
   sigaction(signo, &handler, NULL);
}

pid_t checked_fork() {
   pid_t pid = fork();
   if (pid < 0) {
      perror("");
      exit(-1);
   }
   return pid;
}

int get_port(int argc, char **argv) {
   int result = 0;
   if (argc != 2) {
      fprintf(stderr, "Two arguments required, %d given\n", argc);
      exit(-1);
   }
   result = atoi(argv[1]);
   if (result == 0) {
      fprintf(stderr, "Second argument must be the port number (integer)\n");
      exit(-1);
   }
   return result;
}

void handleRequest(int fd) {
   char buf[KB];
   int bytes_read;
   while ((bytes_read = read(fd, buf, KB)) > 0) {
      buf[bytes_read] = 0;
      printf("%s\n", buf);
   }
}

int main(int argc, char **argv) {
   int port = get_port(argc, argv);
   int fd = create_service(port, QUEUE_LIMIT);
   setup(SIGCHLD);
   while (1) {
      int connection_fd = accept_connection(fd);
      pid_t pid = checked_fork();
      if (pid == 0) {
         handleRequest(connection_fd);
      }
      close(connection_fd);
   }
   return 0;
}
