#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define buf_size 2048
/*
   1 17.227
   2 8.598
   16 1.130
   32 0.651
   64 0.350
   128 0.201
   256 0.146
   512 0.087
   1024 0.037
   2048 0.026
*/


int main(void) {
   char *file_name = "/usr/lib/locale/locale-archive";
   /*char *file_name = "/home/ggchan/Keen/test.c";*/
   int file_desc = open(file_name, O_RDONLY);
   char buf[buf_size];
   while (read(file_desc, buf, buf_size) > 0) {
      /*printf("%s", buf);*/
   }
   printf("\nFd %d Size %d\n", file_desc, buf_size);
   return 0;
}
