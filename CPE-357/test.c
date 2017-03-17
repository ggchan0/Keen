#include <stdio.h>
#include <string.h>

int main(void) {
	char stuff[] = "    here  is a test string";
	char *token;
	char *delim = " ";
	token = strtok(stuff, delim);
	while (token != NULL) {
		printf("%s %d\n", token, (int) strlen(token));
		token = strtok(NULL, delim);
	}
	return 0;
}
