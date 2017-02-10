#include <stdio.h>
#include <stdlib.h>

#define KILO 1024

/*
   Unsuccessful at 5000 kb
*/

int main(void) {
   char *ptr = malloc(KILO * KILO * 10);
   if (ptr == NULL) {
      printf("Malloc unsuccessful\n");
   } else {
      printf("Successfully malloced\n");
      free(ptr);
   }
   return 0;
}