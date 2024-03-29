#include <stdio.h>
#include <string.h>

int get_mode(int argc, char ** args) {
   int delete_mode = strcmp(args[1], "-d") == 0;
   return delete_mode;
}

int match_special_chars(char c, char cur_c) {
   int delete_char = 0;
   switch(cur_c) {
      case '\0':
         delete_char = (c == '\\');
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
         delete_char = (c == cur_c);
         break;
   }
   return delete_char;
}

int char_in_delete_set(char c, char *delete_set) {
   int i = 0;
   int delete_char = 0;
   while (delete_set[i] != '\0') {
      if (delete_set[i] == '\\') {
         delete_char = match_special_chars(c, delete_set[++i]);
         if (delete_set[i] == '\0') {
            i--;
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

void execute_delete_mode(char c, char *delete_set) {
   if (char_in_delete_set(c, delete_set) == 0) {
      putchar(c);
   }
}

int find_sub_position(char c, char *set) {
   int pos = 1;
   int final_pos = -1;
   int i = 0;
   int sub_char = 0;
   while (set[i] != '\0') {
      if (set[i] == '\\') {
         sub_char = match_special_chars(c, set[++i]);
         if (set[i] == '\0') {
            i--;
         }
      } else {
         sub_char = (c == set[i]);
      }
      if (sub_char > 0) {
         final_pos = pos;
         sub_char = 0;
      }
      pos++;
      i++;
   }
   return final_pos;
}

char find_sub_char(int sub_pos, char *set) {
   int pos = 1;
   int i = 0;
   char final_c;
   while (set[i] != '\0') {
      if (set[i] == '\\') {
         i++;
         switch(set[i]) {
            case '\0':
               i--;
               final_c = '\\';
               break;

            case '\\':
               final_c = '\\';
               break;

            case 'n':
               final_c = '\n';
               break;

            case 't':
               final_c = '\t';
               break;

            default:
               final_c = set[i];
               break;
         }
      } else {
         final_c = set[i];
      }
      if (sub_pos == pos) {
         break;
      }
      pos++;
      i++;
   }
   return final_c;
}

void execute_substitute_mode(char c, char **argv) {
   int sub_pos = find_sub_position(c, argv[1]);
   if (sub_pos > 0) {
      char subbed_char = find_sub_char(sub_pos, argv[2]);
      putchar(subbed_char);
   } else {
      putchar(c);
   }
}

int main(int argc, char **argv) {
   int delete_mode;
   int c;
   if (argc != 3) {
	   fprintf(stderr, "ERROR: Program requires only 3 arguments, %d given\n", argc);
	   return 1;
   }
   if (strcmp(argv[1], "") == 0 || strcmp(argv[2], "") == 0) {
      fprintf(stderr, "ERROR: One of the inputted sets is empty.\n");
      return 1;
   }
   delete_mode = get_mode(argc, argv);
   while ((c = getchar()) != EOF) {
      if (delete_mode == 1) {
         execute_delete_mode(c, argv[2]);
      } else {
         execute_substitute_mode(c, argv);
      }
   }
   return 0;
}