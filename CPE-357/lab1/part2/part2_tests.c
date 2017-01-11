#include <stdio.h>
#include "checkit.h"
#include "part2.h"

void test_calc_negative_input() {
   checkit_double(calc(-1.0), 40);
}

void test_calc_positive_input() {
   checkit_double(calc(12), 49);
}

void test_calc_negative_result() {
   checkit_double(calc(-37.0), -14);
}

void test_calc_zero_result() {
   checkit_double(calc(-34.580247), 0);
}


void test_calc() {
   test_calc_negative_input();
   test_calc_positive_input();
   test_calc_negative_result();
   test_calc_zero_result();
}

int main(void) {
   test_calc();
   return 0;
}
