#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

typedef struct Node {
   char *data;
   struct Node *next;
} Node;

char *doubleSize(char *str, int length) {
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

int isValidMethod(char *method) {
   return strcmp(method, "GET") == 0 || strcmp(method, "HEAD") == 0;
}
