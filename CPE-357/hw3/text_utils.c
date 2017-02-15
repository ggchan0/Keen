#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *doubleSize(char *str, int length) {
   return realloc(str, length);
}

char *readline(FILE *file) {
   int length = 10;
   int index = 0;
   char *str = malloc(sizeof(char) * length);
   int c;
   while ((c = fgetc(file)) != EOF && c != '\n') {
      str[index++] = c;
      if (index == (length - 1)) {
         length *= 2;
         str = doubleSize(str, length);
      }
   }
   str[index] = '\0';
   if (c == EOF) {
      free(str);
      str = NULL;
   }
   return str;
}

int fileExists(char * file_name) {
   FILE *file = fopen(file_name, "r");
   if (file) {
      fclose(file);
      return 1;
   } else {
      return 0;
   }
}

/*found on stackoverflow */
char *trimwhitespace(char *str) {
   char *end;

   while(isspace((unsigned char)*str)) str++;

   if(*str == 0)
    return str;

   end = str + strlen(str) - 1;
   while(end > str && isspace((unsigned char)*end)) end--;

   *(end+1) = 0;

   return str;
}

int isEmpty(char *line) {
   return line[0] == '\0';
}

int isCommand(char *line) {
   return line[0] == '\t';
}

int hasTarget(char *line) {
   return line[0] != ':';
}

int isValidRule(char *line) {
   int colon_count = 0;
   int index = 0;
   while (*(line + index) != '\0') {
      if (*(line + index) == ':') {
         colon_count++;
      }
   }
   return colon_count == 1;
}
