#include <stdio.h>
#include "checkit.h"
#include "part1.h"

void test_swap_two_positives() {
  int p1 = 4;
  int p2 = 3;
  swap(&p1, &p2);
  checkit_int(p1, 3);
  checkit_int(p2, 4);
}

void test_swap_mixed_numbers() {
  int p1 = -3;
  int p2 = 3;
  swap(&p1, &p2);
  checkit_int(p1, 3);
  checkit_int(p2, -3);
}

void test_swap_same_number() {
  int p1 = 3;
  int p2 = 3;
  swap(&p1, &p2);
  checkit_int(p1, 3);
  checkit_int(p2, 3);
}

void test_swap() {
  test_swap_two_positives();
  test_swap_mixed_numbers();
  test_swap_same_number();
}

int main(void) {
  test_swap();
  return 0;
}
