#include <stdio.h>
#include <time.h>

int main() {
   clock_t start, end;
   double time_taken;
   start = clock();
   int arr[] = {1,2};
   printf("%d\n", *(arr));
   printf("%d\n", *(arr+1));
   end = clock();
   time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("%f\n", time_taken);
   return 0;
}
