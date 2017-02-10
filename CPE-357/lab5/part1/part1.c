#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#define buf_size 2048
/*
   1 1.237379
   2 0.626163
   16 0.087342
   32 0.045484
   64 0.055523
   128 0.011084
   256 0.011234
   512 0.003130
   1024 0.004555
   2048 0.003574
*/


int main(void) {
   char *file_name = "/usr/lib/locale/locale-archive";
   int file_desc = open(file_name, O_RDONLY);
   char buf[buf_size];
   clock_t start, end;
   double time_taken;
   start = clock();
   while (read(file_desc, buf, buf_size) > 0) {
      /*printf("%s", buf);*/
   }
   end = clock();
   time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("Fd %d Size %d", file_desc, buf_size);
   printf("\nIt took %f\n", time_taken);
   return 0;
}
