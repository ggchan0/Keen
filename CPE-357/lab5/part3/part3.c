#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
   printf("_SC_CHILD_MAX: %ld\n", sysconf(_SC_CHILD_MAX));
   printf("_SC_OPEN_MAX: %ld\n", sysconf(_SC_OPEN_MAX));
   printf("SC_PAGE_SIZE: %ld\n", sysconf(_SC_PAGE_SIZE));
   return 0;
}
