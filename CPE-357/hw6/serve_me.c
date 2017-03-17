#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include "simple_net.h"
#include "web_utils.c"

#define QUEUE_LIMIT 10
#define KB 1024

void handleSIGCHLD(int signo);
void setup();
pid_t checked_fork();
int get_port(int argc, char **argv);
int hasReadAccess(struct stat stat_buf);
void write200Head(int fd, int content_length);
void throw400Error(int fd);
void throw403Error(int fd);
void throw404Error(int fd);
void throw500Error(int fd);
void throw501Error(int fd);
void buildPath(int fd, char *buf, char *path);
void executeCGI(int fd, char *method, char *path);
void executeGet(int fd, char *method, char *path);
void handleRequest(int fd);

void handleSIGCHLD(int signo) {
   waitpid(-1, NULL, WNOHANG);
}

void setup() {
   struct sigaction forkhandler;
   forkhandler.sa_handler = handleSIGCHLD;
   if (sigemptyset(&forkhandler.sa_mask) == -1) {
      perror("");
      exit(-1);
   }
   forkhandler.sa_flags = SA_RESTART;
   sigaction(SIGCHLD, &forkhandler, NULL);
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

int hasReadAccess(struct stat stat_buf) {
   return stat_buf.st_mode & S_IRUSR || \
   stat_buf.st_mode & S_IRGRP || \
   stat_buf.st_mode & S_IROTH;
}

void write200Head(int fd, int content_length) {
   char buf[KB];
   buf[0] = 0;
   snprintf(buf, KB, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
                     "Content-Length: %d\r\n\r\n", content_length);
   int length = strlen(buf);
   write(fd, buf, length);
}

void throw400Error(int fd) {
   char *response = "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n"
                     "Content-Length: 51\r\n\r\n"
                     "400 Request could not be understood by the server.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw403Error(int fd) {
   char *response = "HTTP/1.0 403 Permission Denied\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: 48\r\n\r\n"
                     "403 Access to the following resource is denied.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw404Error(int fd) {
   char *response = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n"
                     "Content-Length: 24\r\n\r\n"
                     "404 Resource not found.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw500Error(int fd) {
   char *response = "HTTP/1.0 500 Internal Error\r\nContent-Type: text/html\r\n"
                     "Content-Length: 44\r\n\r\n"
                     "500 Server encountered an unexpected error.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw501Error(int fd) {
   char *response = "HTTP/1.0 501 Not Implemented\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: 31\r\n\r\n"
                     "501 HTTP method not supported.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void buildPath(int fd, char *buf, char *path) {
   char *delim = "/";
   char *token = strtok(path, delim);
   while (token != NULL) {
      if (strcmp(token, "..") == 0 || strcmp(token, "~") == 0) {
         throw403Error(fd);
      }
      strcat(buf, token);
      strcat(buf, "/");
      token = strtok(NULL, delim);
   }
   buf[strlen(buf) - 1] = 0;
}

void executeCGI(int fd, char *method, char *path) {

}

void executeGet(int fd, char *method, char *path) {
   char buf[KB];
   char file_buf[KB];
   int file_fd;
   int bytes_read;
   buf[0] = 0;
   buildPath(fd, buf, path);
   printf("%s\n", buf);
   struct stat stat_buf;
   if (lstat(buf, &stat_buf) == -1) {
      throw404Error(fd);
   }
   if (!hasReadAccess(stat_buf)) {
      throw403Error(fd);
   }
   write200Head(fd, stat_buf.st_size);
   if (strcmp(method, "GET") == 0) {
      file_fd = open(buf, O_RDONLY);
      while ((bytes_read = read(file_fd, file_buf, KB)) > 0) {
         write(fd, file_buf, bytes_read);
      }
   }
}

void handleRequest(int fd) {
   FILE *request = fdopen(fd, "r");
   char *line = readline(request);
   char *method, *path, *HTTP;
   int mode = 0;
   method = strtok(line, " ");
   if (method == NULL) throw400Error(fd);
   path = strtok(NULL, " ");
   if (path == NULL) throw400Error(fd);
   HTTP = strtok(NULL, " ");
   if (HTTP == NULL) throw400Error(fd);
   if (!isValidMethod(method)) {
      throw501Error(fd);
   }
   printf("%s\n", line);
   if (strstr(path, "cgi-like") != NULL) {
      mode = 1;
   }
   if (mode) {
      executeCGI(fd, method, path);
   } else {
      executeGet(fd, method, path);
   }
   fclose(request);
   close(fd);
   free(line);
   exit(0);
}

int main(int argc, char **argv) {
   int port = get_port(argc, argv);
   int fd = create_service(port, QUEUE_LIMIT);
   setup();
   while (1) {
      int connection_fd = accept_connection(fd);
      pid_t pid = checked_fork();
      if (pid == 0) {
         handleRequest(connection_fd);
      }
   }
   return 0;
}
