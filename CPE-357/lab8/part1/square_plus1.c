#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

void checkedPipe(int fds[]) {
   if (pipe(fds) == -1) {
      perror("");
      exit(-1);
   }
}

pid_t checkedFork() {
   pid_t pid = fork();
   if (pid < 0) {
      perror("");
      exit(-1);
   }
   return pid;
}

char * doubleSize(char *str, int length) {
   return realloc(str, length);
}

char *readline(FILE *file) {
   int length = 10;
   int index = 0;
   char *str = malloc(sizeof(char) * length);
   int c;
   while ((c = fgetc(file)) != EOF && c != '\n') {
      str[index++] = c;
      if (index == (length - 1)) {
         length *= 2;
         str = doubleSize(str, length);
      }
   }
   str[index] = '\0';
   if (c == EOF) {
      free(str);
      str = NULL;
   }
   return str;
}

void childOne(int rd, int wr) {
   char input[BUF_SIZE];
   char buf[BUF_SIZE];
   int num;
   int bytes_read;
   while ((bytes_read = read(rd, input, BUF_SIZE)) > 0) {
      input[bytes_read] = '\0';
      num = atoi(input);
      num = num * num;
      snprintf(buf, BUF_SIZE, "%d", num);
      write(wr, buf, bytes_read + 1);
   }
   close(rd);
   close(wr);
   exit(0);
}

void childTwo(int rd, int wr) {
   char input[BUF_SIZE];
   char buf[BUF_SIZE];
   int num;
   int bytes_read;
   while ((bytes_read = read(rd, input, BUF_SIZE)) > 0) {
      input[bytes_read] = '\0';
      num = atoi(input);
      num = num + 1;
      snprintf(buf, BUF_SIZE, "%d", num);
      write(wr, buf, bytes_read + 1);
   }
   close(rd);
   close(wr);
   exit(0);
}

int main(void) {
   char * input;
   int p2c[2];
   int c2c[2];
   int c2p[2];
   pid_t pid_1, pid_2;
   checkedPipe(p2c);
   checkedPipe(c2c);
   checkedPipe(c2p);

   pid_1 = checkedFork();
   if (pid_1 == 0) {
      close(p2c[1]);
      close(c2p[0]);
      close(c2p[1]);
      close(c2c[0]);
      childOne(p2c[0], c2c[1]);
   }

   pid_2 = checkedFork();
   if (pid_2 == 0) {
      close(p2c[0]);
      close(p2c[1]);
      close(c2c[1]);
      close(c2p[0]);
      childTwo(c2c[0], c2p[1]);
   }

   close(p2c[0]);
   close(c2c[0]);
   close(c2c[1]);
   close(c2p[1]);

   while ((input = readline(stdin)) != NULL) {
      char returned[BUF_SIZE];
      write(p2c[1], input, strlen(input));
      read(c2p[0], returned, BUF_SIZE);
      fprintf(stdout, "%s\n", returned);
      free(input);
   }
   printf("Closing\n");
   close(p2c[1]);
   close(c2p[0]);
   wait(NULL);
   wait(NULL);
   return 0;
}
