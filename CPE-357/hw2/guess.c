#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 0
#define RIGHT 1


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

int isYes(char *str) {
   if (str[0] == 'y' || str[0] == 'Y') {
      return 1;
   } else {
      return 0;
   }
}

int main(void) {
   while (1) {
      char *str = readline(stdin);
      if (str == NULL) {
         free(str);
         break;
      } else {
         if (str[0] == '\0') {
            printf("Empty string\n");
         } else if  (isYes(str)) {
            printf("Yes\n");
         } else {
            printf("No\n");
         }
         free(str);
      }
   }
}