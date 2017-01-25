#include <stdio.h>

char * readline(FILE *file) {
   
}

int main(void) {
   FILE *f = fopen("test.txt", "r");
   int c;
   if (f == NULL) {
      printf("Error opening file\n");
      return 1;
   }
   while ((c = fgetc(f)) != EOF) {
      printf("%c", c);
   }
   return 0;
}
