#include <stdio.h>
#include <stdlib.h>

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
   FILE *f = fopen("test.txt", "r");
   if (f == NULL) {
      printf("Error opening file\n");
      return 1;
   }
   while (1) {
      char *str = readline(f);
      if (str == NULL) {
         free(str);
         break;
      } else {
         printf("%s\n", str);
         free(str);
      }
   }
   return 0;
}
