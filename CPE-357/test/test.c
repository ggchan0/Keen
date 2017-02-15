#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int main(void) {
	char first[] = " hello : why hi there ";
	char *token;
	char *delim = ":";
	token = strtok(first, delim);
	while (token != NULL) {
		token = trimwhitespace(token);
		printf("%s %d\n", token, (int) strlen(token));
		token = strtok(NULL, delim);
	}
	return 0;
}
