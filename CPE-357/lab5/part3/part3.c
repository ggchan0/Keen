#include <stdio.h>
#include <unistd.h>
#include <limits.h>

/*

   local: 
   _SC_CHILD_MAX: 4096
   _SC_OPEN_MAX: 1024
   _SC_PAGE_SIZE: 4096

   unix3:
   _SC_CHILD_MAX: 1024
   _SC_OPEN_MAX: 1024
   _SC_PAGE_SIZE: 4096

   unix13:
   _SC_CHILD_MAX: 1024
   _SC_OPEN_MAX: 1024
   _SC_PAGE_SIZE: 4096

*/

int main(void) {
   printf("_SC_CHILD_MAX: %ld\n", sysconf(_SC_CHILD_MAX));
   printf("_SC_OPEN_MAX: %ld\n", sysconf(_SC_OPEN_MAX));
   printf("_SC_PAGE_SIZE: %ld\n", sysconf(_SC_PAGE_SIZE));
   return 0;
}
