#include <stdio.h>
#include <stdlib.h>

char * doubleSize(char *str, int length) {
   char *new_str = malloc(sizeof(char) * length * 2);
   int i;
   for (i = 0; i < length; i++) {
      new_str[i] = str[i];
   }
   free(str);
   return new_str;
}

char *readline(FILE *file) {
   int length = 40;
   int index = 0;
   char *str = malloc(sizeof(char) * length);
   int c;
   while ((c = fgetc(file)) != EOF && c != '\n') {
      str[index++] = c;
      if (index == length - 1) {
         length *= 2;
         str = doubleSize(str, length);
      }
   }
   if (c == EOF) {
      str = NULL;
   } else {
      str[index] = '\0';
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
