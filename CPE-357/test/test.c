#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

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
    struct stat buf1;
    struct stat buf2;
    stat("./old.txt", &buf1);
    stat("./new.txt", &buf2);
    printf("%s\n", ctime(&buf1.st_mtime));
    printf("%s\n", ctime(&buf2.st_mtime));
	return 0;
}
