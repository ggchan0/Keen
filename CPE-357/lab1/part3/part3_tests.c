#include <stdio.h>
#include "part3.h"
#include "checkit.h"

#define LENGTH 10

void test_sum_all_positives() {
   int arr[10];
   int i;
   for (i = 0; i < LENGTH; i++) {
      arr[i] = i + 1;
   }
   checkit_int(sum(arr, LENGTH), 55);
}

void test_sum_empty_array() {
   int arr[5];
   int i;
   for (i = 0; i < LENGTH; i++) {
      arr[i] = i + 1;
   }
   checkit_int(sum(arr, 0), 0);
}

void test_sum_mixed_integers() {
   int arr[10];
   int i;
   for (i = 0; i < LENGTH; i++) {
      if (i % 2 == 0) {
         arr[i] = -1 * i;
      } else {
         arr[i] = i;
      }
   }
   checkit_int(sum(arr, LENGTH), 5);
}

void test_sum() {
   test_sum_all_positives();
   test_sum_empty_array();
   test_sum_mixed_integers();
}

int main(void) {
   test_sum();
   return 0;
}
