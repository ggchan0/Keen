#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
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
         exit(-1);
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
         exit(-1);
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

int isHidden(char *arg) {
   return arg[0] == '.';
}

char getUserExecuteBit(struct stat buf) {
   if ((buf.st_mode & S_ISUID)) {
      if ((buf.st_mode & S_IXUSR)) {
         return 's';
      } else {
         return 'S';
      }
   }
   if ((buf.st_mode & S_IXUSR)) {
      return 'x';
   } else {
      return '-';
   }
}

char getGroupExecuteBit(struct stat buf) {
   if ((buf.st_mode & S_ISUID)) {
      if ((buf.st_mode & S_IXGRP)) {
         return 's';
      } else {
         return 'S';
      }
   }
   if ((buf.st_mode & S_IXGRP)) {
      return 'x';
   } else {
      return '-';
   }
}

void printPermissions(struct stat buf, char c) {
   printf("[");
   printf("%c", c);
   printf((buf.st_mode & S_IRUSR) ? "r" : "-");
   printf((buf.st_mode & S_IWUSR) ? "w" : "-");
   printf("%c", getUserExecuteBit(buf));
   printf((buf.st_mode & S_IRGRP) ? "r" : "-");
   printf((buf.st_mode & S_IWGRP) ? "w" : "-");
   printf("%c", getGroupExecuteBit(buf));
   printf((buf.st_mode & S_IROTH) ? "r" : "-");
   printf((buf.st_mode & S_IWOTH) ? "w" : "-");
   printf((buf.st_mode & S_IXOTH) ? "x" : "-");
   printf("]");
}

char getFileTypeChar(struct stat buf) {
   char c;
   if (S_ISBLK(buf.st_mode)) {
      c = 'b';
   } else if (S_ISCHR(buf.st_mode)) {
      c = 'c';
   } else if (S_ISDIR(buf.st_mode)) {
      c = 'd';
   } else if (S_ISFIFO(buf.st_mode)) {
      c = 'p';
   } else if (S_ISREG(buf.st_mode)) {
      c = '-';
   } else if (S_ISLNK(buf.st_mode)) {
      c = 'l';
   } else {
      c = 's';
   }
   return c;
}

void printIndent(int level) {
   int i;
   printf("|");
   for (i = 0; i < level; i++) {
      printf("   |");
   }
}
