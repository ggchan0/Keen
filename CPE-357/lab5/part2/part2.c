#include <stdio.h>

#define buf_size 1

/*
   1 0.022
   2 0.037
   16 0.015
   32 0.017
   64 0.025
   128 0.024
   256 0.023
   512 0.019
   1024 0.029
   2048 0.023
*/

int main(void) {
   char *file_name = "/usr/lib/locale/locale-archive";
   /*char *file_name = "/home/ggchan/Keen/test.c";*/
   char buf[buf_size];
   FILE *fp = fopen(file_name, "r");
   while (fread(buf, sizeof(char), buf_size, fp) > 0) {
      /*printf("%s", buf);*/
   }
   printf("\nSize %d\n", buf_size);
   return 0;
}