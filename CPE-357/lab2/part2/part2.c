#include <stdio.h>
#include "part2.h"
#include <ctype.h>

void str_lower(char str1[], char str2[]) {
   int i = 0;
   while (*(str1 + i) != '\0') {
      *(str2 + i) = tolower(*(str1 + i));
      i++;
   }
   *(str2 + i) = '\0';
}

void str_lower_mutate(char str[]) {
   int i;
   for (i = 0; *(str + i) != '\0'; i++) {
      str[i] = tolower(str[i]);
   }
}
