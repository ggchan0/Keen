#include <stdio.h>
#include "part3.h"

int sum(int arr[], int length) {
   int sum = 0;
   int i;
   for (i = 0; i < length; i++) {
      sum += arr[i];
   }
   return sum;
}