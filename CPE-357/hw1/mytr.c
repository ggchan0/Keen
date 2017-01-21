#include <stdio.h>
#include <string.h>

int get_mode(int argc, char ** args) {
   int i;
   int code = 0;
   for (i = 0; i < argc; i++) {
      char *arg = *(args + i);
      if (strcmp(arg, "-d") == 0) {
        code = 1;
      }
   }
   return code;
}

int char_in_delete_set(char c, char *delete_set) {
   int i = 0;
   int delete_char = 0;
   while (delete_set[i] != '\0') {
      if (delete_set[i] == '\\') {
         i++;
         switch(delete_set[i]) {
            case '\0':
               i--;
               break;

            case '\\':
               delete_char = (c == '\\');
               break;

            case 'n':
               delete_char = (c == '\n');
               break;

            case 't':
               delete_char = (c == '\t');
               break;

            default:
               delete_char = (c == delete_set[i]);
               break;
         }
      } else {
         delete_char = (c == delete_set[i]);
      }
      if (delete_char > 0) {
         break;
      }
      i++;
   }
   return delete_char;
}

char *get_delete_set(char ** argv) {
   char *delete_set;
   if (strcmp(argv[1], "-d") == 0) {
      delete_set = argv[2];
   } else {
      delete_set = argv[1];
   }
   return delete_set;
}

void execute_delete_mode(char c, char *delete_set) {
   if (char_in_delete_set(c, delete_set) == 0) {
      putchar(c);
   }
}

int main(int argc, char **argv) {
   if (argc != 3) {
	   printf("Program requires only 3 arguments, %d given\n", argc);
	   return 1;
   }
   int code = get_mode(argc, argv);
   int c;
   while ((c = getchar()) != EOF) {
      if (code == 1) {
         char *delete_set = get_delete_set(argv);
         execute_delete_mode(c, delete_set);
      } else {

      }
   }
   return 0;
}
