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
#include "web_utils.h"

void handleSIGCHLD(int signo);
void handleSIGPIPE(int signo);
void setup();
void revertSIGCHLD();
pid_t checked_fork();
int get_port(int argc, char **argv);
void buildPath(int fd, char *buf, char *path);
void getFileName(char *buf);
void cleanUpFile(char *file_name);
void handleExec(int fd, char *method, char *prog_name, char ** args);
void getProgram(int fd, char *prog_name, char *arg_string, char *program_line);
char **getArgs(char *malloced_prog_name, char *arg_string);
void freeArgs(char **args);
void CGIWithArgs(int fd, char *method, char *program_line);
void CGIWithoutArgs(int fd, char *method, char *program_line);
void executeCGI(int fd, char *method, char *path);
void executeGet(int fd, char *method, char *path);
void handleRequest(int fd);

void handleSIGCHLD(int signo) {
   waitpid(-1, NULL, WNOHANG);
}

void handleSIGPIPE(int signo) {
   printf("Connection terminated abruptly, quitting.\n");
   exit(-1);
}

void setup() {
   struct sigaction forkhandler;
   struct sigaction pipehandler;
   forkhandler.sa_handler = handleSIGCHLD;
   pipehandler.sa_handler = handleSIGPIPE;
   if (sigemptyset(&forkhandler.sa_mask) == -1) {
      perror("");
      exit(-1);
   }
   if (sigemptyset(&pipehandler.sa_mask) == -1) {
      perror("");
      exit(-1);
   }
   forkhandler.sa_flags = SA_RESTART;
   pipehandler.sa_flags = SA_RESTART;
   sigaction(SIGCHLD, &forkhandler, NULL);
   sigaction(SIGPIPE, &pipehandler, NULL);
}

void revertSIGCHLD() {
   struct sigaction forkhandler;
   forkhandler.sa_handler = SIG_IGN;
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

void getFileName(char *buf) {
   int length = snprintf(NULL, 0, "%d", (int) getpid());
   char *str = malloc(length + 12);
   snprintf(str, length + 12, "output-%d.txt", (int) getpid());
   strcpy(buf, str);
   free(str);
}

void cleanUpFile(char *file_name) {
   char *command = malloc(sizeof(char) * strlen(file_name) + 4);
   command[0] = 0;
   strcat(command, "rm ");
   strcat(command, file_name);
   system(command);
   free(command);
}

void getProgram(int fd, char *prog_name, char *arg_string, char *program_line) {
   char *delim = "?";
   char *token = strtok(program_line, delim);
   strcpy(prog_name, token);
   token = strtok(NULL, delim);
   if (token == NULL) {
      throw404Error(fd);
   }
   strcpy(arg_string, token);
}

char **getArgs(char *malloced_prog_name, char *arg_string) {
   int arg_count = 0, i = 1;
   char **args = NULL;
   char *delim = "&";
   Nodelist *list = initializeNodelist();
   char *token = strtok(arg_string, delim);
   CharNode *temp;
   while (token != NULL) {
      CharNode *node = initializeCharNode(strdup(token));
      addToNodelist(list, node);
      arg_count++;
      token = strtok(NULL, delim);
   }
   args = malloc(sizeof(char**) * (arg_count + 2));
   temp = list->head;
   while (temp != NULL) {
      args[i++] = strdup(temp->data);
      temp = temp->next;
   }
   args[0] = malloced_prog_name;
   args[i] = NULL;
   freeNodelist(list);
   free(list);
   return args;
}

void freeArgs(char **args) {
   int i = 0;
   while (args[i] != NULL) {
      free(args[i++]);
   }
   free(args);
}

void handleExec(int fd, char *method, char *prog_name, char ** args) {
   pid_t pid;
   char buf[KB];
   char file_name[KB];
   int bytes_read = 0, output_fd, status = 0;
   struct stat stat_buf;
   getFileName(file_name);
   pid = fork();
   if (pid == 0) {
      output_fd = open(file_name, O_WRONLY | O_CREAT,
                        S_IRUSR | S_IXUSR | S_IWUSR);
      /*close(1);
      dup(output_fd);
      close(output_fd);*/
      dup2(output_fd, 1);
      if (execv(prog_name, args) == -1) {
         throw500Error(fd);
      }
   } else if (pid < 0) {
      throw500Error(fd);
   }
   wait(&status);
   if (lstat(file_name, &stat_buf) == -1 || status != 0) {
      throw500Error(fd);
   }
   write200Head(fd, stat_buf.st_size);
   if (strcmp(method, "GET") == 0) {
      output_fd = open(file_name, O_RDONLY, S_IRUSR);
      while ((bytes_read = read(output_fd, buf, KB)) > 0) {
         write(fd, buf, bytes_read);
      }
      close(output_fd);
   }
   cleanUpFile(file_name);
}

void CGIWithArgs(int fd, char *method, char *program_line) {
   char prog_name[KB];
   char arg_string[KB];
   char *malloced_prog_name;
   char **args;
   struct stat stat_buf;
   getProgram(fd, prog_name, arg_string, program_line);
   malloced_prog_name = strdup(prog_name);
   args = getArgs(malloced_prog_name, arg_string);
   if (lstat(prog_name, &stat_buf) == -1) {
      throw404Error(fd);
   }
   if (!hasExecAccess(stat_buf)) {
      throw403Error(fd);
   }
   handleExec(fd, method, prog_name, args);
   freeArgs(args);
}

void CGIWithoutArgs(int fd, char *method, char *program_line) {
   char *args[2];
   struct stat stat_buf;
   if (lstat(program_line, &stat_buf) == -1) {
      throw404Error(fd);
   }
   if (!hasExecAccess(stat_buf)) {
      throw403Error(fd);
   }
   args[0] = program_line;
   args[1] = NULL;
   handleExec(fd, method, program_line, args);
}

void executeCGI(int fd, char *method, char *path) {
   revertSIGCHLD();
   char buf[KB];
   char *delim = "/";
   char *cgi_dir = NULL, *program_line = NULL;
   buf[0] = 0;
   buildPath(fd, buf, path);
   cgi_dir = strtok(buf, delim);
   chdir(cgi_dir);
   program_line = buf + strlen(cgi_dir) + 1;
   int hasArgs = strstr(program_line, "?") != NULL;
   if (hasArgs) {
      CGIWithArgs(fd, method, program_line);
   } else {
      CGIWithoutArgs(fd, method, program_line);
   }
}

void executeGet(int fd, char *method, char *path) {
   char buf[KB];
   char file_buf[KB];
   int file_fd;
   int bytes_read;
   buf[0] = 0;
   buildPath(fd, buf, path);
   struct stat stat_buf;
   if (lstat(buf, &stat_buf) == -1) {
      throw404Error(fd);
   }
   if (!hasReadAccess(stat_buf)) {
      throw403Error(fd);
   }
   if (S_ISDIR(stat_buf.st_mode)) {
      throw400Error(fd);
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
   if (method == NULL) {
      throw400Error(fd);
   }
   path = strtok(NULL, " ");
   if (path == NULL) {
      throw400Error(fd);
   }
   HTTP = strtok(NULL, " ");
   if (HTTP == NULL) {
      throw400Error(fd);
   }
   if (!isValidMethod(method)) {
      throw501Error(fd);
   }
   if (strstr(path, "/cgi-like") != NULL) {
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
      close(connection_fd);
   }
   close(fd);
   return 0;
}
