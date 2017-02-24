#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

int checkHidden(char *arg) {
   int i = 1;
   int print_hidden = 0;
   while (*(arg + i)) {
      if (*(arg + i) == 'a') {
         print_hidden = 1;
      } else if (*(arg + i) == 'l') {

      } else {
         fprintf(stderr, "Invalid flag %c\n", *(arg + i));
         exit(EXIT_FAILURE);
      }
      i++;
   }
   return print_hidden;
}

int checkPerms(char *arg) {
   int i = 1;
   int print_perms = 0;
   while (*(arg + i)) {
      if (*(arg + i) == 'l') {
         print_perms = 1;
      } else if (*(arg + i) == 'a') {

      } else {
         fprintf(stderr, "Invalid flag %c\n", *(arg + i));
         exit(EXIT_FAILURE);
      }
      i++;
   }
   return print_perms;
}

int isFlag(char *arg) {
   return arg[0] == '-';
}

int isAbsolutePath(char *arg) {
   return arg[0] == '/';
}

void printDirs(char *dir, int level, int print_hidden, int print_perms) {
   struct dirent **namelist;
   int i, n;
   n = scandir(cwd, &namelist, 0, alphasort);
   if (n < 0) {
      perror("scandir");
   } else {
      for (i = 0; i < n; i++) {
         printf("%s\n", namelist[i]->d_name);
         free(namelist[i]);
      }
      free(namelist);
   }
}

int main(int argc, char **argv) {
   int print_hidden = 0, print_perms = 0;
   int i;
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) == NULL) {
      fprintf(stderr, "Cannot get current working directory\n");
   }
   strcat(cwd, "/");
   for (i = 0; i < argc; i++) {
      if (isFlag(argv[i])) {
         print_hidden = print_hidden | checkHidden(argv[i]);
         print_perms = print_perms | checkPerms(argv[i]);
         printf("%d %d\n", print_hidden, print_perms);
      } else {
         if (isAbsolutePath(argv[i])) {
            if (chdir(argv[i]) == -1) {
               fprintf(stderr, "Could not chdir to %s\n", argv[i]);
               exit(EXIT_FAILURE);
            }
         } else {
            if (chdir(cwd) == -1) {
               fprintf(stderr, "Could not chdir to %s\n", cwd);
               exit(EXIT_FAILURE);
            }
         }
         printDirs(".", 0, print_hidden, print_perms);
      }
   }
}