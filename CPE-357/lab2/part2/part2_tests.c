#include <stdio.h>
#include "part2.h"
#include "checkit.h"

void test_str_lower_all_lower() {
   char str1[] = "hello there";
   char str2[12];
   str_lower(str1, str2);
   checkit_string(str2, str1);
}

void test_str_lower_all_upper() {
   char str1[] = "HELLO THERE";
   char str2[12];
   str_lower(str1, str2);
   checkit_string(str2, "hello there");
}

void test_str_lower_non_alphabetic() {
   char str1[] = "!29-\t";
   char str2[6];
   str_lower(str1, str2);
   checkit_string(str2, str1);
}

void test_str_lower_empty_string() {
   char str1[] = "";
   char str2[1];
   str_lower(str1, str2);
   checkit_string(str2, str1);
}

void test_str_lower_mutate_all_lower() {
   char str1[] = "hello there";
   str_lower_mutate(str1);
   checkit_string(str1, "hello there");
}

void test_str_lower_mutate_all_upper() {
   char str1[] = "HELLO THERE";
   str_lower_mutate(str1);
   checkit_string(str1, "hello there");
}

void test_str_lower_mutate_non_alphabetic() {
   char str1[] = "!29-\t";
   str_lower_mutate(str1);
   checkit_string(str1, "!29-\t");
}

void test_str_lower_mutate_empty_string() {
   char str1[] = "";
   str_lower_mutate(str1);
   checkit_string(str1, "");
}

void test_str_lower() {
   test_str_lower_all_lower();
   test_str_lower_all_upper();
   test_str_lower_non_alphabetic();
   test_str_lower_empty_string();
}

void test_str_lower_mutate() {
   test_str_lower_mutate_all_lower();
   test_str_lower_mutate_all_upper();
   test_str_lower_mutate_non_alphabetic();
   test_str_lower_mutate_empty_string();
}

int main(void) {
   test_str_lower();
   test_str_lower_mutate();
   return 0;
}
