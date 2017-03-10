#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handleSignal(int signo);
void setup(int signo);

void handleSignal(int signo) {
   printf("Caught ctrl c - enter 'quit' to exit\n");
   setup(signo);
}

void setup(int signo) {
   /*
   struct sigaction sigIntHandler;
   sigIntHandler.sa_handler = handleSignal;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = SA_SIGINFO;
   sigaction(signo, &sigIntHandler, NULL);
   */
   signal(signo, handleSignal);
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

int main(void) {
   setup(SIGINT);
   char * str;
   while ((str = readline(stdin)) != NULL && strcmp(str, "quit") != 0) {
      printf("%s\n", str);
      free(str);
   }
   free(str);
}
