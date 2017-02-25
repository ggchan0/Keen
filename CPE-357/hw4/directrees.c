#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "utilitrees.c"
#include <sys/dir.h>

int checkedChdir(char *dir) {
   int status;
   if ((status = chdir(dir)) == -1) {
      printf("Cannot change into directory %s\n", dir);
   }
   if (status == -1) {
      return 0;
   } else {
      return 1;
   }
}

int isSpecial(char *dir) {
   return strcmp(dir, ".") == 0 || strcmp(dir, "..") == 0;
}

void printDirs(char *dir, int level, int print_hidden, int print_perms) {
   char cwd[1024];
   int i, n;
   struct stat buf;
   char type;
   struct dirent **namelist;
   n = scandir(dir, &namelist, 0, alphasort);
   if (getcwd(cwd, sizeof(cwd)) == NULL) {
      fprintf(stderr, "Cannot get current working directory\n");
   }
   strcat(cwd, "/");
   if (n < 0) {
      perror("scandir");
   } else {
      for (i = 0; i < n; i++) {
         char *entry = namelist[i]->d_name;
         if (!isSpecial(entry)) {
            if (stat(entry, &buf) == -1) {
               printf("Could not read file %s", namelist[i]->d_name);
            } else {
               if ((type = getFileTypeChar(buf)) != 'd') {
                  if (!isHidden(entry) || print_hidden) {
                     printIndent(level);
                     printf("-- ");
                     if (print_perms) {
                        printPermissions(buf, type);
                        printf(" ");
                     }
                     printf("%s\n", entry);
                  }
               } else {
                  if (isHidden(namelist[i]->d_name) && print_hidden) {
                     printIndent(level);
                     if (print_perms) {
                        printPermissions(buf, type);
                     }
                     printf("--%s\n", namelist[i]->d_name);
                     if (chdir(namelist[i]->d_name) == -1) {
                        printf("Cannot change into directory %s\n", namelist[i]->d_name);
                     } else {
                        char new_cwd[1024];
                        if (getcwd(new_cwd, sizeof(new_cwd)) == NULL) {
                           fprintf(stderr, "Cannot get current working directory\n");
                        }
                        strcat(new_cwd, "/");
                        printDirs(new_cwd, level + 1, print_hidden, print_perms);
                        if (chdir(cwd) == -1) {
                           printf("Cannot change into directory %s\n", cwd);
                        }
                     }
                  } else if (!isHidden(namelist[i]->d_name)) {
                     printIndent(level);
                     if (print_perms) {
                        printPermissions(buf, type);
                     }
                     printf("--%s\n", namelist[i]->d_name);
                     if (chdir(namelist[i]->d_name) == -1) {
                        printf("Cannot change into directory %s\n", namelist[i]->d_name);
                     } else {
                        char new_cwd[1024];
                        if (getcwd(new_cwd, sizeof(new_cwd)) == NULL) {
                           fprintf(stderr, "Cannot get current working directory\n");
                        }
                        strcat(new_cwd, "/");
                        printDirs(new_cwd, level + 1, print_hidden, print_perms);
                        if (chdir(cwd) == -1) {
                           printf("Cannot change into directory %s\n", cwd);
                        }
                     }
                  }
               }
            }
         }
         free(namelist[i]);
      }
      free(namelist);
   }
}

void executePrintDirs(char *dir, struct stat buf, char type, int print_hidden, int print_perms) {
   if (print_perms) {
      printPermissions(buf, type);
   }
   printf(" %s\n", dir);
   printDirs(".", 0, print_hidden, print_perms);
}

/*
int main(int argc, char **argv) {
   int print_hidden = 0, print_perms = 0;
   int i;
   struct stat buf;
   char type;
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) == NULL) {
      fprintf(stderr, "Cannot get current working directory\n");
   }
   strcat(cwd, "/");
   for (i = 1; i < argc; i++) {
      if (isFlag(argv[i])) {
         print_hidden = print_hidden | checkHidden(argv[i]);
         print_perms = print_perms | checkPerms(argv[i]);
      } else {
         if (isAbsolutePath(argv[i])) {
            if (stat(argv[i], &buf) == -1) {
               printf("%s does not exist\n", argv[i]);
               continue;
            }
            if ((type = getFileTypeChar(buf)) != 'd') {
               if (print_perms) {
                  printPermissions(buf, type);
               }
               printf("%s", argv[i]);
            } else {
               if (chdir(argv[i]) == -1) {
                  printf("Cannot change into directory %s\n", argv[i]);
               } else {
                  printf("%s\n", argv[i]);
                  printDirs(".", 0, print_hidden, print_perms);
               }
            }
         } else {
            chdir(cwd);
            if (chdir(argv[i]) == -1) {
               printf("Cannot change into local directory %s\n", argv[i]);
            } else {
               printf("%s\n", argv[i]);
               printDirs(".", 0, print_hidden, print_perms);
            }
         }
      }
   }
   if (argc == 1) {
      chdir(cwd);
      printf("%s\n", cwd);
      printDirs(cwd, 0, print_hidden, print_perms);
   }
   return 0;
}*/

int main(int argc, char **argv) {
   int print_hidden = 0, print_perms = 0, file_hit = 0;
   int i;
   struct stat buf;
   char type;
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) == NULL) {
      fprintf(stderr, "Cannot get current working directory\n");
   }
   strcat(cwd, "/");
   for (i = 1; i < argc; i++) {
      if (isFlag(argv[i]) && file_hit == 0) {
         print_hidden = print_hidden | checkHidden(argv[i]);
         print_perms = print_perms | checkPerms(argv[i]);
      } else {
         file_hit = 1;
         if (stat(argv[i], &buf) == -1) {
            printf("%s does not exist\n", argv[i]);
            continue;
         }
         if ((type = getFileTypeChar(buf)) != 'd') {
            if (print_perms) {
               printPermissions(buf, type);
            }
            printf("%s\n", argv[i]);
         } else {
            /*
            if (!isAbsolutePath(argv[i])) {
               checkedChdir(cwd);
            }
            if (!checkedChdir(argv[i])) {
               continue;
            }*/
            if (!checkedChdir(argv[i])) {
               continue;
            }
            executePrintDirs(argv[i], buf, type, print_hidden, print_perms);
            checkedChdir(cwd);
         }
      }
   }
   if (!file_hit) {
      if (stat(cwd, &buf) == -1) {
         printf("%s does not exist\n", cwd);
      } else {
         type = getFileTypeChar(buf);
         executePrintDirs(cwd, buf, type, print_hidden, print_perms);
      }
   }
   return 0;
}
